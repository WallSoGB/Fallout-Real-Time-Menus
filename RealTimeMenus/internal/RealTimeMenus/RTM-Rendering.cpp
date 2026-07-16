#include "RTM-Rendering.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/BSShaderManager.hpp"
#include "Bethesda/ImageSpaceManager.hpp"
#include "Bethesda/ImageSpaceModifierInstanceForm.hpp"
#include "Bethesda/ImageSpaceEffectDepthOfField.hpp"
#include "Bethesda/Interface.hpp"
#include "Bethesda/InterfaceManager.hpp"
#include "Bethesda/LockPickMenu.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/RendererSettingCollection.hpp"
#include "Bethesda/ScreenCustomSplatter.hpp"
#include "Bethesda/ScreenSplatter.hpp"
#include "Bethesda/StartMenu.hpp"
#include "Bethesda/TESMain.hpp"
#include "Gamebryo/NiDX9Renderer.hpp"

#pragma region Code

namespace RealTimeMenus {

	namespace Rendering {

		HookUtils::CallDetour kRenderScreenSplatterDetour;
		HookUtils::CallDetour kClearScreenSplatterDetour;

		bool bCanDoAsyncUI = false;
		bool bUIUpdated = false;
		bool bCanClearScreenSplatter = true;
		bool bRenderingRenderedMenuWorld = false;
		
		const Setting::Info iSettingAlwaysFalse(0);


		Utils::ImageSpaceStage eImageSpaceStage = Utils::IS_NONE;

		namespace {
			// If true, UI can run on the background thread
			static bool CanMultithreadUI() {
				return (!Interface::IsInMenuMode() || TESMain::bIsInPauseFade) && !TESMain::bConsoleOpen && !TESMain::GetSingleton()->bFreezeTime;
			}

			static bool ShouldUseNormalRenderPath() {
				InterfaceManager* pUIMgr = InterfaceManager::GetSingleton();
				return (Settings::IsMenuPaused(Interface::MainFour) && Interface::IsInPipBoy()) || (Settings::IsMenuPaused(Interface::Computers) && pUIMgr->pRenderedMenu && pUIMgr->pRenderedMenu != pUIMgr->pPipboyManager);
			}
		}

		static SPEC_NOINLINE void __fastcall RenderBackgroundWorld(TESMain* apMain, BSRenderedTexture* apMainTarget, BSRenderedTexture* apCustomRenderTarget, uint32_t auiCopyAddress) {
			NiDX9Renderer* pRenderer = NiDX9Renderer::GetSingleton();
			ImageSpaceManager* pISMgr = ImageSpaceManager::GetSingleton();
			const bool bIsMSAA = CdeclCall<bool>(0x4DC310);

			if (ShouldUseNormalRenderPath()) {
				ThisCall(auiCopyAddress, apMain, pISMgr, pRenderer, bIsMSAA, apCustomRenderTarget);
				return;
			}

			const bool bWireFrame = ThisCall<bool>(0x870770, BSShaderManager::GetShadowSceneNode(0)); // ShadowSceneNode::IsWireframe
			if (bWireFrame) {
				return;
			}

			eImageSpaceStage = Utils::IS_BG;

			apMain->DrawWorld_Init();
			apMain->DrawWorld_UpdateWater();

			const bool bEOF = ImageSpaceManager::bEOFEnabled;

			TESImageSpaceModifier* pISMod = reinterpret_cast<TESImageSpaceModifier*>(Utils::GetCurrentMenuBackgroundFX());

			apMain->DrawWorld_RestartRenderTexture(apMainTarget, bIsMSAA, NiRenderer::CLEAR_ZBUFFER | NiRenderer::CLEAR_BACKBUFFER);

			if (pISMod) {
				ImageSpaceModifierInstanceForm::Trigger(pISMod, 1.f, nullptr);
				apMain->UpdateImageSpace(false);
				ImageSpaceManager::bEOFEnabled = true;
			}

			bRenderingRenderedMenuWorld = true;
			apMain->DrawWorld_DrawScene(nullptr, false, bWireFrame, false);
			bRenderingRenderedMenuWorld = false;

			if (BSShaderManager::IsRefractionEnabled()) {
				if (ThisCall<uint32_t>(0xB63A90, apMain->spWorldAccum.m_pObject)) { // BSShaderAccumulator::GetRefractivePassCount
					apMain->DrawWorld_Refraction(apMain->spWorldAccum, apMainTarget);
				}
			}

			ImageSpaceEffectDepthOfField* pDOF = static_cast<ImageSpaceEffectDepthOfField*>(pISMgr->GetEffect(ImageSpaceManager::IS_EFFECT_DEPTH_OF_FIELD));
			if (pDOF && pDOF->DoDepth())
				apMain->DrawWorld_DoDepth(apMain->spWorldAccum, apMainTarget);

			if (!BSRenderedTexture::IsOutsideFrame())
				BSRenderedTexture::StopOffscreen();

			BSShaderManager::pCamera = TESMain::GetWorldSceneGraph()->GetCamera();
			BSShaderManager::RenderImageSpaceEffects(pRenderer, apMainTarget, apMainTarget);

			if (pISMod) {
				ImageSpaceModifierInstanceForm::Stop(pISMod);
				apMain->UpdateImageSpace(false);
				ImageSpaceManager::bEOFEnabled = bEOF;
			}

			apMain->DrawWorld_RestartRenderTexture(apMainTarget, bIsMSAA, NiRenderer::CLEAR_ZBUFFER);

			eImageSpaceStage = Utils::IS_NONE;
		}

