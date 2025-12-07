#pragma once

#include "BSMemObject.hpp"
#include "BSSimpleList.hpp"
#include "BSStringT.hpp"
#include "Gamebryo/NiFixedString.hpp"
#include "Gamebryo/NiTList.hpp"
#include "Tile.hpp"

class TileMenu;
class TileExtra;

enum MenuSpecialKeyboardInputCode
{
	kMenu_Enter = -2,
	kMenu_UpArrow = 0x1,
	kMenu_DownArrow = 0x2,
	kMenu_RightArrow = 0x3,
	kMenu_LeftArrow = 0x4,
	kMenu_Space = 0x9,
	kMenu_Tab = 0xA,
	kMenu_ShiftEnter = 0xB,
	kMenu_AltEnter = 0xC,
	kMenu_ShiftLeft = 0xD,
	kMenu_ShiftRight = 0xE,
	kMenu_PageUp = 0xF,
	kMenu_PageDown = 0x10,
};

class Menu {
public:
	virtual				~Menu();
	virtual void		AttachTileByID(uint32_t auiTileID, Tile* apTile);
	virtual void		DoDownClick(uint32_t auiTileID, Tile* apTarget);
	virtual void		DoClick(uint32_t auiTileID, Tile* apTarget);
	virtual void		DoEnter(uint32_t auiTileID, Tile* apTarget);
	virtual void		DoLeave(uint32_t auiTileID, Tile* apTarget);
	virtual void		DoDragEnter(uint32_t auiTileID, Tile* apNewTile, Tile* apTarget);
	virtual void		DoDragLeave(uint32_t auiTileID, Tile* apOldTile, Tile* apTarget);
	virtual void		DoStillDown(uint32_t auiTileID, Tile* apTarget);
	virtual void		DoStillDragOver(uint32_t auiTileID, Tile* apTarget);
	virtual void		DoWheelMove(uint32_t auiTileID, Tile* apTarget);
	virtual void		DoIdle();
	virtual bool		DoKey(uint32_t auiInputChar);
	virtual uint32_t	GetClass() const;
	virtual bool		DoGamepad(MenuSpecialKeyboardInputCode aeCode, float afKeyState);
	virtual bool		HandleControllerInput(int aiInput, Tile* apTile);
	virtual void		OnUpdateUserTrait(int aitileVal);
	virtual void		HandleControllerConnectOrDisconnect(bool abIsControllerConnected);

	enum FADE_STATE : int32_t {
		FADE_SHOWN		= 0x1,
		FADE_HIDDEN		= 0x4,
		FADE_NOT_HIDDEN = 0xB,

		FADING_IN		= 0x8,
		FADING_OUT		= 0x2,

		FADE_IN_SHOWN	= 0x9,
		FADE_OUT_SHOWN	= 0x3,

		FADE_ALL		= 0xF,
	};

	TileMenu*							pMenu;
	BSSimpleList<Tile::TileTemplate*>	kTemplates;
	Tile*								pLastTile;
	int32_t								iIsModal;
	int32_t								iMenuThickness;
	bool								bForceFadeOut;
	bool								bDeleteTemplates;
	int32_t								iClass;
	uint32_t							eFadeState;
};

ASSERT_SIZE(Menu, 0x28);