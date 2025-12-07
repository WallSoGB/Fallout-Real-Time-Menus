#pragma once

enum KeyboardMenuInputCode : uint32_t;
class Tile;
class Menu;

class ListBoxBase {
public:
	virtual bool	SetSelectedTile(Tile* apkTile);
	virtual Tile*	GetSelectedTile();
	virtual Tile*	HandleKeyboardInput(KeyboardMenuInputCode aeCode);
	virtual bool	IsMenuEqual(Menu* apkMenu);
	virtual void	ScrollToHighlight();
	virtual Tile*	GetTileByIndex(int aiIndex, bool abIsNotTileListIndex);
};

ASSERT_SIZE(ListBoxBase, 0x4);