		template<uint32_t uiAddress>
		class Hook_RenderBeckground {
			static inline HookUtils::CallDetour kDetour;

			void Hook(BSRenderedTexture* apMainTarget, BSRenderedTexture* apCustomRenderTarget) {
				TESMain* pThis = reinterpret_cast<TESMain*>(this);
				RenderBackgroundWorld(pThis, apMainTarget, apCustomRenderTarget, kDetour);
			}

		public:
			Hook_RenderBeckground() {
				kDetour.ReplaceCall(uiAddress, &Hook_RenderBeckground::Hook);
			}
		};

		template<uint32_t uiAddress, bool bMainThread>
		class Hook_InterfaceUpdate {
			static inline HookUtils::CallDetour kDetour;
			void InterfaceUpdateMT() {
				if (bCanDoAsyncUI && !bUIUpdated) {
					ThisCall(kDetour, this);
					bUIUpdated = true;
				}
			}

			void InterfaceUpdateST() {
				if (!bCanDoAsyncUI && !bUIUpdated) {
					ThisCall(kDetour, this);
					bUIUpdated = true;
				}
			}

		public:
			Hook_InterfaceUpdate() {
				if constexpr (bMainThread) {
					kDetour.ReplaceCall(uiAddress, &Hook_InterfaceUpdate::InterfaceUpdateST);
				}
				else {
					kDetour.ReplaceCall(uiAddress, &Hook_InterfaceUpdate::InterfaceUpdateMT);
				}
			}
		};

		template<uint32_t uiAddress>
		class Hook_BlurredDrawWorld {
		private:
			static inline HookUtils::CallDetour kDetour;
			void DrawWorld(void* apTexture, bool abRenderedMenu, bool abPipboyMode) {
				eImageSpaceStage = Utils::IS_FG;

				if (abRenderedMenu) {
					if (ShouldUseNormalRenderPath()) {
						ThisCall(kDetour, this, apTexture, abRenderedMenu, abPipboyMode);
						eImageSpaceStage = Utils::IS_NONE;
						return;
					}
				}

				bool bBlur = false;
				bool bEOF = true;
				TESImageSpaceModifier* pISMod = nullptr;

				if (Interface::IsInMenuMode()
					&& !Interface::IsTopMenuID(Interface::Menus::Dialog)
					&& !Interface::IsTopMenuID(Interface::Menus::MainFour)
					&& !StartMenu::MainMenuIsActive()
					&& !Interface::IsMenuIDVisible(Interface::Menus::VATS, 11)
					&& !Interface::IsInPipBoy()
					&& !Interface::IsConsoleVisible()) {

					eImageSpaceStage = Utils::IS_BG;

					bBlur = true;

					bEOF = ImageSpaceManager::bEOFEnabled;
					ImageSpaceManager::bEOFEnabled = true;

					pISMod = reinterpret_cast<TESImageSpaceModifier*>(Utils::GetCurrentMenuBackgroundFX());

					if (pISMod) {
						ImageSpaceModifierInstanceForm::Trigger(pISMod, 1.f, nullptr);
						reinterpret_cast<TESMain*>(this)->UpdateImageSpace(false);
					}
				}

				ThisCall(kDetour, this, apTexture, abRenderedMenu, abPipboyMode);

				if (bBlur) {
					if (pISMod) {
						ImageSpaceModifierInstanceForm::Stop(pISMod);
						reinterpret_cast<TESMain*>(this)->UpdateImageSpace(false);
					}

					ImageSpaceManager::bEOFEnabled = bEOF;
				}

				eImageSpaceStage = Utils::IS_NONE;
			}
		public:
			Hook_BlurredDrawWorld() {
				kDetour.ReplaceCall(uiAddress, &Hook_BlurredDrawWorld::DrawWorld);
			}
		};

