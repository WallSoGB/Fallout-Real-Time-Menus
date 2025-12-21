#pragma once

class FORenderedMenu;
class FOPipboyManager;
class VATSEffect;
class TESObjectREFR;

typedef void(*MessageBoxCallback)();

class Interface {
public:
	enum Emotion : uint32_t {
		NEUTRAL	= 0,
		HAPPY	= 1,
		SAD		= 2,
		PAIN	= 3,
	};

	enum Menus {
		NoMenu					= 0,
		MainFour				= 1,
		OtherRoot				= 2,
		Console					= 3,

		PipBoy					= MainFour,

		Message					= 1001,
		Inventory				= 1002,
		Stats					= 1003,
		HUDMainMenu				= 1004,
		Loading					= 1007,
		Container				= 1008,
		Dialog					= 1009,
		SleepWait				= 1012,
		Pause					= 1013,
		LockPick				= 1014,
		Quantity				= 1016,
		AudioMenu				= 1017,
		VideoMenu				= 1018,
		GamePlayMenu			= 1020,
		PipboyData				= 1023,
		BookMenu				= 1026,
		LevelUp					= 1027,
		PipboyRepair			= 1035,
		RaceMenu				= 1036,
		SurgeryMenu				= 1036,
		BarberMenu				= 1036,
		Credits					= 1047,
		CharGen					= 1048,
		TextEdit				= 1051,
		Barter					= 1053,
		Surgery					= 1054,
		Hacking					= 1055,
		VATS					= 1056,
		Computers				= 1057,
		VendorRepair			= 1058,
		Tutorial				= 1059,
		SPECIALBook				= 1060,
		ItemModMenu				= 1061,
		LoveTester				= 1074,
		CompanionWheel			= 1075,
		MedicalQuestionnaire	= 1076,
		Recipe					= 1077,
		SlotMachine				= 1080,
		BlackJack				= 1081,
		Roulette				= 1082,
		Caravan					= 1083,
		Traits					= 1084,

		MenuIDCount,
	};

	static bool IsInMenuMode();
	static bool IsMenuIDVisible(Menus aeMenu, uint32_t auiFlags);
	static bool IsTopMenuID(Menus aeMenu);
	static bool IsPipBoyMenuOpen();
	static bool IsPipBoyOpening();
	static bool IsPipBoyOpened();
	static bool IsInPipBoy();
	static bool IsInRenderedMenu();
	static bool IsDebugTextVisible();
	static bool IsConsoleVisible();
	static bool IsInDialog();
	static Menus GetTopMenuID();
	static TESObjectREFR* GetActivateTarget();
	static TESObjectREFR* GetDialogueTarget();
	static FORenderedMenu* GetCurrentRenderedMenu();
	static FOPipboyManager* GetPipboyManager();
	static VATSEffect* GetVATSEffect();
	static void SetMenuSystemVisible(bool abVisible);
};