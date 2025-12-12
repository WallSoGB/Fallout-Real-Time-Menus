#include "RTM-Settings.hpp"

#include "simpleini/SimpleIni.h"

namespace RealTimeMenus {

	namespace Settings {

		// Main
		bool bNoHardcoreInMenus = true;
		bool bControllerRumbleInMenus = false;
		uint32_t uiCompanionMenuHandling = 0;

		// Menus
		bool bPauseContainers = false;
		bool bPauseLockPickMenu = false;
		bool bPauseComputerMenu = false;
		bool bPauseUtilityMenus = true;
		bool bPauseCompanionWheel = false;
		bool bPausePipBoy = false;
		bool bPauseDialogue = false;
		bool bPauseVATS = false;

		// Backgrounds
		bool bBackgroundBlurInContainers = true;
		bool bBackgroundBlurInLockPickMenu = true;
		bool bBackgroundBlurInComputerMenu = true;
		bool bBackgroundBlurInCompanionWheel = true;
		bool bBackgroundBlurInPipBoy = true;

		// PipBoy
		bool bShakeCameraInPipBoy = true;
		bool bScreenBloodInPipBoy = true;
		bool bHitEffectInPipBoy = false;

		// VATS
		float fVATSTimeMultiplier = 0.02f;
		bool bShowHandsInVATS = true;

		// Dialogue
		bool bNoDialogueZoom = true;
		bool bStopCombatInDialogue = true;
		bool bNoDamageInDialogue = true;
		bool bAllowCombatInCompanionDialogue = false;
		bool bLookAroundInDialogue = true;
		bool b3rdPersonInDialogue = false;
		bool bShowHandsInDialogue = true;

		bool bGameModeInDialogue = false;
		bool bMenuModeForSpeakersOnly = true;
		bool bMenuModeForActorsOnly = false;

		void ReadSettings() {

			CSimpleIniA kINI;
			kINI.SetUnicode();
			SI_Error eError = kINI.LoadFile("Data\\NVSE\\Plugins\\RealTimeMenus.ini");
			if (eError != SI_OK){
				MessageBox(nullptr, "INI file not found!\nPlease download it from the mod page!", "Real Time Menus", MB_OK | MB_ICONWARNING);
			}

			// Main
			Settings::bNoHardcoreInMenus				= kINI.GetBoolValue("Main", "bNoHardcoreInMenus", true);
			Settings::bControllerRumbleInMenus			= kINI.GetBoolValue("Main", "bControllerRumbleInMenus", true);
			Settings::uiCompanionMenuHandling			= kINI.GetLongValue("Main", "uiCompanionMenuHandling", 0);

			// Menus
			Settings::bPauseContainers					= kINI.GetBoolValue("Menus", "bPauseContainers", false);
			Settings::bPauseLockPickMenu				= kINI.GetBoolValue("Menus", "bPauseLockPickMenu", false);
			Settings::bPauseComputerMenu				= kINI.GetBoolValue("Menus", "bPauseComputerMenu", false);
			Settings::bPauseCompanionWheel				= kINI.GetBoolValue("Menus", "bPauseCompanionWheel", false);
			Settings::bPausePipBoy						= kINI.GetBoolValue("Menus", "bPausePipBoy", false);
			Settings::bPauseDialogue					= kINI.GetBoolValue("Menus", "bPauseDialogue", false);
			Settings::bPauseVATS						= kINI.GetBoolValue("Menus", "bPauseVATS", false);

			// Backgrounds
			Settings::bBackgroundBlurInContainers		= kINI.GetBoolValue("Backgrounds", "bBackgroundBlurInContainers", true);
			Settings::bBackgroundBlurInLockPickMenu		= kINI.GetBoolValue("Backgrounds", "bBackgroundBlurInLockPickMenu", true);
			Settings::bBackgroundBlurInComputerMenu		= kINI.GetBoolValue("Backgrounds", "bBackgroundBlurInComputerMenu", true);
			Settings::bBackgroundBlurInCompanionWheel	= kINI.GetBoolValue("Backgrounds", "bBackgroundBlurInCompanionWheel", true);
			Settings::bBackgroundBlurInPipBoy			= kINI.GetBoolValue("Backgrounds", "bBackgroundBlurInPipBoy", true);

			// PipBoy
			Settings::bShakeCameraInPipBoy				= kINI.GetBoolValue("PipBoy", "bShakeCameraInPipBoy", true);
			Settings::bScreenBloodInPipBoy				= kINI.GetBoolValue("PipBoy", "bScreenBloodInPipBoy", true);
			Settings::bHitEffectInPipBoy				= kINI.GetBoolValue("PipBoy", "bHitEffectInPipBoy", true);

			// VATS
			Settings::fVATSTimeMultiplier				= kINI.GetDoubleValue("VATS", "fVATSTimeMult", 0.02);
			Settings::bShowHandsInVATS					= kINI.GetBoolValue("VATS", "bShowHandsInVATS", true);

			// Dialogue
			Settings::bNoDialogueZoom					= kINI.GetBoolValue("Dialogue", "bNoDialogueZoom", false);
			Settings::bStopCombatInDialogue				= kINI.GetBoolValue("Dialogue", "bStopCombatInDialogue", true);
			Settings::bNoDamageInDialogue				= kINI.GetBoolValue("Dialogue", "bNoDamageInDialogue", true);
			Settings::bAllowCombatInCompanionDialogue	= kINI.GetBoolValue("Dialogue", "bAllowCombatInCompanionDialogue", false);
			Settings::bLookAroundInDialogue				= kINI.GetBoolValue("Dialogue", "bLookAroundInDialogue", true);
			Settings::b3rdPersonInDialogue				= kINI.GetBoolValue("Dialogue", "b3rdPersonInDialogue", false);
			Settings::bShowHandsInDialogue				= kINI.GetBoolValue("Dialogue", "bShowHandsInDialogue", true);

			Settings::bGameModeInDialogue				= kINI.GetBoolValue("Dialogue", "bGameModeInDialogue", false);
			Settings::bMenuModeForSpeakersOnly			= kINI.GetBoolValue("Dialogue", "bMenuModeForSpeakersOnly", true);
			Settings::bMenuModeForActorsOnly			= kINI.GetBoolValue("Dialogue", "bMenuModeForActorsOnly", false);
		}

	}

}