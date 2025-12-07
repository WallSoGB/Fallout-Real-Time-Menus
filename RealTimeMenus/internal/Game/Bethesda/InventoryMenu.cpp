#include "InventoryMenu.hpp"

InventoryMenu* InventoryMenu::GetSingleton() {
	return *reinterpret_cast<InventoryMenu**>(0x11D9EA4);
}