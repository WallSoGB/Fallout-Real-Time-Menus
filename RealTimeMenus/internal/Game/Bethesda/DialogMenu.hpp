#pragma once

#include "Menu.hpp"
#include "ListBox.hpp"
#include "BSSoundHandle.hpp"

class TESTopic;
class TESTopicInfo;
class ImageSpaceModifierInstance;
class MenuTopicManager;
class Actor;
class TESObjectREFR;

class DialogMenu : public Menu {
public:
	DialogMenu();
	~DialogMenu();

	enum TopicType {
		TOPIC_GOODBYE	= 1,
		INFO_GOODBYE	= 2,
	};

	enum Stage {
		CAMERA_ZOOM_IN		= 0,
		LOAD_SPEECH			= 1,
		SPEECH_IDLE			= 2,
		PLAY_SPEECH			= 3,
		CAMERA_ZOOM_OUT		= 4,
		SERVICE_FADE_OUT	= 5,
	};

	uint32_t					eStage;
	uint32_t					eTopicType;
	Tile*						pTiles[4];
	ListBox<int32_t>			kTopics;
	MenuTopicManager*			pMenuTopicManager;
	BSSoundHandle				kCurrentSound;
	TESObjectREFR*				pTargetRef;
	char						cTopicDebug[128];
	bool						bHasResponses;
	float						fTime;
	int							lastMenuClickTime;
	BSSimpleList<TESTopicInfo*>	infos110;
	BSSimpleList<TESTopicInfo*>	infos118;
	BSSimpleList<TESTopicInfo*>	infos120;
	float						fZoomProgress;
	ImageSpaceModifierInstance* pImageSpaceDOF;
	bool						bWasPlayerThirdPerson;
	bool						bPaused;
	uint32_t					eStoredStage;
	bool						bInBackground;
	bool						bShouldShowHelpMessage;
	bool						byte13A;
	char						byte13B;

	static constexpr AddressPtr<bool, 0x11D9514> bIsOpen;

	static DialogMenu* GetSingleton();

	static bool IsOpen();
};

ASSERT_SIZE(DialogMenu, 0x13C);
