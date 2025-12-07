#include "InterfaceManager.hpp"
#include "Interface.hpp"

InterfaceManager* InterfaceManager::GetSingleton() {
	return *reinterpret_cast<InterfaceManager**>(0x11D8A80);
}

// GAME - 0x602170
VATSEffect* InterfaceManager::GetVATSEffect() {
	return &kVATSEffect;
}

// GAME - 0x7079F0
bool InterfaceManager::GetUseRenderedBackground() const {
	return bIsRenderedMenuSet || Interface::IsPipBoyMenuOpen();
}

// GAME - 0x644640
bool InterfaceManager::IsDebugTextVisible() const {
	return bDebugTextVisible;
}

// GAME - 0x704370
FOPipboyManager* InterfaceManager::GetPipboyManager() const {
	return pPipboyManager;
}

// GAME - 0x707AF0
FORenderedMenu* InterfaceManager::GetCurrentRenderedMenu() const {
	return Interface::IsPipBoyMenuOpen() ? pPipboyManager : pRenderedMenu;
}

// GAME - 0x714F70
int32_t InterfaceManager::GetEnterStack(uint32_t auiIndex) const {
	return iEnterStack[auiIndex];
}

// GAME - 0x714F90
bool InterfaceManager::IsMenuInStack(uint32_t auiMenuID) const {
	for (uint32_t i = 0; i < 10; ++i) {
		if (GetEnterStack(i) == auiMenuID)
			return true;
	}
	return false;
}

// GAME - 0x4B71D0
bool InterfaceManager::IsControllerConnected() {
	return CdeclCall<bool>(0x4B71D0);
}

// GAME - 0x715D40
float InterfaceManager::GetScreenWidth() {
	return CdeclCall<float>(0x715D40);
}

// GAME - 0x715DA0
float InterfaceManager::GetScreenHeight() {
	return CdeclCall<float>(0x715DA0);
}