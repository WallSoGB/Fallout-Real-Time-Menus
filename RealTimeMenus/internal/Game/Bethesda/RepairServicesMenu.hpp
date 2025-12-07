#pragma once

#include "Menu.hpp"
#include "ListBox.hpp"

class ItemChange;

class RepairServicesMenu : public Menu {
public:
	RepairServicesMenu();
	~RepairServicesMenu();

	Tile*					pTiles[16];
	ListBox<ItemChange*>	kItems;
	uint32_t				uiRepairAllCost;
	uint8_t					ucSkill;

	static RepairServicesMenu* GetSingleton();
};

ASSERT_SIZE(RepairServicesMenu, 0xA0);