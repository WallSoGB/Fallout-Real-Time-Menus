#pragma once

#include "Menu.hpp"
#include "ListBox.hpp"
#include "BSSoundHandle.hpp"

class ItemChange;
class TileImage;
class TileText;
class TileRect;
class TESForm;
class TESObjectREFR;

class ContainerMenu : public Menu {
public:
	ContainerMenu();
	~ContainerMenu();

	enum Mode : uint32_t
	{
		kNormal = 0x1,
		kPickpocket,
		kTeammate,
		kRockItLauncher
	};

	enum Buttons
	{
		CM_Items_LeftFilterArrow = 0x0,
		CM_ItemsTitle = 0x1,
		CM_Items_RightFilterArrow = 0x2,
		CM_Items_CapsLabel = 0x3,
		CM_Items_InventoryList = 0x4,
		CM_Container_LeftFilterArrow = 0x5,
		CM_ContainerTitle = 0x6,
		CM_Container_RightFilterArrow = 0x7,
		CM_Container_InventoryList = 0x8,
		CM_ArrowIcon = 0x9,
		CM_TakeAllButton = 0xA,
		CM_ExitButton = 0xB,
		CM_ItemIcon = 0xC,
		CM_ItemData = 0xD,
		CM_list_template_container = 0x14,
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

	union
	{
		Tile*					pTiles[19];
		struct
		{
			TileImage*			pTile028;
			TileText*			pItemsTitle;
			TileImage*			pTile030;
			TileText*			pTileInventoryWeight;
			TileImage*			pTile038;
			TileImage*			pTile03C;
			TileText*			pContainerTitle;
			TileImage*			pTile044;
			TileImage*			pTile048;
			TileImage*			pTile04C;
			TileImage*			pTakeAllTile;
			TileImage*			pTile054;
			TileImage*			pTile058;
			TileRect*			pTile05C;
			TileRect*			pTile060;
			TileRect*			pTile064;
			TileRect*			pTile068;
			TileRect*			pTile06C;
			TileRect*			pTile070;
		};
	};
	TESObjectREFR*				pContainer;
	BSSimpleList<void*>			list078;
	Mode						eMode;
	uint32_t					uiValueTransferred;
	uint8_t						bHasPickedPocket;
	uint8_t						bHasFailedPickpocket;
	uint8_t						pad08A;
	uint8_t						pad08B;
	Filter						eLeftFilter;
	Filter						eRightFilter;
	uint32_t					uiMenuSoundID;
	ListBox<ItemChange*>		kLeftItems;
	ListBox<ItemChange*>		kRightItems;
	ListBox<ItemChange*>*		pCurrentItems;
	uint32_t					unk0FC;
	BSSoundHandle				kMenuSound;

	static ContainerMenu*	GetSingleton();
	static ItemChange*		GetSelection();
	static void				SetSelection(ItemChange* apItem);
	static void				Close();

	void 			Refresh(TESForm* apForm = nullptr);
};

ASSERT_SIZE(ContainerMenu, 0x10C);