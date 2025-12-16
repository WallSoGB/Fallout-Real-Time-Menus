#include "RealTimeMenus.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Scripting.hpp"
#include "RTM-Rendering.hpp"
#include "RTM-Container.hpp"
#include "RTM-Terminal.hpp"
#include "RTM-PipBoy.hpp"
#include "RTM-Dialogue.hpp"
#include "RTM-VATS.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/ActiveEffect.hpp"
#include "Bethesda/BSAudio.hpp"
#include "Bethesda/BSShaderManager.hpp"
#include "Bethesda/EffectItem.hpp"
#include "Bethesda/EffectSetting.hpp"
#include "Bethesda/GameSettingCollection.hpp"
#include "Bethesda/Interface.hpp"
#include "Bethesda/InterfaceManager.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/ShadowSceneNode.hpp"
#include "Bethesda/TES.hpp"
#include "Bethesda/TimeGlobal.hpp"
#include "Bethesda/DialogMenu.hpp"

#include "Gamebryo/NiControllerSequence.hpp"

#include "shared/NVSEManager/NVSEGlobalManager.hpp"

#pragma region Code

namespace RealTimeMenus {

	constexpr uint32_t EFFECT_FLAG_PAUSED_IN_MENU = 1 << 30;

	CallDetour kInterfacePreIdle;
	CallDetour kLoadBlockedScriptsDetour;

	bool bOurInMenuMode = false;
	bool bBlockingMenuMode = false;

	bool bSkipAnimThisFrame = false;
	bool bWasLastFrameInMenuMode = false;

	namespace {
		SPEC_NOINLINE TESForm* IsTriggerDisallowedForForm(TESForm* apForm) {
			if (apForm && apForm->IsActor() && Interface::IsInDialog()) {
				Actor* pActor = static_cast<Actor*>(apForm);
				if (pActor == PlayerCharacter::GetSingleton() || pActor->IsInDialoguewithPlayer())
					return nullptr;
			}
			return apForm;
		}

		SPEC_NOINLINE void SetActivatedRefrence() {
			TESObjectREFR* pTarget = Interface::GetActivateTarget();
			if (pTarget && pTarget->GetObjectReference())
				RealTimeMenus::Utils::pActivatedReference = pTarget;
		}
	}

	template<uint32_t uiAddress>
	class Hook_FakeMenuMode {
		static inline CallDetour kDetour;
		static bool IsMenuMode() {
			const uint32_t uiOverwrittenAddr = kDetour.GetOverwrittenAddr();
			if (uiOverwrittenAddr != 0x702360)
				CdeclCall(uiOverwrittenAddr);

			[[msvc::noinline_calls]]
			return RealTimeMenus::Utils::IsMenuPausingGame();
		}
	public:
		Hook_FakeMenuMode() {
			kDetour.ReplaceCall(uiAddress, IsMenuMode);
		};
	};

	template<uint32_t uiAddress>
	class Hook_CanSendOnTriggerEvents {
		static inline CallDetour kDetour;
		static TESForm* CanSendEvent(void* apArg) {
			TESForm* pForm = CdeclCall<TESForm*>(kDetour.GetOverwrittenAddr(), apArg);
			return IsTriggerDisallowedForForm(pForm);
		}
	public:
		Hook_CanSendOnTriggerEvents() {
			kDetour.ReplaceCall(uiAddress, CanSendEvent);
		};
	};

	template<uint32_t uiAddress>
	class Hook_TrackActivatedReference {
		static inline CallDetour kDetour;
		bool GrabActivatedRef() {
			bool bNoActivate = ThisCall<bool>(kDetour.GetOverwrittenAddr(), this); // PlayerCharacter::GetPreventActivate
			if (!bNoActivate) {
				SetActivatedRefrence();
			}
			return bNoActivate;
		}
	public:
		Hook_TrackActivatedReference() {
			kDetour.ReplaceCallEx(uiAddress, &Hook_TrackActivatedReference::GrabActivatedRef);
		};
	};

	class Hook {
	public:
		void WaitSkyUpdate(float afDelta) {
			ThisCall(0x63AC70, this, afDelta); // Sky::Update
			TES::GetSingleton()->UpdateCellAnimations(afDelta);
			NiUpdateData kData(afDelta, true, false);
			ThisCall(0xC50610, CdeclCall<void*>(0x45A190), &kData); // BSParticleSystemManager::UpdateAll
		}

