#include "RTM-Dialogue.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/BSGameSound.hpp"
#include "Bethesda/CombatController.hpp"
#include "Bethesda/Controls.hpp"
#include "Bethesda/DialogMenu.hpp"
#include "Bethesda/DialoguePackage.hpp"
#include "Bethesda/HighProcess.hpp"
#include "Bethesda/INIPrefSettingCollection.hpp"
#include "Bethesda/Interface.hpp"
#include "Bethesda/InterfaceManager.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/TESMain.hpp"
#include "Bethesda/TimeGlobal.hpp"

#include <directxmath.h>

#pragma region Code

namespace RealTimeMenus {

	namespace Dialogue {

		static SPEC_NOINLINE bool CanActorBeIgnored(Actor* apActor) {
			if (apActor == PlayerCharacter::GetSingleton() && Interface::IsInMenuMode())
				return true;

			if (apActor->IsInDialoguewithPlayer())
				return true;

			return false;
		}

		static SPEC_NOINLINE bool __fastcall IsActorExcludedFromDialogueCombat(Actor* apActor, bool abCheckFaction) {
			if (apActor->IsInDialoguewithPlayer())
				return true;

			if (Interface::IsInDialog()) {
				PlayerCharacter* pPlayer = PlayerCharacter::GetSingleton();
				if (apActor == pPlayer)
					return true;

				if (apActor->IsPlayerTeammate())
					return true;

				if (pPlayer->IsPlayerFollower(apActor))
					return true;

				if (abCheckFaction) {
					bool bPlayerEnemy = false;
					if (pPlayer->GetFactionRelation(apActor, bPlayerEnemy) >= FACTION_RELATION_ALLY && !bPlayerEnemy)
						return true;
				}
			}
			return false;
		}

