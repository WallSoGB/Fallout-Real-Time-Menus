#pragma once

#include "Menu.hpp"
#include "ListBox.hpp"
#include "HotKeyWheel.hpp"

class ItemChange;
class TileImage;
class TileText;

class InventoryMenu : public Menu		// 1002
{
public:
	InventoryMenu();
	~InventoryMenu() override;

	enum Filter : uint32_t
	{
		kWeapons,
		kApparel,
		kAid,
		kMisc,
		kAmmo
	};

	enum InventoryMenuHotkey
	{
		Inv_HotKeyWheel			= 0x5,
		Inv_Equip				= 0x6,
		Inv_Drop				= 0x7,
		Inv_Repair				= 0x8,
		Inv_HotkeyButton		= 0x9,
		Inv_Cancel				= 0xA,
		Inv_ModMenu				= 0x13,
	};

	enum Buttons
	{
		kButton_HotKeyWheel		= 0x5,
		kButton_Equip			= 0x6,
		kButton_Drop			= 0x7,
		kButton_Repair			= 0x8,
		kButton_HotkeyButton	= 0x9,
		kButton_Cancel			= 0xA,
		kButton_ModMenu			= 0x13,
	};

	union
	{
		Tile*					pTiles[23];
		struct
		{
			TileRect*			pTileCapsInfo;
			TileRect*			pTilePlayerHPInfo;
			TileRect*			pTilePlayerDRInfo;
			TileRect*			pTilePlayerWGInfo;
			TileImage*			pTileInventoryList;
			TileRect*			pTileHotKeyWheel;
			TileImage*			pTileEquipButton;
			TileImage*			pTileDropButton;
			TileImage*			pTileRepairButton;
			TileImage*			pTileHotkeyButton;
			TileImage*			pTileCancelButton;
			TileImage*			pTileItemIcon;
			TileRect*			pTileItemInfoRect;
			TileRect*			pTileTabline;
			TileRect*			pTileDAMInfo;
			TileRect*			pTileDPSInfo;
			TileRect*			pTileStrengthReqInfo;
			TileRect*			pTileDamResistInfo;
			TileRect*			pTileDamThresholdInfo;
			TileImage*			pTileModButton;
			TileImage*			pTileItemIconBadge;
			TileRect*			pTilePlayerDTInfo;
			TileText*			pTileStrReq;
		};
	};

	struct ScrollPos
	{
		int32_t					iListIndex;
		int32_t					iCurrentValue;
	};

	Filter						eFilter;
	ScrollPos					kTabScrollPositions[6];
	ListBox<ItemChange*>		kItemsList;
	HotKeyWheel					kHotkeyWheel;
	BSSimpleList<ItemChange*>	kChangedItemsList;

	static constexpr AddressPtr<uint32_t, 0x11D9EB4> uiValueTrait;

	static InventoryMenu*		GetSingleton();
};

ASSERT_SIZE(InventoryMenu, 0x124);