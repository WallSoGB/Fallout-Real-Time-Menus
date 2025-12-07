#include "StartMenu.hpp"

StartMenu* StartMenu::GetSingleton() {
	return *reinterpret_cast<StartMenu**>(0x11DAAC0);
}

// GAME - 0x70EDF0
bool StartMenu::MainMenuIsActive() {
	return CdeclCall<bool>(0x70EDF0);
}

// GAME - 0x4A4040
bool StartMenu::PauseMenuIsActive() {
	return CdeclCall<bool>(0x4A4040);
}