		void UpdatePlayer() {
			PlayerCharacter* pThis = reinterpret_cast<PlayerCharacter*>(this);
			pThis->ForceGrenadeHold();
			if (RealTimeMenus::Utils::IsMenuPausingGame()) [[unlikely]] {
				return;
			}

			Utils::AutoPlayerCall kAutoPlayerCall;

			const bool bInDialog = Interface::IsInDialog();
			const bool bInRenderedMenu = Interface::GetCurrentRenderedMenu() != nullptr;
			const float fDelta = TimeGlobal::GetSingleton()->fDelta;

			BaseProcess* pProcess = pThis->GetCurrentProcess();

			if (pThis->bIsHardcore) {
				if (bInDialog || Settings::bNoHardcoreInMenus)
					pThis->bSkipHCNeedsUpdate = true;
				else
					pThis->UpdateHardcoreMode();
			}

			if (bSkipAnimThisFrame) [[unlikely]] {
				pThis->pActorMover->ClearMovementFlag(ActorMover::MovementFlags::INPUT_MASK);
				pThis->SetWalkRunFlagDirty();
				bSkipAnimThisFrame = false;
			}

			if (!bInRenderedMenu) [[likely]]
				pProcess->Update3DModel(pThis);

			pProcess->CalculateLightValue(pThis, false);

			if (!bInDialog) {
				pThis->UpdateGreetTimer();
				pThis->HandleResetGreetTimer();
			}

			// Update when not in dialog
			// Dialog updates player in its own code
			const bool bInDialogServiceMenu = DialogMenu::GetSingleton() && DialogMenu::GetSingleton()->bInBackground;
			if (!bInDialog || bInDialogServiceMenu) {
				Utils::UpdatePlayerMovement(pThis, fDelta, !bInRenderedMenu, true, true);
			}

			if (!bInDialog)
				pThis->UpdateCamera(false, true);

			pThis->HandlePostAnimationActions();

			BSShaderManager::GetShadowSceneNode(0)->UpdateObjectLighting(static_cast<Actor*>(pThis)->Get3D(), true);

			if (!pThis->IsDead(false) && !pThis->IsDying()) [[likely]] {
				pThis->UpdateMagic(false, false);
			}

			pThis->UpdatePlayerCombat(fDelta);

			if (!bInDialog) [[likely]] {
				if (pThis->fLastHelloTime > 0.f) {
					if (GameSettingCollection::fHelloCooldownTime->Float() * 1000.f < GetTickCount() - pThis->fLastHelloTime)
						pThis->fLastHelloTime = -1.f;
				}
			}

			if (!pThis->IsTrespassing()) {
				ExtraDataList* pExtraData = pThis->GetExtraDataList();
				if (ThisCall<void*>(0x41CD70, pExtraData)) // ExtraDataList::GetTrespassPackage
					ThisCall(0x41CDA0, pExtraData); // ExtraDataList::RemoveTrespassPackage
			}

			RealTimeMenus::Utils::ShouldCloseMenus(pThis);
		}

		void UpdateActiveEffect(float afDelta) {
			ActiveEffect* pThis = reinterpret_cast<ActiveEffect*>(this);
			if (Interface::IsInMenuMode() && pThis->pTarget && pThis->pTarget->GetTargetAsActor() == PlayerCharacter::GetSingleton()) {
				if (pThis->pEffect && pThis->pEffect->pEffectSetting) {
					bool bNoMenuMode = pThis->pEffect->pEffectSetting->uiFormFlags.Get(EFFECT_FLAG_PAUSED_IN_MENU);
					if (bNoMenuMode)
						return;
				}
			}

			ThisCall(0x804560, this, afDelta); // ActiveEffect::UpdateEffect
		}

