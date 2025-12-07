#include "HackingMenu.hpp"

HackingMenu* HackingMenu::GetSingleton() { 
	return *reinterpret_cast<HackingMenu**>(0x11D95B8); 
}