		HookUtils::CallDetour kHandleMenuBackgroundDetour;
		HookUtils::CallDetour kSetMainRenderingDetour;
		HookUtils::CallDetour kUpdateOffscreenInterfaceDetour;
		HookUtils::CallDetour kSettingIntDetour;
		class Hook {
		public:
			void HandleMenuStaticBackground() {
				bool bIsInMenuMode = RealTimeMenus::Utils::IsMenuPausingGame();
				if (!bIsInMenuMode) {
					if (Interface::IsPipBoyOpening()) // Ignore opening because it'll blur too early
						return;

					if (Interface::IsInRenderedMenu()) {
						FORenderedMenu* pMenu = Interface::GetCurrentRenderedMenu();
						if (pMenu->bIsInitialized && !pMenu->IsRaceSexMenu()) {
							TESMain::bMenuBGReady = true;
							return;
						}
					}

					if (Interface::IsMenuIDVisible(Interface::Menus::LockPick, 0xF)) {
						TESMain::bMenuBGReady = true;
						return;
					}
				}

				ThisCall(kHandleMenuBackgroundDetour, this);
			}

			void DrawImageSpaceAndScrenSplatter(BOOL abMSAA, class BSRenderedTexture* apRenderTarget, class NiDX9Renderer* apRenderer, class BSRenderedTexture* apDestination) {
				ThisCall(kRenderScreenSplatterDetour, this, abMSAA, apRenderTarget, apRenderer, apDestination);
				if (ScreenCustomSplatter::IsActiveSplatter() || ScreenSplatter::IsActiveSplatter())
					reinterpret_cast<TESMain*>(this)->DrawWorld_ScreenSplatter(apRenderer);
			}

			static void UpdateOffscreenInterface() {
				if (Interface::IsMenuIDVisible(Interface::Menus::LockPick, 0))
					TESMain::GetSingleton()->DrawWorld_UpdateOffscreenBuffers();
				else
					CdeclCall(kUpdateOffscreenInterfaceDetour);
			}

			static void __cdecl SetMainRendering(bool abDefer) {
				bCanDoAsyncUI = CanMultithreadUI();
				if (!bCanDoAsyncUI) {
					TESMain::GetSingleton()->OnIdle_DoInterfaceIdle();
					bUIUpdated = true;
				}

				CdeclCall(kSetMainRenderingDetour, abDefer);
			}

			const Setting::Info* CanResume3DTasks() {
				if (bRenderingRenderedMenuWorld)
					return &iSettingAlwaysFalse;

				return ThisCall<Setting::Info*>(kSettingIntDetour, this);
			}
		};

	}

}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	namespace Rendering {

		void InitHooks() {

		}

		void InitDeferredHooks() {
			Hook_BlurredDrawWorld<0x870244>();
			Hook_BlurredDrawWorld<0x8702A9>();

			HookUtils::ReplaceCall(0x87212D, &Utils::GetCurrentMenuBackgroundFX);

			kHandleMenuBackgroundDetour.ReplaceCall(0x86E8FD, &Hook::HandleMenuStaticBackground);

			if (!Settings::IsMenuPaused(Interface::PipBoy)) {
				HookUtils::SafeWriteBuf(0x870009, "\xC6\x05\x29\xEA\x1D\x01\x01\x90\x90\x90\x90"); // Set bMenuBGReady=1 instead of rendering it
			}

			if (!Settings::IsMenuPaused(Interface::PipBoy) || !Settings::IsMenuPaused(Interface::Computers)) {
				//HookUtils::SafeWriteBuf(0x87085B, "\x6A\x00\x90\x90\x90", 5);
				HookUtils::PatchMemoryNopRange(0x870876, 0x870884);
				HookUtils::SafeWriteBuf(0x87087C, "\x90\x90\x90\x90\x90\x90\x52\x50");
				Hook_RenderBeckground<0x870887>(); // Call TESMain::DrawWorld_DrawScene in Rendered Menu render, instead of TESMain::DrawWorld_CopyMenuBG
			}

			HookUtils::PatchMemoryNopRange(0x872983, 0x872994);
			HookUtils::SafeWriteBuf(0x872983, "\x90\x90\x90\x8B\x55\x08\x52\x50");
			Hook_RenderBeckground<0x872997>(); // Call TESMain::DrawWorld_DrawScene in 3D Menu render, instead of TESMain::DrawWorld_CopyMenuBG

			// Switch UI update from AI thread to main thread properly
			Hook_InterfaceUpdate<0x8C7BE9, false>();	// Task 0
			Hook_InterfaceUpdate<0x8C7DB9, false>();	// Task 1
			Hook_InterfaceUpdate<0x86ECBA, true>();	// Main thread

			kSetMainRenderingDetour.ReplaceCall(0x86EC78, Hook::SetMainRendering);
			kUpdateOffscreenInterfaceDetour.ReplaceCall(0x8702F7, Hook::UpdateOffscreenInterface);
			kSettingIntDetour.ReplaceCall(0x8743B0, &Hook::CanResume3DTasks);

			if (Settings::bScreenBloodInPipBoy) {
				HookUtils::PatchMemoryNop(0x87110B, 2);
				HookUtils::PatchMemoryNop(0x7F813B, 13); // Don't set BSShaderBloodSplatter's alpha to 0 on FOPipboyManager open
				kRenderScreenSplatterDetour.ReplaceCall(0x870994, &Hook::DrawImageSpaceAndScrenSplatter);
			}
		}

	}

}

#pragma endregion