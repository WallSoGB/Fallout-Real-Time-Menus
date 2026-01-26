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

		CallDetour kRenderScreenSplatterDetour;
		CallDetour kClearScreenSplatterDetour;

		bool bCanDoAsyncUI = false;
		bool bUIUpdated = false;
		bool bCanClearScreenSplatter = true;

		Utils::ImageSpaceStage kImageSpaceStage = Utils::IS_NONE;

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

		template<uint32_t uiAddress, bool bMainThread>
		class Hook_InterfaceUpdate {
			static inline CallDetour kDetour;
			void InterfaceUpdateMT() {
				if (bCanDoAsyncUI && !bUIUpdated) {
					ThisCall(kDetour.GetOverwrittenAddr(), this);
				}
			}

			void InterfaceUpdateST() {
				if (!bCanDoAsyncUI && !bUIUpdated) {
					ThisCall(kDetour.GetOverwrittenAddr(), this);
				}
			}

		public:
			Hook_InterfaceUpdate() {
				if constexpr (bMainThread) {
					kDetour.ReplaceCallEx(uiAddress, &Hook_InterfaceUpdate::InterfaceUpdateST);
				}
				else {
					kDetour.ReplaceCallEx(uiAddress, &Hook_InterfaceUpdate::InterfaceUpdateMT);
				}
			}
		};

		template<uint32_t uiAddress>
		class Hook_BlurredDrawWorld {
		private:
			static inline CallDetour kDetour;
			void DrawWorld(void* apTexture, bool abRenderedMenu, bool abPipboyMode) {
				kImageSpaceStage = Utils::IS_FG;

				if (abRenderedMenu) {
					if (ShouldUseNormalRenderPath()) {
						ThisCall(kDetour.GetOverwrittenAddr(), this, apTexture, abRenderedMenu, abPipboyMode);
						kImageSpaceStage = Utils::IS_NONE;
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

					kImageSpaceStage = Utils::IS_BG;

					bBlur = true;

					bEOF = ImageSpaceManager::bEOFEnabled;
					ImageSpaceManager::bEOFEnabled = true;

					pISMod = reinterpret_cast<TESImageSpaceModifier*>(Utils::GetCurrentMenuBackgroundFX());

					if (pISMod) {
						ImageSpaceModifierInstanceForm::Trigger(pISMod, 1.f, nullptr);
						reinterpret_cast<TESMain*>(this)->UpdateImageSpace(false);
					}
				}

				ThisCall(kDetour.GetOverwrittenAddr(), this, apTexture, abRenderedMenu, abPipboyMode);

				if (bBlur) {
					if (pISMod) {
						ImageSpaceModifierInstanceForm::Stop(pISMod);
						reinterpret_cast<TESMain*>(this)->UpdateImageSpace(false);
					}

					ImageSpaceManager::bEOFEnabled = bEOF;
				}

				kImageSpaceStage = Utils::IS_NONE;
			}
		public:
			Hook_BlurredDrawWorld() {
				kDetour.ReplaceCallEx(uiAddress, &Hook_BlurredDrawWorld::DrawWorld);
			}
		};

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

				ThisCall(0x86F450, this);
			}

			void DrawWorld_Standard_Ex(BSRenderedTexture* apMainTarget, BSRenderedTexture* apCustomRenderTarget) {
				TESMain* pThis = reinterpret_cast<TESMain*>(this);
				NiDX9Renderer* pRenderer = NiDX9Renderer::GetSingleton();
				ImageSpaceManager* pISMgr = ImageSpaceManager::GetSingleton();
				bool bIsMSAA = CdeclCall<bool>(0x4DC310);

				if (ShouldUseNormalRenderPath()) {
					pThis->DrawWorld_CopyMenuBG(pISMgr, pRenderer, bIsMSAA, apCustomRenderTarget);
					return;
				}

				kImageSpaceStage = Utils::IS_BG;

				pThis->DrawWorld_Init();
				pThis->DrawWorld_UpdateWater();
				BSRenderedTexture* pCurrentRT = pThis->DrawWorld_InitRenderTexture(pRenderer, apCustomRenderTarget, false, bIsMSAA);
				TESImageSpaceModifier* pISMod = reinterpret_cast<TESImageSpaceModifier*>(Utils::GetCurrentMenuBackgroundFX());

				bool bEOF = ImageSpaceManager::bEOFEnabled;

				if (pISMod) {
					ImageSpaceModifierInstanceForm::Trigger(pISMod, 1.f, nullptr);
					reinterpret_cast<TESMain*>(this)->UpdateImageSpace(false);
					ImageSpaceManager::bEOFEnabled = true;
				}

				pThis->DrawWorld_DrawScene(nullptr, false, false, false);
				if (BSShaderManager::IsRefractionEnabled()) {
					if (ThisCall(0xB63A90, pThis->spWorldAccum.m_pObject)) { // BSShaderAccumulator::GetRefractivePassCount
						pThis->DrawWorld_Refraction(pThis->spWorldAccum.m_pObject, pCurrentRT);
					}
				}

				ImageSpaceEffectDepthOfField* pDOF = static_cast<ImageSpaceEffectDepthOfField*>(pISMgr->GetEffect(ImageSpaceManager::IS_EFFECT_DEPTH_OF_FIELD));
				if (pDOF && pDOF->DoDepth())
					pThis->DrawWorld_DepthOfField(pThis->spWorldAccum.m_pObject, pCurrentRT);

				if (!BSRenderedTexture::IsOutsideFrame())
					BSRenderedTexture::StopOffscreen();

				BSShaderManager::RenderImageSpaceEffects(pRenderer, pCurrentRT, apMainTarget);

				if (pISMod) {
					ImageSpaceModifierInstanceForm::Stop(pISMod);
					reinterpret_cast<TESMain*>(this)->UpdateImageSpace(false);
					ImageSpaceManager::bEOFEnabled = bEOF;
				}

				BSShaderManager::GetTextureManager()->ReturnRenderedTexture(pCurrentRT);

				pThis->DrawWorld_RestartRenderTexture(apMainTarget, bIsMSAA, NiRenderer::CLEAR_ZBUFFER);
        
        kImageSpaceStage = Utils::IS_NONE;
			}

			void DrawImageSpaceAndScrenSplatter(BOOL abMSAA, class BSRenderedTexture* apRenderTarget, class NiDX9Renderer* apRenderer, class BSRenderedTexture* apDestination) {
				ThisCall(kRenderScreenSplatterDetour.GetOverwrittenAddr(), this, abMSAA, apRenderTarget, apRenderer, apDestination);
				if (ScreenCustomSplatter::IsActiveSplatter() || ScreenSplatter::IsActiveSplatter())
					reinterpret_cast<TESMain*>(this)->DrawWorld_ScreenSplatter(apRenderer);
			}

			static void UpdateOffscreenBuffers() {
				if (Interface::IsMenuIDVisible(Interface::Menus::LockPick, 0))
					TESMain::GetSingleton()->DrawWorld_UpdateOffscreenBuffers();
				else
					TESMain::DrawWorld_RenderedMenuInterface();
			}

			static void __cdecl SetDefer3D(bool abDefer) {
				bCanDoAsyncUI = CanMultithreadUI();
				if (!bCanDoAsyncUI) {
					TESMain::GetSingleton()->OnIdle_DoInterfaceIdle();
					bUIUpdated = true;
				}

				CdeclCall(0x8C80E0, true);
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

			ReplaceCall(0x87212D, &Utils::GetCurrentMenuBackgroundFX);

			ReplaceCallEx(0x86E8FD, &Hook::HandleMenuStaticBackground);

			if (!Settings::IsMenuPaused(Interface::PipBoy)) {
				SafeWriteBuf(0x870009, "\xC6\x05\x29\xEA\x1D\x01\x01\x90\x90\x90\x90", 11); // Set bMenuBGReady=1 instead of rendering it
			}

			if (!Settings::IsMenuPaused(Interface::PipBoy) || !Settings::IsMenuPaused(Interface::Computers)) {
				PatchMemoryNopRange(0x870876, 0x870884);
				SafeWriteBuf(0x87087C, "\x90\x90\x90\x90\x90\x90\x52\x50", 8);
				ReplaceCallEx(0x870887, &Hook::DrawWorld_Standard_Ex); // Call TESMain::DrawWorld_DrawScene in Rendered Menu render, instead of TESMain::DrawWorld_CopyMenuBG
			}

			PatchMemoryNopRange(0x872983, 0x872994);
			SafeWriteBuf(0x872983, "\x90\x90\x90\x8B\x55\x08\x52\x50", 8);
			ReplaceCallEx(0x872997, &Hook::DrawWorld_Standard_Ex); // Call TESMain::DrawWorld_DrawScene in 3D Menu render, instead of TESMain::DrawWorld_CopyMenuBG

			// Switch UI update from AI thread to main thread properly
			Hook_InterfaceUpdate<0x8C7BE9, false>();	// Task 0
			Hook_InterfaceUpdate<0x8C7DB9, false>();	// Task 1
			Hook_InterfaceUpdate<0x86ECBA, true>();	// Main thread

			ReplaceCall(0x86EC78, Hook::SetDefer3D);
			ReplaceCall(0x8702F7, Hook::UpdateOffscreenBuffers);

			if (Settings::bScreenBloodInPipBoy) {
				PatchMemoryNop(0x87110B, 2);
				PatchMemoryNop(0x7F813B, 13); // Don't set BSShaderBloodSplatter's alpha to 0 on FOPipboyManager open
				kRenderScreenSplatterDetour.ReplaceCallEx(0x870994, &Hook::DrawImageSpaceAndScrenSplatter);
			}
		}

	}

}

#pragma endregion