		static SPEC_NOINLINE void __fastcall HandleDialogueLookAround(PlayerCharacter* apPlayer) {
			const float fDefaultFOV = INISettingCollection::Display::fDefaultWorldFOV->Float();
			const float fCameraFOV = TESMain::GetWorldSceneGraph()->fCurrentFOV;
			float fFOVSensitivity = 1.f;
			if (fDefaultFOV > fCameraFOV)
				fFOVSensitivity = fCameraFOV / fDefaultFOV;

			const float fSensitivity = INIPrefSettingCollection::Controls::fMouseSensitivity->Float() * fFOVSensitivity;

			InterfaceManager* pUIMgr = InterfaceManager::GetSingleton();
			const float& rTimeDelta = TimeGlobal::GetSingleton()->fRealTimeDelta;

			if (pUIMgr->IsControllerConnected()) {
				int32_t iDeltaX = Controls::GetSingleton()->GetControllerValue(0, 9);
				int32_t iDeltaY = Controls::GetSingleton()->GetControllerValue(0, 10);
				if (abs(iDeltaX) < 8689)
					iDeltaX = 0;
				if (abs(iDeltaY) < 8689)
					iDeltaY = 0;
				if (iDeltaX || iDeltaY) {
					bool bNegX = false;
					if (iDeltaX < 0) {
						iDeltaX = -iDeltaX;
						bNegX = true;
					}
					bool bNegY = true;
					if (iDeltaY < 0) {
						iDeltaY = -iDeltaY;
						bNegY = false;
					}
					NiPoint3 v94;
					v94.x = iDeltaX / 32767.f;
					v94.y = iDeltaY / 32767.f;
					v94.z = 0.;
					float v92 = v94.Length();;
					if (v92 > 1.f)
						v92 = 1.f;

					float v90 = v94.x / v92;
					float v93 = v94.y / v92;
					float v95 = 1.f;
					float v87 = 1.f;

					for (int32_t i = 0; i < INISettingCollection::Controls::iXenonHorizLookAccel->Int(); ++i)
						v95 = v95 * v92;
					for (int32_t j = 0; j < INISettingCollection::Controls::iXenonVertLookAccel->Int(); ++j)
						v87 = v87 * v92;

					const float fXenonMinLookSpeed = INISettingCollection::Controls::fXenonMinLookSpeed->Float();
					if (fXenonMinLookSpeed > v95)
						v95 = fXenonMinLookSpeed;
					if (fXenonMinLookSpeed > v87)
						v87 = fXenonMinLookSpeed;

					iDeltaX = (v95 * v90 * INIPrefSettingCollection::Controls::fXenonHorizLookSpeed->Float() * rTimeDelta);
					iDeltaY = (v87 * v93 * INIPrefSettingCollection::Controls::fXenonVertLookSpeed->Float() * rTimeDelta);

					if (bNegX)
						iDeltaX = -iDeltaX;

					if (bNegY)
						iDeltaY = -iDeltaY;

					float fHeading = iDeltaX * fSensitivity;
					float fLooking = iDeltaY * fSensitivity;
					if (INIPrefSettingCollection::Controls::bInvertYValues->Bool())
						fLooking = -fLooking;

					apPlayer->ModHeading(fHeading);
					apPlayer->ModLooking(fLooking);
				}
			}
			else if (pUIMgr->bMouseVisible) {
				constexpr float fLookSpeed = 400.f;
				constexpr float fUITriggerZone = 25.f;

				const NiPoint3& rCursorPos = pUIMgr->kCursorPos;
				const float fScreenWidth = InterfaceManager::GetScreenWidth();
				const float fScreenHeight = InterfaceManager::GetScreenHeight();
				const int32_t iCursorX = ThisCall<int32_t>(0x747D00, pUIMgr);
				const int32_t iCursorY = ThisCall<int32_t>(0x73B020, pUIMgr);

				const float fCursorMinX = 0.f;
				const float fCursorMaxX = fScreenWidth;

				const float fCursorMinY = 0.f;
				const float fCursorMaxY = fScreenHeight;

				bool bInZone = false;
				if (fCursorMaxX - fUITriggerZone < iCursorX)
					bInZone = true;

				if (fCursorMinX + fUITriggerZone > iCursorX)
					bInZone = true;

				if (fCursorMaxY - fUITriggerZone < iCursorY)
					bInZone = true;

				if (fCursorMinY + fUITriggerZone > iCursorY)
					bInZone = true;

				if (bInZone && !pUIMgr->pMouseOverTarget) {
					const float fCursorX = rCursorPos.x * 2.f;
					const float fCursorY = rCursorPos.z * 2.f;

					const float fNormX = fCursorX / fScreenWidth;
					const float fNormY = fCursorY / fScreenHeight;

					float fHeading = DirectX::XMConvertToRadians(fNormX * fSensitivity * rTimeDelta * fLookSpeed * 24.f);
					float fLooking = DirectX::XMConvertToRadians(fNormY * fSensitivity * rTimeDelta * fLookSpeed * 24.f);
					if (!INIPrefSettingCollection::Controls::bInvertYValues->Bool())
						fLooking = -fLooking;

					apPlayer->ModHeading(fHeading);
					apPlayer->ModLooking(fLooking);
				}
			}
		}

		static SPEC_NOINLINE void __fastcall UpdateDialogueCamera(PlayerCharacter* apPlayer, Actor* apTarget, float afZoomPercent, bool abOnlyZoom, uint32_t auiCallAddress) {
			DialogMenu* pDialogMenu = DialogMenu::GetSingleton();
			if (!pDialogMenu)
				return;

			if (pDialogMenu->bInBackground)
				return;

			const float fTimeDelta = TimeGlobal::GetSingleton()->fDelta;
			if (Settings::bLookAroundInDialogue && pDialogMenu->eStage != DialogMenu::CAMERA_ZOOM_IN) {
				if (PlayerCharacter::fDialogZoomPercent < 1.f) {
					ThisCall(auiCallAddress, apPlayer, apTarget, afZoomPercent, abOnlyZoom);
				}
				else {
					HandleDialogueLookAround(apPlayer);
					CdeclCall(0xC66210); // bhkWorld::ClearPickData
				}
			}
			else {
				ThisCall(auiCallAddress, apPlayer, apTarget, afZoomPercent, abOnlyZoom);
			}
			Utils::AutoPlayerCall kAutoPlayerCall;
			Utils::UpdatePlayerMovement(apPlayer, fTimeDelta, true, true, false);
		}

		template<uint32_t uiAddress>
		class Hook_CanBeActivated {
			static inline HookUtils::CallDetour kDetour;

			BaseProcess* Hook() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (CanActorBeIgnored(pActor))
					return nullptr;

