#pragma once

namespace RealTimeMenus {

	namespace Settings {

		// Main
		extern bool		bNoHardcoreInMenus;
		extern bool		bControllerRumbleInMenus;
		extern uint32_t uiCompanionMenuHandling;

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
		extern bool b3DVoiceInDialogue;

		extern bool bGameModeInDialogue;
		extern bool bMenuModeForSpeakersOnly;
		extern bool bMenuModeForActorsOnly;

		void ReadSettings();

		void SetMenuPaused(uint32_t aeMenu, bool abPaused);
		bool IsMenuPaused(uint32_t aeMenu);
		void SetMenuBackgroundBlur(uint32_t aeMenu, bool abBlurred);
		bool IsMenuBackgroundBlurred(uint32_t aeMenu);
	}

}