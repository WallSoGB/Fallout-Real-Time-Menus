#pragma once

#include "Menu.hpp"
#include "ListBox.hpp"

class ItemChange;

class RepairMenu : public Menu {
public:
	RepairMenu();
	~RepairMenu();

	Tile*					pTiles[13];
	ListBox<ItemChange*>	kRepairItems;

	static RepairMenu* GetSingleton();
};

ASSERT_SIZE(RepairMenu, 0x8C);