				return ThisCall<BaseProcess*>(kDetour, pActor);
			}

		public:
			Hook_CanBeActivated() {
				kDetour.ReplaceCall(uiAddress, &Hook_CanBeActivated::Hook);
			}
		};

		template<uint32_t uiAddress, bool bPlayer>
		class Hook_IsInvulnerable {
			static inline HookUtils::VirtFuncDetour kDetour;

			bool Hook() {
				MagicTarget* pThis = reinterpret_cast<MagicTarget*>(this);
				Actor* pActor = static_cast<Actor*>(pThis);
				if constexpr (bPlayer) {
					if (Interface::IsInDialog())
						return true;
				}
				else {
					if (pActor->IsInDialoguewithPlayer())
						return true;
				}
				return ThisCall<bool>(kDetour, this);
			}

		public:
			Hook_IsInvulnerable() {
				kDetour.ReplaceVirtualFunc(uiAddress, &Hook_IsInvulnerable::Hook);
			};
		};

		template<uint32_t uiAddress>
		class Hook_FakeInDialogue {
			static inline HookUtils::CallDetour kDetour;

			static bool Hook() {
				const uint32_t uiOverwrittenAddr = kDetour;
				if (uiOverwrittenAddr != 0x7050D0)
					CdeclCall(uiOverwrittenAddr);
				return false;
			}

		public:
			Hook_FakeInDialogue() {
				kDetour.ReplaceCall(uiAddress, Hook_FakeInDialogue::Hook);
			};
		};

		template<uint32_t uiAddress>
		class Hook_DialogueCamera {
			static inline HookUtils::CallDetour kDetour;

			void Hook(Actor* apTarget, float afZoomPercent, bool abOnlyZoom) {
				PlayerCharacter* pPlayer = reinterpret_cast<PlayerCharacter*>(this);
				UpdateDialogueCamera(pPlayer, apTarget, afZoomPercent, abOnlyZoom, kDetour);
			}

		public:
			Hook_DialogueCamera() {
				kDetour.ReplaceCall(uiAddress, &Hook_DialogueCamera::Hook);
			};
		};

		template<uint32_t uiAddress>
		class Hook_CanUpdateActor {
			static inline HookUtils::VirtCallDetour kDetour;
		
			bool Hook() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (kDetour) {
					if (!ThisCall<bool>(kDetour, pActor))
						return false;
				}
				else if (!pActor->IsActor())
					return false;

				return pActor->IsInDialoguewithPlayer() == false;
			}

