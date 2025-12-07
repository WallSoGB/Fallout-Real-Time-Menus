#include "MapMenu.hpp"

MapMenu* MapMenu::GetSingleton() { 
	return *reinterpret_cast<MapMenu**>(0x11DA368); 
};

// GAME - 0x79E0A0
void MapMenu::AddQuestMarkers() {
	ThisCall(0x79E0A0, this);
}

// GAME - 0x79EA30
void MapMenu::AddCompanionMarkers() {
	ThisCall(0x79EA30, this);
}
