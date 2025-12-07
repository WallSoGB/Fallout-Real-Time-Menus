#pragma once

#include "Bethesda/Menu.hpp"
#include "Bethesda/BSSoundHandle.hpp"

class Actor;

class CompanionWheelMenu : public Menu {
public:
	enum Buttons {
		kAggressivePassive = 0x0,
		kUseStimpak = 0x1,
		kStayFollow = 0x2,
		kTalkTo = 0x3,
		kBackUp = 0x4,
		kNearFar = 0x5,
		kOpenInventory = 0x6,
		kRangedMelee = 0x7,
		kExit = 0xB,

		CWM_RadialLabel = 0x8,
		CWM_ButtonPreviewText = 0x9,
		CWM_ButtonContextText = 0xA,
		CWM_Subtitle = 0xC,
		CWM_Callout_Exit = 0xD,
		CWM_Callout_Select = 0xE,
		CWM_Callout_Navigate = 0xF,
	};

	Tile*			pTiles[16];
	Actor*			pCompanionRef;
	bool			bIsIsFollowingLong;
	bool			bIsFollowerSwitchAggressive;
	bool			bIsCombatStyleRanged;
	bool			bIsWaiting;
	uint32_t		uiLastSelectedTileID;
	bool			bTalkToActorWhenClosingMenu;
	uint32_t		time78;
	BSSoundHandle	kResponse;

	static CompanionWheelMenu* GetSingleton();
};

ASSERT_SIZE(CompanionWheelMenu, 0x88);