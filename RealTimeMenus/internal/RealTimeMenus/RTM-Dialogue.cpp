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

		bool IsActorExcludedFromDialogueCombat(Actor* apActor, bool abCheckFaction) {
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

		template<uint32_t uiAddress, bool bPlayer>
		class Hook_IsInvulnerable {
			static inline VirtFuncDetour kDetour;
			bool IsInvulnerable() {
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
				return ThisCall<bool>(kDetour.GetOverwrittenAddr(), this);
			}

		public:
			Hook_IsInvulnerable() {
				kDetour.ReplaceVirtualFuncEx(uiAddress, &Hook_IsInvulnerable::IsInvulnerable);
			};
		};


		template<uint32_t uiAddress>
		class Hook_FakeInDialogue {
			static inline CallDetour kDetour;
			static bool InDialog() {
				const uint32_t uiOverwrittenAddr = kDetour.GetOverwrittenAddr();
				if (uiOverwrittenAddr != 0x7050D0)
					CdeclCall(uiOverwrittenAddr);
				return false;
			}

		public:
			Hook_FakeInDialogue() {
				kDetour.ReplaceCall(uiAddress, InDialog);
			};
		};

		class Hook {
		private:
			SPEC_NOINLINE bool CanActorBeIgnored(Actor* apActor) {
				if (apActor == PlayerCharacter::GetSingleton() && Interface::IsInMenuMode())
					return true;

				if (apActor->IsInDialoguewithPlayer())
					return true;

				return false;
			}
		public:
			// 0x96C816
			bool CanActorUpdate() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (!pActor->IsActor())
					return false;

				return pActor->IsInDialoguewithPlayer() == false;
			}

			// 0x76298A
			void* GetCurrentProcess_CanActorSpeak() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (pActor->IsDead(false))
					return nullptr;

				return pActor->GetCurrentProcess();
			}

			bool CanSkipActorUpdate() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (pActor->IsInDialoguewithPlayer())
					return true;

				return ThisCall<bool>(0x8B0520, this);
			}

			void SetSkippedLastUpdate(bool abSkipped) {
				HighProcess* pProcess = reinterpret_cast<HighProcess*>(this);
				if (Interface::IsInDialog() && pProcess->GetTalkingtoPC())
					return;

				pProcess->bSkippedUpdate = abSkipped;
			}

			BaseProcess* CanActorBeActivated() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (CanActorBeIgnored(pActor))
					return nullptr;

				return pActor->GetCurrentProcess();
			}

			bool IsInCombat_IsInDialogue() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (CanActorBeIgnored(pActor))
					return true;

				return pActor->IsInCombat();
			}

			TESPackage* GetRunOncePackage_IsInDialogue() {
				BaseProcess* pProcess = reinterpret_cast<BaseProcess*>(this);
				if (Interface::IsInDialog() && pProcess->GetTalkingtoPC())
					return nullptr;

				return pProcess->GetRunOncePackage();
			}

			bool CanSpeak_IsInDialogue() {
				Actor* pActor = reinterpret_cast<Actor*>(this);
				if (CanActorBeIgnored(pActor))
					return false;

				return ThisCall<bool>(0x884480, this);
			}

			Actor* ShouldStopCombat() {
				CombatController* pCtrl = reinterpret_cast<CombatController*>(this);
				Actor* pActor = pCtrl->GetPackageOwner();
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
				bool bIgnore = ThisCall<bool>(0x57B500, this); // TESObjectREFR::IsIgnoredBySandbox
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
				Actor* pTarget = ThisCall<Actor*>(0x8A3ED0, this);
				if (pTarget && pActor->IsInDialoguewithPlayer() && pTarget != PlayerCharacter::GetSingleton()) {
					return PlayerCharacter::GetSingleton();
				}
				return pTarget;
			}

			inline static bool bPlayerInCombat = false;

			static void OnDialogueStart(bool abShowScope) {
				bPlayerInCombat = PlayerCharacter::GetSingleton()->IsInCombat();
				CdeclCall(0x709C40, abShowScope);
			}

			static void OnDialogueEnd() {
				if (bPlayerInCombat) {
					Actor* pPlayer = PlayerCharacter::GetSingleton();
					pPlayer->GetCurrentProcess()->SetActorsDetectionEvent(pPlayer, pPlayer->GetLocationOnReference(), 100, 3, pPlayer);
				}
				CdeclCall(0x7624C0);
			}

			void HandleDialogueLookAround() {
				PlayerCharacter* pPlayer = reinterpret_cast<PlayerCharacter*>(this);
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

						pPlayer->ModHeading(fHeading);
						pPlayer->ModLooking(fLooking);
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

						pPlayer->ModHeading(fHeading);
						pPlayer->ModLooking(fLooking);
					}
				}
			}

			void Dialogue_FocusOnActor(Actor* apTarget, float afZoomPercent, bool abOnlyZoom) {
				DialogMenu* pDialogMenu = DialogMenu::GetSingleton();
				if (!pDialogMenu)
					return;

				if (pDialogMenu->bInBackground)
					return;

				const float fTimeDelta = TimeGlobal::GetSingleton()->fDelta;
				PlayerCharacter* pThis = reinterpret_cast<PlayerCharacter*>(this);
				if (Settings::bLookAroundInDialogue && pDialogMenu->eStage != DialogMenu::CAMERA_ZOOM_IN) {
					if (PlayerCharacter::fDialogZoomPercent < 1.f) {
						ThisCall(0x953060, this, apTarget, afZoomPercent, abOnlyZoom);
					}
					else {
						HandleDialogueLookAround();
						CdeclCall(0xC66210);
					}
				}
				else {
					ThisCall(0x953060, this, apTarget, afZoomPercent, abOnlyZoom);
				}
				Utils::AutoPlayerCall kAutoPlayerCall;
				Utils::UpdatePlayerMovement(pThis, fTimeDelta, true, true, false);
			}

			bool CanMove() {
				Actor* pThis = reinterpret_cast<Actor*>(this);
				if (pThis == PlayerCharacter::GetSingleton())
					return false;

				return pThis->IsInDialoguewithPlayer();
			}

			static bool __fastcall ShouldWaitForTargetInDialogue(DialoguePackage* apPackage) {
				Actor* pTarget = apPackage->pTargetActor;
				Actor* pActor = apPackage->pActor;
				if (pTarget && (pTarget->IsInDialoguewithPlayer() || (pTarget == PlayerCharacter::GetSingleton() && Interface::IsInMenuMode()))) {
					bool bIsMoving = pActor->IsMoving();
					bool bIsAtTarget = apPackage->IsActorAtRefTarget(pActor, 96);
					bool bIssuedMoveRequest = false;
					if (!bIsMoving && !bIsAtTarget) {
						bIssuedMoveRequest = ThisCall<bool>(0x8B36F0, pActor, pTarget, -1.f, nullptr);
					}

					if (bIsMoving && bIsAtTarget) {
						ThisCall(0x8B3AD0, pActor);
					}

					return true;
				}

				return false;
			}

			static uint32_t __fastcall SetDialogueSoundFlag(Actor* apActor) {
				uint32_t uiFlags = BSGameSound::TypeFlags::VOICE | BSGameSound::TypeFlags::ONE_SHOT;
				if (apActor->IsInDialoguewithPlayer())
					uiFlags |= BSGameSound::TypeFlags::UNKBIT10;
				return uiFlags;
			}
		};

		bool __fastcall ExcludeCombatTarget(Actor* apActor) {
			if (Settings::bAllowCombatInCompanionDialogue && apActor->IsPlayerTeammate())
				return false;

			return IsActorExcludedFromDialogueCombat(apActor, true);
		}

		__declspec(naked) void ExcludeComabtTarget_Asm() {
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

		__declspec(naked) void ShouldWaitForTargetInDialogue_Asm() {
			static constexpr uint32_t uiFailExit		= 0x8E8EF3; // Exit current scope completely
			static constexpr uint32_t uiStartHeadTrack	= 0x8E8DD5; // Start headtracking
			static constexpr uint32_t uiActivateTarget	= 0x8E8E81; // Skip headtracking
			__asm {
				mov		ecx, [ebp - 8]
				call	Hook::ShouldWaitForTargetInDialogue
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

		__declspec(naked) void SetDialogueSoundFlag_Asm() {
			static constexpr uint32_t uiJumpTarget = 0x8A2820;
			__asm {
				mov     ecx, [ebp - 0x298]
				call	Hook::SetDialogueSoundFlag
				mov		dword ptr[ebp - 0x240], eax
				jmp		uiJumpTarget
			}
		}

		template<uint32_t uiPass, uint32_t uiFail>
		__declspec(naked) void CheckDialogueSoundFlag_Asm() {
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
			ReplaceCallEx(0x607A0A, &Hook::CanActorBeActivated); // Don't allow to activate actors in dialogue
			ReplaceCallEx(0x5FA3B0, &Hook::CanActorBeActivated); // Same, but creatures

			WriteRelJump(0x8E8DC8, ShouldWaitForTargetInDialogue_Asm);

			if (!Settings::bPauseDialogue) {
				Hook_FakeInDialogue<0x4537C9>(); // TES::UpdateCellMainThread
				Hook_FakeInDialogue<0x86E6E7>(); // TESMain::OnIdle
				Hook_FakeInDialogue<0x86E7CE>(); // TESMain::OnIdle

				PatchMemoryNop(0x96C816, 8); // Disable high update for dialogue actor (updated elsewhere)
				WriteRelCallEx(0x96C816, &Hook::CanActorUpdate);

				PatchMemoryNop(0x96DBBB, 8); // Disable general update for dialogue actor (updated elsewhere)
				WriteRelCallEx(0x96DBBB, &Hook::CanActorUpdate);

				PatchMemoryNop(0x96CCFE, 8); // Disable anim update for dialogue actor (updated elsewhere)
				WriteRelCallEx(0x96CCFE, &Hook::CanActorUpdate);

				PatchMemoryNop(0x96CE5A, 8); // Disable movement anim update for dialogue actor (updated elsewhere)
				WriteRelCallEx(0x96CE5A, &Hook::CanActorUpdate);

				ReplaceCallEx(0x96BE40, &Hook::CanSkipActorUpdate); // Disable actor update for dialogue actor (updated elsewhere)
				ReplaceCallEx(0x96BE83, &Hook::SetSkippedLastUpdate); // Don't mark as skipped if in dialogue

				ReplaceCallEx(0x76298A, &Hook::GetCurrentProcess_CanActorSpeak); // Close dialogue menu if speaker is dead

				ReplaceCallEx(0x8EF5E5, &Hook::IsInCombat_IsInDialogue); // Don't greet chatter in dialogue

				PatchMemoryNop(0x8EFF90, 8); // Don't clear run-once package if in dialogue and fAwarePlayerTimer runs out
				WriteRelCallEx(0x8EFF90, &Hook::GetRunOncePackage_IsInDialogue);

				ReplaceCallEx(0x9018B6, &Hook::CanSpeak_IsInDialogue);

				ReplaceCallEx(0x9F513C, &Hook::ShouldIgnoreInSandbox); // Ignore actor in dialogue as a sandbox choice

				ReplaceCallEx(0x8A35DB, &Hook::GetBestDefaultHeadTrackTarget); // Force actor in dialogue to choose player

				ReplaceCallEx(0x8A62DC, &Hook::CanMove); // Force actor in dialogue to choose player

				// Allow voice fade out in dialogue
				WriteRelJump(0xADFC3F, uint32_t(CheckDialogueSoundFlag_Asm<0xADFD49, 0xADFC5A>));
				WriteRelJump(0xAE063C, uint32_t(CheckDialogueSoundFlag_Asm<0xAE0746, 0xAE0657>));
				WriteRelJump(0xAE1B90, uint32_t(CheckDialogueSoundFlag_Asm<0xAE1DF5, 0xAE1BAB>));
				WriteRelJump(0x8A2816, uint32_t(SetDialogueSoundFlag_Asm));
				SafeWrite8(0x8A567F + 2, 0x5);
				SafeWrite8(0x7972CA + 2, 0x5);// Holotapes in PipBoy

				// Update camera and player in dialogue ourselves
				ReplaceCallEx(0x762E8A, &Hook::Dialogue_FocusOnActor);
				ReplaceCallEx(0x762F85, &Hook::Dialogue_FocusOnActor);
				WriteRelJump(0x953AFA, 0x953BB4); // Disables game's player updates in Dialogue

				if (Settings::bStopCombatInDialogue) {
					// Handle combat AI for actors when in dialogue. Don't attack player, the speaker, player's followers or teammates.
					ReplaceCallEx(0x97DAAE, &Hook::ShouldStopCombat);
					WriteRelJump(0x986DDF, ExcludeComabtTarget_Asm);

					ReplaceCall(0x762083, Hook::OnDialogueStart);
					ReplaceCall(0x7624AA, Hook::OnDialogueEnd);
				}

				if (Settings::bNoDamageInDialogue) {
					Hook_IsInvulnerable<0x108A9B4, true>();  // Make player invulnerable in dialogue
					Hook_IsInvulnerable<0x10841CC, false>(); // Make speaker invulnerable in dialogue
					Hook_IsInvulnerable<0x10869E4, false>(); // Make speaker invulnerable in dialogue
					Hook_IsInvulnerable<0x1087024, false>(); // Make speaker invulnerable in dialogue
				}

				if (Settings::b3DVoiceInDialogue) {
					SafeWrite8(0x8A5714 + 1, 0); // Play dialogue in 3D
				}
			}

			if (Settings::b3rdPersonInDialogue) {
				PatchMemoryNop(0x953124, 5);
				PatchMemoryNop(0x761DEF, 5);
				WriteRelJump(0x953ABF, 0x953AF5);
				WriteRelJump(0x762E55, 0x762E72);
			}

			if (Settings::bNoDialogueZoom) {
				WriteRelJump(0x9533BE, 0x953562);
				SafeWrite8(0x953BBA, 0xEB);
			}


			HMODULE hEnhancedCamera = GetModuleHandleA("NVSE_EnhancedCamera.dll");
			if (!Settings::bPauseDialogue && hEnhancedCamera) {
				if (*reinterpret_cast<uint16_t*>(Utils::GetDLLAddress(hEnhancedCamera, 0x10019B75)) == 0x5175) {
					_MESSAGE("Patching Enhanced Camera for Real Time Menus - Dialogue support");
					// UpdateCamera
					PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x10019B75), 2);
					PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x10019B7E), 2);
					PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x10019D36), 6);
					PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x10019D43), 6);

					// UpdateActorAnim
					PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x1001A551), 6);
					PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x1001A55E), 6);

					// TranslateThirdPerson
					PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x1001A758), 6);
					PatchMemoryNop(Utils::GetDLLAddress(hEnhancedCamera, 0x1001A765), 6);
				}
				else {
					MessageBoxA(nullptr, "Real Time Menus detected an outdated version of Enhanced Camera.\nPlease update to the 1.4f version from Nexus Mods.\nThe mod page is called \"New Vegas - Enhanced Camera - Detached head hotfix\"", "Real Time Menus", MB_OK | MB_ICONERROR);
					ExitProcess(0);
				}
			}
		}

	}

}

#pragma endregion