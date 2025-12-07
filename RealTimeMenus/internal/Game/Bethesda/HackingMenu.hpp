#pragma once

#include "Menu.hpp"
#include "Tile.hpp"
#include "BSSoundHandle.hpp"
#include "HackingText.hpp"

class TileImage;
class TESObjectREFR;

class HackingMenu : public Menu {
public:
	HackingMenu();
	~HackingMenu();

	enum Stages {
		kFailed		= 3,
		kSuccess	= 4
	};

	enum
	{
		kHackingMenu_NumHeaders = 4, // the number of lines printed above the hacking minigame (ROBCO INDUSTRIES, ENTER PASSWORD NOW etc.)
		kHackingMenu_LineLength = 0xC,
		kHackingMenu_AddressLength = 7, // e.g. '0xF688 '
		kHackingMenu_EmptyPWord = 0x198,
	};

	struct _Sounds {
		enum Sounds : uint32_t {
			FAN_HUM			= 0,
			HACKING_PASS	= 1,
			HACKING_BAD		= 2,
			CHAR_SCROLL		= 3,
			CHAR_ENTER		= 4,
			TOTAL_COUNT
		};
	};
	using Sounds = _Sounds::Sounds;

	struct Entry {
		HackingText kText;
		Tile*		pTile;
		Tile*		pTemplateTile;
	};

	Entry*				pLines[39];
	uint32_t			unk0C4[7];
	HackingText			kBottomLine;
	Tile*				pHacking_intro_rect;
	Tile*				pHacking_header_rect;
	Tile*				pHacking_password_file_rect;
	Tile*				pHacking_password_log_rect;
	Tile*				pHacking_cursor;
	Tile*				pHacking_password_entry;
	Tile*				pHacking_locked_message_text_1;
	Tile*				pHacking_locked_message_text_2;
	BSSoundHandle		kSounds[Sounds::TOTAL_COUNT];
	uint32_t			printedLines_TileChildList[3];
	uint32_t			tileChildList174[3];
	BSSimpleList<char*> kWords;
	uint32_t			tileChildList188[3];
	TESObjectREFR*		pTargetBaseRef;
	TESObjectREFR*		pTargetRef;
	char*				pCorrectAnswer;
	uint32_t			uiAttemptsRemaining;
	uint32_t			uiMaxAttemptsRemaining;
	uint32_t			uiNumWordsRemaining;
	uint32_t			uiCorrectAnswerLength;
	uint32_t*			pWordPositions;
	uint32_t			uiTopTextHeight;
	uint32_t			uiTopTextLineHeight;
	uint32_t			uiHighlightedCharacterXIndex;
	uint32_t			uiActiveTile;
	uint32_t			uiStage;
	TileImage*			pHacking_password_file_target;
	uint32_t			uiControllerSelectedTileIdx;
	uint32_t			uiTransitionToComputersMenuTime;
	Tile::Value*		pIsComputersCursorVisible;
	bool				bShouldNotHighlightWordOnHover;
	bool				bHasAllowanceBeenReplenished;
	bool				bHasShownHelp;

	static HackingMenu* GetSingleton();
};

ASSERT_SIZE(HackingMenu, 0x1DC);