		void InterfaceManager_PreIdle() {
			InterfaceManager* pThis = reinterpret_cast<InterfaceManager*>(this);
			if (pThis->uiMenuMode == InterfaceManager::MENU_MODE_ENTERING && !pThis->bLockMenuModeForFade) {
				if (RealTimeMenus::Utils::IsMenuPausingGame() && !pThis->bHasMutedSounds) {
					BSAudio* pAudio = BSAudio::GetSingleton();

					pThis->ucMuteAnimationSounds++;
					pAudio->PauseAnimationSounds();

					pThis->ucMuteWorldSounds++;
					pAudio->PauseWorldVoices();

					pThis->bHasMutedSounds = true;
				}
			}

			ThisCall(kInterfacePreIdle.GetOverwrittenAddr(), this);

			if (pThis->uiMenuMode == InterfaceManager::MENU_MODE_OFF || !RealTimeMenus::Utils::IsMenuPausingGame()) {
				if (pThis->bHasMutedSounds) {
					BSAudio* pAudio = BSAudio::GetSingleton();

					if (pThis->ucMuteAnimationSounds) {
						pThis->ucMuteAnimationSounds--;
						pAudio->UnpauseAnimationSounds();
					}

					if (pThis->ucMuteWorldSounds) {
						pThis->ucMuteWorldSounds--;
						pAudio->UnpauseWorldVoices();
					}

					pThis->bHasMutedSounds = false;
				}
			}
		}

		static int32_t __fastcall ShouldDeactivateSequence(const TESObjectREFR* apReference, NiControllerSequence* apSequence) {
			if (apReference == Utils::pActivatedReference) {
				const TESBoundObject* pBase = apReference->GetObjectReference();
				const char* pSequenceName = apSequence->GetName();
				if (pBase && pSequenceName && strcmp(pSequenceName, "Open") == 0) {
					auto eType = pBase->GetFormType();
					switch (eType) {
						case FORM_TYPE::TESObjectCONT:
							if (!Settings::bPauseContainers)
								return 0;
							break;
						case FORM_TYPE::BGSTerminal:
							if (!Settings::bPauseComputerMenu)
								return 0;
							break;
						default:
							break;
					}
				}
			}

			return apSequence->Deactivate(0.f, false);
		}

		static void* InitBlockedScripts() {
			RealTimeMenus::Scripting::LoadBlockFiles();
			return CdeclCall<void*>(kLoadBlockedScriptsDetour.GetOverwrittenAddr());
		}
	};

