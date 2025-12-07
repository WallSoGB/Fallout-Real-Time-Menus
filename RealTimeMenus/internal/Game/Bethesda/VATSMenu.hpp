#pragma once

#include "Menu.hpp"
#include "ListBox.hpp"
#include "VATS.hpp"
#include "BSEnums.hpp"
#include "Gamebryo/NiPoint2.hpp"

class HitData;
class TileImage;
class TileText;
class TileRect;

struct VATSBodyPart {
	NiPoint2		kScreenPos;
	NiPoint3		kRelativePos;
	NiPoint3		kPos;
	BODY_PART_TYPE	eBodyPart;
	float			fPercentVisible;
	float			fHitChance;
	bool			bIsOnScreen;
	bool			bChanceCalculated;
	bool			bFirstTimeShown;
	bool			bNeedsRecalc;
	Tile*			pMeter;
	float			fAngleToStick;
	uint8_t			bLeftOfCenter;
};

struct VATSTarget {
	enum Type {
		NONE		= 0,
		ACTOR		= 1,
		PROJECTILE	= 2,
	};

	TESObjectREFR*					pReference;
	uint32_t						eType;
	BSSimpleList<VATSBodyPart*>		kBodyParts;
	bool							bSetUp;
	float							unk14;
	float							fAngle;
	float							fDistance;
	bool							bHostile;
};

class VATSMenu : public Menu {
public:
	VATSMenu();
	~VATSMenu();

	enum Buttons
	{
		UNK0						= 0,
		QUEUED_ACTIONS,
		ARROW_LEFT,
		ARROW_RIGHT,
		BUTTON_ACCEPT,
		BUTTON_RETURN,
		BUTTON_BODY_PART,
		BUTTON_SELECT,
		RIGHT_BRACKET,
		METER_0,
		METER_ACTION_POINTS,
		JUSTIFY_TEXT_RIGHT_0,
		MESSAGE_LOW_AP,
		AMMO_COUNT,
		CND_LABEL,
		CND_METER,
		CND_METER_BACKGROUND,
		LEFT_BRACKET,
		METER_1,
		JUSTIFY_RIGHT_TEXT,
		COMPASS_MARKERS,
		COMPASS_QUESTS,
		COMPASS_NPCS,
		COMPASS_WINDOW,
		WARNING_BACKGROUND,
		ENEMY_HEALTH_BRACKET,
		METER_2,
		METER_3,
		METER_4,
		JUSTIFY_CENTER_TEXT,
		BUTTON_SPECIAL_ATTACK,
		BUTTON_SPECIAL_ATTACK_2,
		TOTAL_BUTTONS
	};

	union
	{
		Tile*	pTiles[TOTAL_BUTTONS];
		struct
		{
			Tile*		pTile028;
			TileImage*	pTile02C;
			TileImage*	pTile030;
			TileImage*	pTile034;
			TileImage*	pTile038;
			TileImage*	pTile03C;
			TileImage*	pTile040;
			TileImage*	pTile044;
			TileImage*	pTile048;
			TileImage*	pTile04C;
			TileImage*	pTile050;
			TileText*	pTile054;
			TileText*	pTile058;
			TileText*	pTile05C;
			TileText*	pTile060;
			TileImage*	pTile064;
			TileImage*	pTile068;
			TileImage*	pTile06C;
			TileImage*	pTile070;
			TileText*	pTile074;
			TileRect*	pTile078;
			TileRect*	pTile07C;
			TileRect*	pTile080;
			TileImage*	pTile084;
			TileRect*	pTile088;
			TileImage*	pTile08C;
			TileImage*	pTile090;
			TileImage*	pTile094;
			TileImage*	pTile098;
			TileText*	pTile09C;
			TileImage*	pTile0A0;
			TileImage*	pTile0A4;
		};
	};

	Tile*					pActive;
	bool					bDebounceMenuButton;
	ListBox<uint32_t>		kQueuedActions;
	float					fActionPoints;
	float					fActionPointsBase;
	float					fClipAmmo;
	float					fTotalAmmo;
	float					fQueuedClipAmmo;
	float					fQueuedTotalAmmo;
	bool					bHeadingReached;
	bool					bMetersShown;
	bool					bTargetsLit;
	bool					bClosing;
	float					fProbableEnemyDamage;
	VATSBodyPart*			pCurrentBodyPart;
	bool					bConcentratedFirePerk;
	VATSBodyPart*			pLastLimb;
	bool					bStickReset;
	bool					bTargetStickReset;
	bool					bMouseOverButton;
	bool					bSelectLeftHeld;
	bool					bSelectRightHeld;
	uint32_t				uiSelectTimer;
	bool					bDataInTemporaryVATSCommand;
	VATS_COMMAND			kCurrentCommand;
	
	static constexpr AddressPtr<BSSimpleList<VATSTarget*>, 0x11DB150> kTargets;

	static VATSMenu* GetSingleton() { return *reinterpret_cast<VATSMenu**>(0x11DB0D4); };
	static TESObjectREFR* GetTargetReference() { return *reinterpret_cast<TESObjectREFR**>(0x11F21CC); };
	
	BSSimpleList<VATSTarget*>* FindTarget(TESObjectREFR* apRef);
};

ASSERT_SIZE(VATSMenu, 0x144);
