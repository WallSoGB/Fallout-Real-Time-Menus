#pragma once

#include "BSSoundHandle.hpp"
#include "ListBox.hpp"
#include "Menu.hpp"
#include "BGSTerminal.hpp"
#include "Tile.hpp"
#include "HackingText.hpp"

class ComputersMenu : public Menu {
public:
	ComputersMenu();
	~ComputersMenu();

	enum Buttons
	{
		kComputersServer = 0x0,
		kComputersFileFirectoryListbox = 0x1,
		kComputersDisplayZone = 0x2,
		kComputersHeader1 = 0x3,
		kComputersHeader2 = 0x4,
		kComputersResultText = 0x5,
		kComputersCursor = 0x6,
		kComputersWelcome = 0x7,
		kComputersResultPrompt = 0x8,
		kComputersIntroWelcome = 0x9,
		kComputersIntroLogon = 0xA,
		kComputersIntroEnterPW = 0xB,
		kComputersIntroPWDisplay = 0xC,
		kComputersBackground = 0xD,
		kComputersDisplayZoneText = 0xE,
		kComputersSeparator = 0xF,
	};


	Tile*							pTile_computers_server;
	Tile*							pTile_computers_file_directory;
	Tile*							pTile_computers_display_zone;
	Tile*							pTile_computers_header1;
	Tile*							pTile_computers_header2;
	Tile*							pTile_computers_result_text;
	Tile*							pTile_computers_cursor;
	Tile*							pTile_computers_welcome;
	Tile*							pTile_computers_result_prompt;
	Tile*							pTile_computers_intro_welcome;
	Tile*							pTile_computers_intro_logon;
	Tile*							pTile_computers_intro_enterpw;
	Tile*							pTile_computers_intro_pwdisplay;
	Tile*							pTile_computers_background;
	Tile*							pTile_computers_display_zone_text;
	Tile*							pTile_computers_separator;
	ListBox<TERMINAL_MENU_ITEM*>	kTerminalEntries;
	BSSimpleList<BGSTerminal*>		kSubMenus;
	uint32_t						time0A0;
	Tile::Value*					pIsComputersCursorVisible;
	BGSTerminal*					pTargetRefBaseForm;
	TESObjectREFR*					pTargetRef;
	uint32_t						uiStage;
	BSString						strNoteText;
	const char*						pRemainingNoteText; // pointer into the current note text, moves as you change page 
	uint8_t							byte0C0;
	bool							bIsRenderingSubmenu;
	BSSoundHandle					kFanHumSound;
	BSSoundHandle					kCharScrollSound;
	BSSoundHandle					kCharEnterSound;
	BSSimpleList<HackingText>*		kHackingTexts;
	uint32_t						pPlayNoteAudioCallback;
	uint32_t						pShowNoteTextCallback;
	uint32_t						pShowNoteImageCallback;
	uint32_t						pUnk0F8;

	static ComputersMenu* GetSingleton();
};

ASSERT_SIZE(ComputersMenu, 0xFC);