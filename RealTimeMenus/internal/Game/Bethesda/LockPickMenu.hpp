#pragma once

#include "Menu.hpp"
#include "Gamebryo/NiQuaternion.hpp"

class NiControllerSequence;
class TileRect;
class TileImage;
class TileText;
class TESObjectREFR;

class LockPickMenu : public Menu {
public:
	~LockPickMenu();
	LockPickMenu();

	uint32_t				uiStage;
	TileRect*				pTile2C;
	TileRect*				pLockPickCount;
	TileRect*				pTile34;
	TileImage*				pTile38;
	TileText*				pTile3C;
	TileImage*				pTile40;
	TileImage*				pTile44;
	TileText*				pTile48;
	TileImage*				pTile4C;
	TileImage*				pTile50;
	TileImage*				pTile54;
	TileText*				pTile58;
	TileText*				pTile5C;
	TileImage*				pTile60;
	TileImage*				pTile64;
	TileImage*				pTile68;
	TESObjectREFR*			pLockRef;
	uint32_t				uiLockLevel;
	uint32_t				uiSkillLevel;
	float					fSkillLevel;
	uint32_t				unk7C;
	uint32_t				unk80;
	float					flt84;
	uint32_t				uiSweetSpotLen;
	float					fCylinderAngle;
	float					fPickAngle;
	float					fPinHealth;
	bool					bSsForceRotate;
	bool					bShouldShowHelp;
	NiControllerSequence*	pLockForwardSequence;
	NiControllerSequence*	pLockBackwardSequence;
	NiControllerManager*	pLockPickCtrlManager;
	NiControllerSequence*	pLockPickForwardSequence;
	NiControllerSequence*	pLockPickBackwardSequence;
	NiControllerSequence*	pLockPickLeftSequence;
	NiMatrix3				kLockPickRotation;
	NiPoint3				kLockPickPosition;

	static LockPickMenu* GetSingleton();

	static void Close();
};

ASSERT_SIZE(LockPickMenu, 0xE4);