		public:
			Hook_CanUpdateActor() {
				kDetour.ReplaceVirtualCall(uiAddress, &Hook_CanUpdateActor::Hook, 8);
			}
		};

		template<uint32_t uiAddress>
		class Hook_SetVoiceVolume {
			static inline HookUtils::VirtCallDetour kDetour;
		
			void Hook(float afVolume) {
				BSGameSound* pThis = reinterpret_cast<BSGameSound*>(this);
				if (pThis->uiTypeFlags.bUnkBit10)
					pThis->usFaderAttenuation = 0;
				else if (pThis->uiTypeFlags.bVoice)
					pThis->usFaderAttenuation = static_cast<uint16_t>(Settings::fDialogueVoiceFadeDecibels * 100.f);

				if (kDetour)
					ThisCall(kDetour, pThis, afVolume);
				else
					pThis->SetVolume(afVolume);
			}

		public:
			Hook_SetVoiceVolume() {
				kDetour.ReplaceVirtualCall(uiAddress, &Hook_SetVoiceVolume::Hook, 5);
			}
		};

		HookUtils::CallDetour kCanSkipActorUpdateDetour;
		HookUtils::CallDetour kSetSkippedLastUpdateDetour;
		HookUtils::CallDetour kCloseDialogueIfDeadDetour;
		HookUtils::CallDetour kIsInCombatDetour;
		HookUtils::VirtCallDetour kGetRunOncePackageDetour;
		HookUtils::CallDetour kCanSpeakDetour;
		HookUtils::CallDetour kGetIgnoredBySandboxDetour;
		HookUtils::CallDetour kGetBestDefaultHeadTrackTargetDetour;
		HookUtils::CallDetour kShouldStopCombatDetour;
		HookUtils::CallDetour kOnDialogueStartDetour;
		HookUtils::CallDetour kOnDialogueEndDetour;
		HookUtils::CallDetour kIsInDialoguewithPlayerDetour;
		HookUtils::CallDetour kGetSoundHandleByFilenameDetour;
		class Hook {
		public:
			BaseProcess* GetCurrentProcess_CanActorSpeak() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (pActor->IsDead(false))
					return nullptr;

				return ThisCall<BaseProcess*>(kCloseDialogueIfDeadDetour, pActor);
			}

			bool CanSkipActorUpdate() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (pActor->IsInDialoguewithPlayer())
					return true;

				return ThisCall<bool>(kCanSkipActorUpdateDetour, this);
			}

			void SetSkippedLastUpdate(bool abSkipped) {
				HighProcess* pProcess = reinterpret_cast<HighProcess*>(this);
				if (Interface::IsInDialog() && pProcess->GetTalkingtoPC())
					return;

				ThisCall(kSetSkippedLastUpdateDetour, this, abSkipped);
			}

			bool IsInCombat_IsInDialogue() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (CanActorBeIgnored(pActor))
					return true;

				return ThisCall<bool>(kIsInCombatDetour, pActor);
			}

			TESPackage* GetRunOncePackage_IsInDialogue() {
				BaseProcess* pProcess = reinterpret_cast<BaseProcess*>(this);
				if (Interface::IsInDialog() && pProcess->GetTalkingtoPC())
					return nullptr;

				if (kGetRunOncePackageDetour)
					return ThisCall<TESPackage*>(kGetRunOncePackageDetour, this);
				else
					return pProcess->GetRunOncePackage();
			}

			bool CanSpeak_IsInDialogue() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (CanActorBeIgnored(pActor))
					return false;

				return ThisCall<bool>(kCanSpeakDetour, this);
			}

			Actor* ShouldStopCombat() {
				CombatController* pCtrl = reinterpret_cast<CombatController*>(this);
				Actor* pActor = ThisCall<Actor*>(kShouldStopCombatDetour, pCtrl);
				TESObjectREFR* pTarget = pCtrl->GetPackageTarget();

				if (pActor && IsActorExcludedFromDialogueCombat(pActor, false))
					return nullptr;

				if (pTarget && pTarget->IsActor()) {
					Actor* pTargetActor = static_cast<Actor*>(pTarget);
					if (Settings::bAllowCombatInCompanionDialogue && pTargetActor->IsPlayerTeammate())
						return pActor;

					if (IsActorExcludedFromDialogueCombat(pTargetActor, true))
						return nullptr;
				}

				return pActor;
			}

			bool ShouldIgnoreInSandbox() {
				bool bIgnore = ThisCall<bool>(kGetIgnoredBySandboxDetour, this); // TESObjectREFR::IsIgnoredBySandbox
				TESObjectREFR* pThis = reinterpret_cast<TESObjectREFR*>(this);
				if (!bIgnore && pThis->IsActor()) {
					Actor* pActor = static_cast<Actor*>(pThis);
					if (pActor->IsInDialoguewithPlayer())
						return true;
				}
				return bIgnore;
			}

			Actor* GetBestDefaultHeadTrackTarget() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				Actor* pTarget = ThisCall<Actor*>(kGetBestDefaultHeadTrackTargetDetour, this);
				if (pTarget && pActor->IsInDialoguewithPlayer() && pTarget != PlayerCharacter::GetSingleton()) {
					return PlayerCharacter::GetSingleton();
				}
				return pTarget;
			}

			inline static bool bPlayerInCombat = false;

			static void OnDialogueStart(bool abShowScope) {
				bPlayerInCombat = PlayerCharacter::GetSingleton()->IsInCombat();
				CdeclCall(kOnDialogueStartDetour, abShowScope);
			}

			static void OnDialogueEnd() {
				if (bPlayerInCombat) {
					Actor* pPlayer = PlayerCharacter::GetSingleton();
					pPlayer->GetCurrentProcess()->SetActorsDetectionEvent(pPlayer, pPlayer->GetLocationOnReference(), 100, 3, pPlayer);
				}
				CdeclCall(kOnDialogueEndDetour);
			}

			bool CanMove() {
				Actor* pThis = reinterpret_cast<Actor*>(this);
				if (pThis == PlayerCharacter::GetSingleton())
					return false;

				return ThisCall<bool>(kIsInDialoguewithPlayerDetour, this);
			}

			BSSoundHandle* GetSoundHandleByFilename(BSSoundHandle* apOut, const char* apFileName, uint32_t auiSoundFlags, TESSound* apSound) {
				uint8_t* pEBP = HookUtils::GetParentBasePtr();
				Actor* pActor = *reinterpret_cast<Actor**>(pEBP - 0x298);
				if (pActor->IsInDialoguewithPlayer())
					auiSoundFlags |= BSGameSound::TypeFlags::UNKBIT10;

				return ThisCall<BSSoundHandle*>(kGetSoundHandleByFilenameDetour, this, apOut, apFileName, auiSoundFlags, apSound);
			}
		};

		bool __fastcall ExcludeCombatTarget(Actor* apActor) {
			if (Settings::bAllowCombatInCompanionDialogue && apActor->IsPlayerTeammate())
				return false;

			return IsActorExcludedFromDialogueCombat(apActor, true);
		}

		SPEC_NAKED void ExcludeComabtTarget_Asm() {
			static constexpr uint32_t uiCheckZone	= 0x986DF0; // Check encounter zone
			static constexpr uint32_t uiSkipZone	= 0x986E08; // Skip encounter zone check
			static constexpr uint32_t uiContinue	= 0x986D54; // Next target in the loop
			__asm {
				mov		ecx, dword ptr ss : [ebp - 0x58]
				call	ExcludeCombatTarget
				test	al, al
				jz		DONE

				jmp		uiContinue
				
				DONE:
				cmp		byte ptr ss : [0x11F1844], al // bEncounterZoneTargetRestrict:Combat
				jz		SKIP_ZONE

				jmp		uiCheckZone

				SKIP_ZONE:
				jmp		uiSkipZone
			}
		}

		static bool __fastcall ShouldWaitForTargetInDialogue(DialoguePackage* apPackage) {
			Actor* pTarget = apPackage->pTargetActor;
			Actor* pActor = apPackage->pActor;
			if (pTarget && (pTarget->IsInDialoguewithPlayer() || (pTarget == PlayerCharacter::GetSingleton() && Interface::IsInMenuMode()))) {
				const bool bIsMoving = pActor->IsMoving();
				const bool bIsAtTarget = apPackage->IsActorAtRefTarget(pActor, 96);
				bool bIssuedMoveRequest = false;
				if (!bIsMoving && !bIsAtTarget)
					bIssuedMoveRequest = pActor->SetPathfindingGoal(pTarget, -1.f, nullptr);

				if (bIsMoving && bIsAtTarget)
					pActor->ForceStopMoving();

				return true;
			}

			return false;
		}

		SPEC_NAKED void ShouldWaitForTargetInDialogue_Asm() {
			static constexpr uint32_t uiFailExit		= 0x8E8EF3; // Exit current scope completely
			static constexpr uint32_t uiStartHeadTrack	= 0x8E8DD5; // Start headtracking
			static constexpr uint32_t uiActivateTarget	= 0x8E8E81; // Skip headtracking
			__asm {
				mov		ecx, [ebp - 8]
				call	ShouldWaitForTargetInDialogue
				test	al, al
				jnz		EXIT // Target is busy, ignore!

				mov		ecx, [ebp - 8]
				call	TESPackage::GetPackageSecondLocation
				test	eax, eax
				jz		ACTIVATE_TARGET

				jmp		uiStartHeadTrack

				ACTIVATE_TARGET:
				jmp		uiActivateTarget

				EXIT:
				jmp		uiFailExit
			}
		}

		SPEC_NAKED void SetVoiceVolume_FadeOut_Asm() {
			static constexpr uint32_t uiJumpTarget = 0xAE1C64;
			__asm {
				mov     eax, [ebp - 0x1C] // Get Sound
				mov		ecx, [eax + 0x8]  // Get sound flags
				and		ecx, 4			  // Test if sound has voice flag
				jnz		VOICE_SOUND

				mov     eax, [ebp - 0x218] // Use existing value (fDialogueFadeDecibels)
				jmp		EXIT

				VOICE_SOUND:
				mov		eax, Settings::fDialogueVoiceFadeDecibels

				EXIT:
				jmp		uiJumpTarget
			}
		}

		SPEC_NAKED void SetVoiceVolume_FadeIn_Asm() {
			static constexpr uint32_t uiJumpTarget = 0xAE1D63;
			__asm {
				mov     edx, [ebp - 0x1C] // Get Sound
				mov		eax, [edx + 0x8]  // Get sound flags
				and		eax, 4			  // Test if sound has voice flag
				jnz		VOICE_SOUND

				mov     edx, [ebp - 0x228] // Use existing value (fDialogueFadeDecibels)
				jmp		EXIT

				VOICE_SOUND:
				mov		edx, Settings::fDialogueVoiceFadeDecibels

				EXIT:
				jmp		uiJumpTarget
			}
		}

		template<uint32_t uiPass, uint32_t uiFail>
		SPEC_NAKED void CheckDialogueSoundFlag_Asm() {
			static constexpr uint32_t uiJumpTargetPass = uiPass;
			static constexpr uint32_t uiJumpTargetFail = uiFail;
			__asm {
				mov		eax, [ebp - 0x1C]	// Get sound
				mov		edx, [eax + 0x8]	// Get sound flags
				and		edx, 0x400			// Test if sound has dialogue flag
				jnz		PASS

				jmp		uiJumpTargetFail

				PASS:
				jmp		uiJumpTargetPass
			}
		}
	}

}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	namespace Dialogue {

		void InitHooks() {

		}

		void InitDeferredHooks() {
			Hook_CanBeActivated<0x607A0A>(); // Don't allow to activate actors in dialogue
			Hook_CanBeActivated<0x5FA3B0>(); // Same, but creatures

			HookUtils::WriteRelJump(0x8E8DC8, ShouldWaitForTargetInDialogue_Asm);

			if (!Settings::IsMenuPaused(Interface::Dialog)) {
				Hook_FakeInDialogue<0x4537C9>(); // TES::UpdateCellMainThread
				Hook_FakeInDialogue<0x86E6E7>(); // TESMain::OnIdle
				Hook_FakeInDialogue<0x86E7CE>(); // TESMain::OnIdle

				Hook_CanUpdateActor<0x96C816>(); // Disable high update for dialogue actor (updated elsewhere)
				Hook_CanUpdateActor<0x96DBBB>(); // Disable general update for dialogue actor (updated elsewhere)
				Hook_CanUpdateActor<0x96CCFE>(); // Disable anim update for dialogue actor (updated elsewhere)
				Hook_CanUpdateActor<0x96CE5A>(); // Disable movement anim update for dialogue actor (updated elsewhere)

				kCanSkipActorUpdateDetour.ReplaceCall(0x96BE40, &Hook::CanSkipActorUpdate); // Disable actor update for dialogue actor (updated elsewhere)
				kSetSkippedLastUpdateDetour.ReplaceCall(0x96BE83, &Hook::SetSkippedLastUpdate); // Don't mark as skipped if in dialogue

				kCloseDialogueIfDeadDetour.ReplaceCall(0x76298A, &Hook::GetCurrentProcess_CanActorSpeak); // Close dialogue menu if speaker is dead

				kIsInCombatDetour.ReplaceCall(0x8EF5E5, &Hook::IsInCombat_IsInDialogue); // Don't greet chatter in dialogue

				kGetRunOncePackageDetour.ReplaceVirtualCall(0x8EFF90, &Hook::GetRunOncePackage_IsInDialogue, 8); // Don't clear run-once package if in dialogue and fAwarePlayerTimer runs out

				kCanSpeakDetour.ReplaceCall(0x9018B6, &Hook::CanSpeak_IsInDialogue);

				kGetIgnoredBySandboxDetour.ReplaceCall(0x9F513C, &Hook::ShouldIgnoreInSandbox); // Ignore actor in dialogue as a sandbox choice

				kGetBestDefaultHeadTrackTargetDetour.ReplaceCall(0x8A35DB, &Hook::GetBestDefaultHeadTrackTarget); // Force actor in dialogue to choose player

				kIsInDialoguewithPlayerDetour.ReplaceCall(0x8A62DC, &Hook::CanMove);

				// Allow voice fade out in dialogue
				HookUtils::WriteRelJump(0xADFC3F, CheckDialogueSoundFlag_Asm<0xADFD49, 0xADFC5A>);
				HookUtils::WriteRelJump(0xAE063C, CheckDialogueSoundFlag_Asm<0xAE0746, 0xAE0657>);
				HookUtils::WriteRelJump(0xAE1B90, CheckDialogueSoundFlag_Asm<0xAE1DF5, 0xAE1BAB>);
				kGetSoundHandleByFilenameDetour.ReplaceCall(0x8A2894, &Hook::GetSoundHandleByFilename);
				HookUtils::SafeWrite8(0x8A567F + 2, 0x5);
				HookUtils::SafeWrite8(0x7972CA + 2, 0x5);// Holotapes in PipBoy
				HookUtils::WriteRelJump(0xAE1C5E, SetVoiceVolume_FadeOut_Asm);
				HookUtils::WriteRelJump(0xAE1D5D, SetVoiceVolume_FadeIn_Asm);
				Hook_SetVoiceVolume<0xADFD44>();
				Hook_SetVoiceVolume<0xAE0741>();

				// Update camera and player in dialogue ourselves
				Hook_DialogueCamera<0x762E8A>();
				Hook_DialogueCamera<0x762F85>();
				HookUtils::WriteRelJump(0x953AFA, 0x953BB4); // Disables game's player updates in Dialogue

				// Don't update lights twice while in dialogue (Dialogue updates lights on its own)
				HookUtils::SafeWrite8(0x8A5E14, 0xEB);
				HookUtils::SafeWrite8(0x933CEA, 0xEB);

				if (Settings::bStopCombatInDialogue) {
					// Handle combat AI for actors when in dialogue. Don't attack player, the speaker, player's followers or teammates.
					kShouldStopCombatDetour.ReplaceCall(0x97DAAE, &Hook::ShouldStopCombat);
					HookUtils::WriteRelJump(0x986DDF, ExcludeComabtTarget_Asm);

					kOnDialogueStartDetour.ReplaceCall(0x762083, Hook::OnDialogueStart);
					kOnDialogueEndDetour.ReplaceCall(0x7624AA, Hook::OnDialogueEnd);
				}

				if (Settings::bNoDamageInDialogue) {
					Hook_IsInvulnerable<0x108A9B4, true>();  // Make player invulnerable in dialogue
					Hook_IsInvulnerable<0x10841CC, false>(); // Make speaker invulnerable in dialogue
					Hook_IsInvulnerable<0x10869E4, false>(); // Make speaker invulnerable in dialogue
					Hook_IsInvulnerable<0x1087024, false>(); // Make speaker invulnerable in dialogue
				}

				HMODULE hEnhancedCamera = GetModuleHandleA("NVSE_EnhancedCamera.dll");
				if (hEnhancedCamera) {
					if (*reinterpret_cast<uint16_t*>(Utils::GetDLLAddress(hEnhancedCamera, 0x10019B75)) == 0x5175) {
						_MESSAGE("Patching Enhanced Camera for Real Time Menus - Dialogue support");
						// UpdateCamera
						HookUtils::PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x10019B75), 2);
						HookUtils::PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x10019B7E), 2);
						HookUtils::PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x10019D36), 6);
						HookUtils::PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x10019D43), 6);

						// UpdateActorAnim
						HookUtils::PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x1001A551), 6);
						HookUtils::PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x1001A55E), 6);

						// TranslateThirdPerson
						HookUtils::PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x1001A758), 6);
						HookUtils::PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x1001A765), 6);
					}
				}
			}

			if (Settings::b3rdPersonInDialogue) {
				HookUtils::PatchMemoryNop(0x953124, 5);
				HookUtils::PatchMemoryNop(0x761DEF, 5);
				HookUtils::WriteRelJump(0x953ABF, 0x953AF5);
				HookUtils::WriteRelJump(0x762E55, 0x762E72);
			}

			if (Settings::bNoDialogueZoom) {
				HookUtils::WriteRelJump(0x9533BE, 0x953562);
				HookUtils::SafeWrite8(0x953BBA, 0xEB);
			}
		}

	}

}

#pragma endregion