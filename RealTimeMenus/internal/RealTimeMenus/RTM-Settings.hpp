#pragma once

namespace RealTimeMenus {

	namespace Settings {

		// Main
		extern bool		bNoHardcoreInMenus;
		extern bool		bControllerRumbleInMenus;
		extern uint32_t uiCompanionMenuHandling;

		// Menus
		extern bool bPauseContainers;
		extern bool bPauseBarter;
		extern bool bPauseServiceRepair;
		extern bool bPauseRecipeMenu;
		extern bool bPauseLockPickMenu;
		extern bool bPauseComputerMenu;
		extern bool bPauseCompanionWheel;
		extern bool bPausePipBoy;
		extern bool bPauseDialogue;
		extern bool bPauseVATS;

		// Backgrounds
		extern bool bBackgroundBlurInContainers;
		extern bool bBackgroundBlurInBarter;
		extern bool bBackgroundBlurInServiceRepair;
		extern bool bBackgroundBlurInRecipeMenu;
		extern bool bBackgroundBlurInLockPickMenu;
		extern bool bBackgroundBlurInComputerMenu;
		extern bool bBackgroundBlurInCompanionWheel;
		extern bool bBackgroundBlurInPipBoy;

		// PipBoy
		extern bool bShakeCameraInPipBoy;
		extern bool bScreenBloodInPipBoy;
		extern bool bHitEffectInPipBoy;

		// VATS
		extern float fVATSTimeMultiplier;
		extern bool bShowHandsInVATS;

		// Dialogue
		extern bool bNoDialogueZoom;
		extern bool bStopCombatInDialogue;
		extern bool bNoDamageInDialogue;
		extern bool bAllowCombatInCompanionDialogue;
		extern bool bLookAroundInDialogue;
		extern bool b3rdPersonInDialogue;
		extern bool bShowHandsInDialogue;

		extern bool bGameModeInDialogue;
		extern bool bMenuModeForSpeakersOnly;
		extern bool bMenuModeForActorsOnly;

		void ReadSettings();

	}

}