#include "RTM-Scripting.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/Interface.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/Script.hpp"
#include "Bethesda/TESMain.hpp"

#include "shared/NVSEManager/NVSEGlobalManager.hpp"

#include "simpleini/SimpleIni.h"

#pragma region Code

namespace RealTimeMenus {

	namespace Scripting {

#define DEBUG_DIALOGUE_SCRIPTS 0

#define SCRIPT_FUNC(name) bool __cdecl name##_Execute(COMMAND_ARGS)
#define SCRIPT_FUNC_NAME(name) name##_Execute

#define CONDITION_FUNC(name) bool __cdecl name##_Cond(COMMAND_ARGS_EVAL)
#define CONDITION_FUNC_NAME(name) name##_Cond

		namespace {
			static constexpr uint32_t SCRIPT_FLAG_PAUSED_GAME_MODE = 1u << 30;

			HANDLE hScriptBlockFileThread = nullptr;

			bool __fastcall BlockScript(const char* apScriptEDID) {
				TESForm* pForm = TESForm::GetFormByEditorID(apScriptEDID);
				if (pForm && pForm->GetFormType() == FORM_TYPE::Script && !pForm->uiFormFlags.Get(SCRIPT_FLAG_PAUSED_GAME_MODE)) {
					_MESSAGE("Blocking script \"%s\" (0x%08X) from in-menu GameMode", apScriptEDID, pForm->GetFormID());
					pForm->uiFormFlags.Set(SCRIPT_FLAG_PAUSED_GAME_MODE);
					return true;
				}
				return false;
			}

			bool __fastcall IsScriptBlocked(Script* apScript) {
				return apScript->uiFormFlags.Get(SCRIPT_FLAG_PAUSED_GAME_MODE);
			}

			uint32_t __fastcall ParseFile(const char* apPath) {
				uint32_t uiScriptsBlocked = 0;
				CSimpleIniA kINI;
				kINI.SetAllowKeyOnly();
				SI_Error eError = kINI.LoadFile(apPath);
				if (eError == SI_OK) {
					CSimpleIniA::TNamesDepend kKeys;
					if (kINI.GetAllKeys("BlockedScripts", kKeys)) {
						for (const auto& rKeyEntry : kKeys)
							uiScriptsBlocked += BlockScript(rKeyEntry.pItem);
					}
				}
				return uiScriptsBlocked;
			}

			void ParseBlockFiles() {
				const char* pPath = "Data\\RealTimeMenus\\BlockedScripts";
				uint32_t uiScriptsBlocked = 0;
				HANDLE hFind;
				WIN32_FIND_DATA kFindData;
				char cSearchPath[MAX_PATH];
				our_snprintf(cSearchPath, sizeof(cSearchPath), "%s\\*.ini", pPath);
				hFind = FindFirstFileA(cSearchPath, &kFindData);
				if (hFind != INVALID_HANDLE_VALUE) {
					do {
						if (!(kFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
							char cFullPath[MAX_PATH];
							our_snprintf(cFullPath, sizeof(cFullPath), "%s\\%s", pPath, kFindData.cFileName);
							_MESSAGE("Processing \"%s\"", kFindData.cFileName);
							uiScriptsBlocked += ParseFile(cFullPath);
						}
					} while (FindNextFileA(hFind, &kFindData) != 0);
					FindClose(hFind);
				}
				_MESSAGE("Flagged %i scripts", uiScriptsBlocked);
			}

			DWORD WINAPI ScriptBlockFileThreadProc(LPVOID lpParam) {
				ParseBlockFiles();
				return 0;
			}

			void LoadBlockFilesThreaded() {
				hScriptBlockFileThread = CreateThread(nullptr, 0, ScriptBlockFileThreadProc, nullptr, 0, nullptr);
			}

			void WaitForBlockFileThread() {
				if (hScriptBlockFileThread) {
					WaitForSingleObject(hScriptBlockFileThread, INFINITE);
					CloseHandle(hScriptBlockFileThread);
					hScriptBlockFileThread = nullptr;
				}
			}
		}

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

			if (bGameMode && Interface::IsInMenuMode() && IsScriptBlocked(apScript))
				bGameMode = false;

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

		void LoadBlockFiles() {
			LoadBlockFilesThreaded();
		}

		void WaitForBlockFiles() {
			WaitForBlockFileThread();
		}

	}

}

#pragma endregion