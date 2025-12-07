#include "ContainerMenu.hpp"

ContainerMenu* ContainerMenu::GetSingleton() {
	return *reinterpret_cast<ContainerMenu**>(0x11D93F8);
}

// GAME - 0x75B750
void ContainerMenu::Close() {
	CdeclCall(0x75B750);
}
