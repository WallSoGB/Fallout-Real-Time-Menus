#include "RTM-Settings.hpp"

#include "simpleini/SimpleIni.h"

#include "Bethesda/Interface.hpp"

namespace RealTimeMenus {

	namespace Settings {

		struct MenuSetting {
			bool bPause			 : 1 = true;
			bool bBackgroundBlur : 1 = true;
		};

		MenuSetting kPipBoyMenuSetting = {};
		MenuSetting kMenuSettings[100] = {};

		// Main
		bool bNoHardcoreInMenus = true;
		bool bControllerRumbleInMenus = false;
		uint32_t uiCompanionMenuHandling = 0;

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
			Settings::SetMenuPaused(Interface::Container,		kINI.GetBoolValue("Menus", "bPauseContainers", false));
			Settings::SetMenuPaused(Interface::Barter,			kINI.GetBoolValue("Menus", "bPauseBarter", false));
			Settings::SetMenuPaused(Interface::VendorRepair,	kINI.GetBoolValue("Menus", "bPauseServiceRepair", false));
			Settings::SetMenuPaused(Interface::Recipe,			kINI.GetBoolValue("Menus", "bPauseRecipeMenu", false));
			Settings::SetMenuPaused(Interface::LockPick,		kINI.GetBoolValue("Menus", "bPauseLockPickMenu", false));
			Settings::SetMenuPaused(Interface::Computers,		kINI.GetBoolValue("Menus", "bPauseComputerMenu", false));
			Settings::SetMenuPaused(Interface::CompanionWheel,	kINI.GetBoolValue("Menus", "bPauseCompanionWheel", false));
			Settings::SetMenuPaused(Interface::PipBoy,			kINI.GetBoolValue("Menus", "bPausePipBoy", false));
			Settings::SetMenuPaused(Interface::Dialog,			kINI.GetBoolValue("Menus", "bPauseDialogue", false));
			Settings::SetMenuPaused(Interface::VATS,			kINI.GetBoolValue("Menus", "bPauseVATS", false));

			// Backgrounds
			Settings::SetMenuBackgroundBlur(Interface::Container,		kINI.GetBoolValue("Backgrounds", "bBlurBackgroundContainers", false));
			Settings::SetMenuBackgroundBlur(Interface::Barter,			kINI.GetBoolValue("Backgrounds", "bBlurBackgroundBarter", false));
			Settings::SetMenuBackgroundBlur(Interface::VendorRepair,	kINI.GetBoolValue("Backgrounds", "bBlurBackgroundServiceRepair", false));
			Settings::SetMenuBackgroundBlur(Interface::Recipe,			kINI.GetBoolValue("Backgrounds", "bBlurBackgroundRecipeMenu", false));
			Settings::SetMenuBackgroundBlur(Interface::LockPick,		kINI.GetBoolValue("Backgrounds", "bBlurBackgroundLockPickMenu", false));
			Settings::SetMenuBackgroundBlur(Interface::Computers,		kINI.GetBoolValue("Backgrounds", "bBlurBackgroundComputerMenu", false));
			Settings::SetMenuBackgroundBlur(Interface::CompanionWheel,	kINI.GetBoolValue("Backgrounds", "bBlurBackgroundCompanionWheel", false));
			Settings::SetMenuBackgroundBlur(Interface::PipBoy,			kINI.GetBoolValue("Backgrounds", "bBlurBackgroundPipBoy", false));

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

		static inline MenuSetting& GetMenuSetting(uint32_t aeMenu) {
			switch (aeMenu) {
				case Interface::MainFour:
				case Interface::Inventory:
				case Interface::Stats:
				case Interface::PipboyData:
				case Interface::PipboyRepair:
				case Interface::ItemModMenu:
					return kPipBoyMenuSetting;
				case Interface::Hacking:
					return kMenuSettings[Interface::Computers - 1000];
				default:
					return kMenuSettings[aeMenu - 1000];
			}
		}

		void SetMenuPaused(uint32_t aeMenu, bool abPaused) {
			GetMenuSetting(aeMenu).bPause = abPaused;
		}

		bool IsMenuPaused(uint32_t aeMenu) {
			return GetMenuSetting(aeMenu).bPause;
		}

		void SetMenuBackgroundBlur(uint32_t aeMenu, bool abBlurred) {
			GetMenuSetting(aeMenu).bBackgroundBlur = abBlurred;
		}

		bool IsMenuBackgroundBlurred(uint32_t aeMenu) {
			return GetMenuSetting(aeMenu).bBackgroundBlur;
		}

	}

}