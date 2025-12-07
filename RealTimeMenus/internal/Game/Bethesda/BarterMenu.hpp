#pragma once

#include "Menu.hpp"
#include "ListBox.hpp"

class ItemChange;
class TESObjectREFR;

class BarterMenu : public Menu {
public:
	BarterMenu();
	~BarterMenu() override;

	enum Buttons
	{
		BM_LeftFilterArrow				= 0x2,
		BM_ItemsTitle					= 0x3,
		BM_RightFilterArrow				= 0x4,
		BM_Container_LeftFilterArrow	= 0x7,
		BM_Container_Title				= 0x8,
		BM_Container_RightFilterArrow	= 0x9,
		BM_ButtonX						= 0xD,
		BM_ButtonB						= 0x13,
		BM_ListBox						= 0x17,
	};

	enum Filter : int32_t
	{
		kAllItems,
		kWeapons,
		kArmor,
		kAid,
		kMisc,
		kAmmo,
		kMAX = kAmmo,
	};

	Tile*								pTiles[22];
	TESObjectREFR*						pMerchantRef;
	float								fBarterTotalSum;
	uint32_t							unk088;
	uint32_t							uiPlayerGold;
	uint32_t							uiMerchantGold;
	float								fBuyValueMult;
	float								fSellValueMult;
	Filter								eLeftFilter;
	Filter								eRightFilter;
	uint32_t							uiMenuSoundID;
	ListBox<ItemChange*>				kLeftItems;
	ListBox<ItemChange*>				kRightItems;
	ListBox<ItemChange*>*				kCurrentItems;
	BSSimpleList<ItemChange*>			kLeftBarter;
	BSSimpleList<ItemChange*>			kRightBarter;
	uint32_t							unk11C;

	static BarterMenu* GetSingleton();
};

ASSERT_SIZE(BarterMenu, 0x120);