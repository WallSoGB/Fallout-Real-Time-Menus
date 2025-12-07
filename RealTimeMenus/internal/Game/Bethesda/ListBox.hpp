#pragma once
// TODO REWORK THIS
#include "BSSimpleList.hpp"
#include "ListBoxBase.hpp"

template<typename Item> 
struct ListBoxItem {
	Tile*	pTile;
	Item	item;
	uint8_t	byte08;

	ListBoxItem(Item* aitem, Tile* apTile = nullptr) : pTile(apTile), item(aitem) {}
};

ASSERT_SIZE(ListBoxItem<void*>, 0xC);

template <typename Item> 
class ListBox : public ListBoxBase, BSSimpleList<ListBoxItem<Item*>*> {
public:
	ListBox();
	virtual	~ListBox();
	virtual void	RemoveAll() {};
	virtual void	Sort(int32_t(__cdecl*)(ListBoxItem<Item>*, ListBoxItem<Item>*)) {};

	enum Flags : uint16_t
	{
		kInitiated = 1u << 0,
		kRecalculateHeightsOnInsert = 1u << 1,
		kFreeContChangeOnListItemDestruction = 1u << 2, // assumes the object is a InventoryChanges - do not set this if the object isn't one...
	};

	Tile*			pTile;
	Tile*			pSelected;
	Tile*			pScrollBar;
	const char*		pcTemplateName;
	uint16_t		usItemCount;
	float			fSumY;
	float			fStoredListIndex;
	float			fStoredScrollbarPos;
	Flags			eFlags;
};

ASSERT_SIZE(ListBox<void*>, 0x30);