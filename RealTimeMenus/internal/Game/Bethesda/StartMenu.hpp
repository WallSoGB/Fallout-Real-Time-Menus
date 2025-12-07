#pragma once

#include "Menu.hpp"
#include "ListBox.hpp"

class TileImage;
class TileText;
class Tile3D;
class BGSSaveLoadFileEntry;
class NiSourceTexture;

class StartMenu : public Menu {
public:
	StartMenu();
	virtual ~StartMenu();

	enum StartMenuFlags {
		kInStartMenu = 0x1,
		kHasChangedSettings = 0x2,
		kLoad = 0x4,
		kIsSaveMenuNotLoad = 0x8,
		kIsMainMenuShown = 0x10,
		kAreOptionsInitialised = 0x20,
		kShowDLCPopup = 0x400,
		kIsActionMapping = 0x1000,
		kShowCredits = 0x10000,
		kControllerDisconnected = 0x20000,
		kSomethingSave = 0x40000000,
		kShowMustRestartToSaveSettings = 0x400000,
		ksomething_credits = 0x2000000,
		kDeleteSave = 0x2000,
	};

	class Option {
	public:
		Option(const char* apName, void(__cdecl* apCallback)(), uint32_t auiFlags);
		virtual ~Option() {};

		const char*		sName;
		void(__cdecl*	pFollowupOption)();
		uint32_t		uiFlags;
	};

	class UserOption : public Option {
	public:
		enum OPTION_TYPE : uint32_t {
			TOGGLE_CYCLES = 0x0,
			TOGGLE_NO_CYCLE = 0x1,
			SLIDER = 0x2,
		};

		UserOption(const char* apName, void(__cdecl* apCallback)(StartMenu::UserOption*), uint32_t auiFlags, OPTION_TYPE aeType, uint32_t auiOptionCount, uint32_t auiWidth, ...);
		virtual ~UserOption();

		const char*		sTemplateName;
		uint32_t		uiCurrValue;
		uint32_t		unk18;
		OPTION_TYPE		eType;
		uint32_t		uiNumOptions;
		uint32_t		uiButtonWidth;
		const char**	ppCurrValueName;
		void(__cdecl*	pOnSelection)(StartMenu::UserOption*);
	};

	class OptionListBox : public ListBox<StartMenu::Option*> {

	};

	Tile*							pMainMenuOptionsRoot;
	TileImage*						pTile02C;
	TileImage*						pTile030;
	TileImage*						pTile034;
	TileImage*						pTile038;
	TileImage*						pTile03C;
	TileImage*						pTile040;
	TileImage*						pTile044;
	TileText*						pTile048;
	TileText*						pTile04C;
	TileText*						pVersionString;
	TileImage*						pTileSaveImage;
	TileText*						pMM_PlayerPlacedMarker;
	Tile3D*							pTile05C;
	TileImage*						pTile060;
	TileImage*						pTile064;
	TileText*						pTileActionMappingDeviceName;
	TileImage*						pTile06C;
	TileText*						pTile070;
	TileText*						pTile074;
	TileText*						pTile078;
	TileImage*						pTile07C;
	TileText*						pTileSaveVersionNumber; // 080
	OptionListBox					kMainMenuOptions;
	OptionListBox					kSettingsSubMenu;
	OptionListBox					kOptionsNewGameMenu;
	OptionListBox					kActionMappingOptions;
	ListBox<int>					kListBox144;
	ListBox<BGSSaveLoadFileEntry*>	kSavesList;
	uint32_t						unk1A4;
	Bitfield32						uiFlags;
	float							scrolledLoadedSavesHeight;
	uint32_t						unk1B0;
	void*							brightnessSettingCallback;
	NiSourceTexture*				spScreenshotFrame;
	uint32_t						unk1BC;
	TileImage*						tile1C0;
	TileImage*						tile1C4;
	uint32_t						currentControlsDevice;
	uint32_t						unk1CC;
	uint32_t						uiEnabledTrait;

	static StartMenu* GetSingleton();

	static bool MainMenuIsActive();
	
	static bool PauseMenuIsActive();
};

ASSERT_SIZE(StartMenu::Option, 0x10);
ASSERT_SIZE(StartMenu::UserOption, 0x30);