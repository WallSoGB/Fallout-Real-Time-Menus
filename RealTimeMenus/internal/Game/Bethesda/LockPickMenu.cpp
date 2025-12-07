#include "LockPickMenu.hpp"

LockPickMenu* LockPickMenu::GetSingleton() { 
	return *reinterpret_cast<LockPickMenu**>(0x11DA204); 
}

// GAME - 0x78E0D0
void LockPickMenu::Close() {
	CdeclCall(0x78E0D0);
}