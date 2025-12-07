#pragma once

class PlayerCharacter;
class TESObjectREFR;
class TESForm;

namespace RealTimeMenus {

	namespace Utils {

		extern TESObjectREFR* pActivatedReference;

		struct AutoPlayerCall {
			static constexpr AddressPtr<uint8_t, 0x11E07A8> ucInPlayerUpdate;

			AutoPlayerCall() { ucInPlayerUpdate++; }

			~AutoPlayerCall() { ucInPlayerUpdate--; }
		};

		enum MenuPauseState : uint32_t {
			MENU_PAUSED	= 0, // Pauses the game, runs MenuMode only
			MENU_LIVE	= 1, // Does not pause the game, runs both GameMode and MenuMode
			MENU_MIXED	= 2, // Does not pause the game, runs only MenuMode
		};

		EXTERN_DLL_EXPORT bool IsLiveMenuInstanceAlive(uint32_t aeMenu);

		EXTERN_DLL_EXPORT MenuPauseState IsLiveMenu(uint32_t aeMenu, bool abCheckInstances, bool abGameModeCheck = false);

		// MenuModes that pause the game
		EXTERN_DLL_EXPORT bool IsMenuPausingGame();

		// MenuModes that prevent GameMode in scripts
		EXTERN_DLL_EXPORT bool IsMenuBlockingGameMode();

		SPEC_NOINLINE TESForm* GetCurrentMenuBackgroundFX();

		SPEC_NOINLINE void __fastcall UpdatePlayerMovement(PlayerCharacter* apPlayer, float afDelta, bool abUpdateAnims, bool abFullUpdate, bool abUpdateMovement);

		SPEC_NOINLINE void __fastcall ShouldCloseMenus(PlayerCharacter* apPlayer);

		SPEC_NOINLINE void __fastcall DeactivateOpenAnim(TESObjectREFR* apReference);

		SPEC_NOINLINE bool __fastcall HasOpenCloseAnims(const TESObjectREFR* apReference, bool abClose);
	
		SPEC_NOINLINE void __fastcall CloseReference(TESObjectREFR* apReference);
	}

}