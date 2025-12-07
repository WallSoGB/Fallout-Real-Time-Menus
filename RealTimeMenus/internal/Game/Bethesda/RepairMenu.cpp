#include "RepairMenu.hpp"

RepairMenu* RepairMenu::GetSingleton() { 
	return *reinterpret_cast<RepairMenu**>(0x11DA75C); 
};
