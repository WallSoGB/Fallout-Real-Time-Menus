#include "CompanionWheelMenu.hpp"

CompanionWheelMenu* CompanionWheelMenu::GetSingleton() {
	return *reinterpret_cast<CompanionWheelMenu**>(0x11D92B8);
}
