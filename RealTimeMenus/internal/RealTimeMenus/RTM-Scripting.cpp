#include "RTM-Scripting.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/Interface.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/Script.hpp"
#include "Bethesda/TESMain.hpp"

#include "shared/NVSEManager/NVSEGlobalManager.hpp"

#pragma region Code

namespace RealTimeMenus {

	namespace Scripting {

#define DEBUG_DIALOGUE_SCRIPTS 0

#define SCRIPT_FUNC(name) bool __cdecl name##_Execute(COMMAND_ARGS)
#define SCRIPT_FUNC_NAME(name) name##_Execute

#define CONDITION_FUNC(name) bool __cdecl name##_Cond(COMMAND_ARGS_EVAL)
#define CONDITION_FUNC_NAME(name) name##_Cond

		CONDITION_FUNC(IsMenuPaused) {
			uint32_t uiMenuID = reinterpret_cast<uint32_t>(apParam1);
			if (uiMenuID == 0) {
				*adResult = RealTimeMenus::Utils::IsMenuBlockingGameMode() ? 1.0 : 0.0;
			}
			else {
				RealTimeMenus::Utils::MenuPauseState eState = RealTimeMenus::Utils::IsLiveMenu(uiMenuID, false);
				switch (eState) {
					case RealTimeMenus::Utils::MenuPauseState::MENU_PAUSED:
						*adResult = 1.0;
						break;
					case RealTimeMenus::Utils::MenuPauseState::MENU_LIVE:
						*adResult = 0.0;
						break;
					case RealTimeMenus::Utils::MenuPauseState::MENU_MIXED:
						*adResult = 2.0;
						break;
					default:
						__assume(false);
				}
			}
			return true;
		}

		SCRIPT_FUNC(IsMenuPaused) {
			*adResult = 0;
			uint32_t uiMenuID = 0;
			if (NVSEGlobalManager::ExtractArgsEx(EXTRACT_ARGS_EX, &uiMenuID)) {
				CONDITION_FUNC_NAME(IsMenuPaused)(apRef, reinterpret_cast<void*>(uiMenuID), nullptr, adResult);
			}
			return true;
		}

		SCRIPT_FUNC(GameMode) {
			const bool bMenuMode = RealTimeMenus::Utils::IsMenuBlockingGameMode() || TESMain::bIsInPauseFade;
			bool bGameMode = bMenuMode == false;

			if (apRef && !Settings::bPauseDialogue && bGameMode) {
				if (Settings::bGameModeInDialogue && Interface::IsInDialog()) {
					if (apRef->IsActor()) {
						if (Settings::bMenuModeForSpeakersOnly) {
							const Actor* pActor = static_cast<Actor*>(apRef);
							if (pActor == PlayerCharacter::GetSingleton() || pActor->IsInDialoguewithPlayer()) {
								bGameMode = false;
							}
						}
						else if (Settings::bMenuModeForActorsOnly) {
							bGameMode = false;
						}
					}

#if DEBUG_DIALOGUE_SCRIPTS
					const char* pScriptEDID = apScript->GetFormEditorID();
					const uint32_t uiScriptFormID = apScript->GetFormID();
					const char* pRefEDID = apRef->GetFormEditorID();
					const uint32_t uiRefFormID = apRef->GetFormID();
					_MESSAGE("[ %08i ] \"%-40s\" (0x%08X) on \"%-40s\" (0x%08X) -> %s", TESMain::iFrameCounter.Get(), pScriptEDID, uiScriptFormID, pRefEDID, uiRefFormID, bGameMode ? "Runs" : "Paused");
#endif
				}
			}


			*adResult = bGameMode;

			return true;
		}

	}

}

#pragma endregion

#pragma region Hooks

namespace RealTimeMenus {

	namespace Scripting {

		void ReplaceCommands() {
			NVSEGlobalManager::GetSingleton().ReplaceScriptFunctions("IsMenuPaused", SCRIPT_FUNC_NAME(IsMenuPaused), CONDITION_FUNC_NAME(IsMenuPaused));
			WriteRelJump(0x5C42A0, GameMode_Execute);
		}

	}

}

#pragma endregion