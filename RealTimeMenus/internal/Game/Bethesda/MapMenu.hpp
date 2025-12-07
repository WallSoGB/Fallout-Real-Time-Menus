#pragma once

#include "Menu.hpp"
#include "BSSoundHandle.hpp"
#include "ListBox.hpp"
#include "Gamebryo/NiPoint2.hpp"

class TESForm;
class TESQuest;
class TESObjectREFR;
class TESChallenge;
class TESObjectCELL;
class TESWorldSpace;
class TileImage;
class BGSNote;
class BGSQuestObjective;

class MapMenu : public Menu {
public:
	MapMenu();
	~MapMenu();

	enum MapMenuTabs {
		TAB_LOCAL_MAP	= 0x20,
		TAB_WORLD_MAP	= 0x21,
		TAB_QUESTS		= 0x22,
		TAB_MISC		= 0x23,
		TAB_RADIO		= 0x24,
	};

	union {
		Tile*			pTiles[21];
		struct {
			Tile*	pLocationInfo;
			Tile*	pTimeDateInfo;
			Tile*	pLocalMapParentImage;
			Tile*	pLocalMapCursor;
			Tile*	pWorldMapParentImage;
			Tile*	pWorldMapCursor;
			Tile*	pMapSelectionRectangle;
			Tile*	pQuestsList;
			Tile*	pNotesList;
			Tile*	pSelectedNoteHighlight;
			Tile*	pRadioStationList;
			Tile*	pButtonA;
			Tile*	pButtonX;
			Tile*	pDataRect;
			Tile*	pDataTextRect;
			Tile*	pQuestObjectivesList;
			Tile*	pWaveform;
			Tile*	pTabline;
			Tile*	pButtonY;
			Tile*	pChallengesList;
			Tile*	pChallengesListSelectedHighlight;
		};
	};

	void*							pChallengeType;
	uint32_t						uiCurrentTab;
	TileImage*						pTileWorldMap;
	float							fLastRadioAnimationUpdateTime;
	bool							bIsShowAllNotes;
	BGSNote*						pCurrentNote;
	uint32_t						uiTimeNoteViewed;
	BSSimpleList<BSSoundHandle>		kHolotapeDialogues;
	BSSimpleArray<BSString>			kHolotapeSubtitles;
	BSSoundHandle*					pMaybeHolotapeSound;
	bool							bIsHolotapeVoicePlaying;
	float							fHolotapeTotalTime;
	uint32_t						uiHolotapePlayStartTime;
	float							fRadioVolume;
	TESQuest*						pSelectedQuest;
	BGSQuestObjective*				pCurrentObjective;
	TileImage*						pMapMarker;
	BSSimpleList<TESObjectREFR*>	kMapMarkerList;
	BSSimpleList<TESObjectREFR*>	kDoorList;
	NiPoint2						kCornerNWPos;
	NiPoint2						kCornerSEPos;
	TESForm*						pMarkerForm;
	NiPoint3						kMarkerPos;
	TESObjectCELL*					pCurrentCellOrWorldspace;
	TESWorldSpace*					pParentmostLastExtDoorWorldspace;
	bool							bCurrentWorldspaceHasNoParent;
	TESObjectREFR*					pLastExtDoor;
	TESObjectREFR*					pSelectedMarker;
	TESObjectCELL*					pCell11C;
	NiPoint2						kClickStartPos;
	uint32_t						unk128;
	bool							bFogOfWar;
	ListBox<TESQuest>				kQuestList;
	ListBox<BGSNote>				kNoteList;
	ListBox<TESObjectREFR>			kRadioRefList;
	ListBox<BGSQuestObjective>		kObjectiveList;
	ListBox<TESChallenge>			kChallengeList;
	BSSimpleArray<Tile>				arr220;

	static MapMenu* GetSingleton();

	void AddQuestMarkers();
	void AddCompanionMarkers();
};

ASSERT_SIZE(MapMenu, 0x230);