	__declspec(naked) void DeactivateSequenceHook_Asm() {
		static constexpr uint32_t uiReturnAddr = 0x567199;
		__asm {
			mov		edx, [ebp - 0x1C]
			mov		ecx, [ebp - 0x28]
			call	Hook::ShouldDeactivateSequence
			jmp		uiReturnAddr
		}
	}
}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	void ReadSettings() {
		RealTimeMenus::Settings::ReadSettings();
	}

	void ReplaceCommands() {
		RealTimeMenus::Scripting::ReplaceCommands();
	}

	void InitHooks() {
		kLoadBlockedScriptsDetour.ReplaceCall(0x86B0ED, &Hook::InitBlockedScripts);

		RealTimeMenus::Rendering::InitHooks();
		RealTimeMenus::Container::InitHooks();
		RealTimeMenus::Terminal::InitHooks();
		RealTimeMenus::PipBoy::InitHooks();
		RealTimeMenus::Dialogue::InitHooks();
		RealTimeMenus::VATS::InitHooks();
	}

	void InitDeferredHooks() {
		ReplaceCallEx(0x86F97C, &Hook::UpdatePlayer);

		// Don't trigger Havok events for player in menu mode
		Hook_CanSendOnTriggerEvents<0x62CE7E>();
		Hook_CanSendOnTriggerEvents<0x62D09F>();
		Hook_CanSendOnTriggerEvents<0x62D244>();

		// Add a flag to disallow effect update in MenuMode
		ReplaceCallEx(0x823DE6, &Hook::UpdateActiveEffect);

		// Track activated object so we can close their menus if player gets too far
		Hook_TrackActivatedReference<0x94063B>();
		Hook_TrackActivatedReference<0x943150>();

		// Visual patches
		SafeWrite32(0x86E771 + 3, uint32_t(&bOurInMenuMode)); // Update imagespace
		SafeWriteBuf(0x94E141, "\xD9\x45\xF8\x90\x90\x90", 6); // Make sky update with correct delta in sleep/wait
		ReplaceCallEx(0x94E157, &Hook::WaitSkyUpdate); // Run animations and particles in sleep/wait

		Hook_FakeMenuMode<0x63E0F8>(); // SkySound::Play
		Hook_FakeMenuMode<0x837D05>(); // ImpactMixer::PlayFootstep
		Hook_FakeMenuMode<0x8387C5>(); // ImpactMixer::PlayJumpSound
		Hook_FakeMenuMode<0x86A4A1>(); // BSShaderManager::CalculateTime
		Hook_FakeMenuMode<0x86A4DE>(); // BSShaderManager::CalculateTime
		Hook_FakeMenuMode<0x86E79E>(); // TESMain::OnIdle
		Hook_FakeMenuMode<0x86EEAF>(); // TESMain::OnIdle
		Hook_FakeMenuMode<0x8898BD>(); // Actor::Update
		
		if (Settings::bControllerRumbleInMenus) {
			Hook_FakeMenuMode<0x89C6CA>(); // Actor::HitMe
			Hook_FakeMenuMode<0x89C74A>(); // Actor::HitMe
			Hook_FakeMenuMode<0x89CEBA>(); // Actor::HitMe
			Hook_FakeMenuMode<0x89CF56>(); // Actor::HitMe
			Hook_FakeMenuMode<0x8A7EA9>(); // Actor::TriggerPain
			Hook_FakeMenuMode<0x8D1AA1>(); // SetExplosionCameraShake
		}

		// Handle audio muting in menu mode
		kInterfacePreIdle.ReplaceCallEx(0x702806, &Hook::InterfaceManager_PreIdle);
		WriteRelJump(0x70BA37, 0x70BA94);
		WriteRelJump(0x70BAA1, 0x70BB7E);

		// Don't deactivate the "Open" anim sequences for activated references, because it'd reset soon after opening the menu (they expect menu to pause the game)
		if (!Settings::bPauseContainers || !Settings::bPauseComputerMenu)
			WriteRelJump(0x567187, DeactivateSequenceHook_Asm);

		if (Settings::bShowHandsInVATS)
			PatchMemoryNop(0x870C99, 2);

		if (Settings::bShowHandsInDialogue)
			PatchMemoryNop(0x870CA5, 2);
	}

	void HandleNVR() {
		const PluginInfo* pNVR = NVSEGlobalManager::GetSingleton().GetCmdInterface()->GetPluginInfoByName("NewVegasReloaded");
		if (pNVR && pNVR->version < 440) {
			MessageBoxA(nullptr, "Real Time Menus detected an outdated version of New Vegas Reloaded.\nPlease update to at least version 4.4.0.\nYou can find latest versions in the #nightly-builds channel.", "Real Time Menus", MB_OK | MB_ICONERROR);
			ExitProcess(0);
		}
	}

	static void MessageHandler(NVSEMessagingInterface::Message* apMessage) {
		switch (apMessage->type) {
			case NVSEMessagingInterface::kMessage_PostPostLoad:
			{
				RealTimeMenus::HandleNVR();
				RealTimeMenus::ReplaceCommands();
				break;
			}
			case NVSEMessagingInterface::kMessage_DeferredInit:
			{
				InitDeferredHooks();

				RealTimeMenus::Rendering::InitDeferredHooks();
				RealTimeMenus::Container::InitDeferredHooks();
				RealTimeMenus::Terminal::InitDeferredHooks();
				RealTimeMenus::PipBoy::InitDeferredHooks();
				RealTimeMenus::Dialogue::InitDeferredHooks();
				RealTimeMenus::VATS::InitDeferredHooks();

				RealTimeMenus::Scripting::WaitForBlockFiles();
				break;
			}
			case NVSEMessagingInterface::kMessage_MainGameLoop:
			{
				RealTimeMenus::Rendering::bCanDoAsyncUI = false;
				RealTimeMenus::Rendering::bUIUpdated = false;
				const bool bIsPipBoyOpening = Interface::IsPipBoyOpening();
				const bool bIsInMenu = Interface::IsInMenuMode() || bIsPipBoyOpening;
				if (bIsInMenu != bWasLastFrameInMenuMode) {
					if (bIsInMenu)
						bSkipAnimThisFrame = true;
					bWasLastFrameInMenuMode = bIsInMenu;
				}

				bBlockingMenuMode = RealTimeMenus::Utils::IsMenuPausingGame();
				bOurInMenuMode = bBlockingMenuMode || bIsPipBoyOpening;

				break;
			}
			case NVSEMessagingInterface::kMessage_PostLoadGame:
			{
				const bool bSaveLoaded = static_cast<bool>(apMessage->data);
				if (bSaveLoaded) {
					RealTimeMenus::Utils::pActivatedReference = nullptr;
				}
				break;
			}
			default:
				break;
		}
	}

	void RegisterMessageHandler(NVSEGlobalManager& arNVSEMgr) {
		arNVSEMgr.RegisterPluginEventListener("NVSE", MessageHandler);
	}
}

#pragma endregion