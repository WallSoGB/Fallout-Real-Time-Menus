#pragma once

#include "TESObjectACTI.hpp"
#include "TESCondition.hpp"
#include "TESDescription.hpp"
#include "Script.hpp"

class BGSNote;
class ScriptLocals;
class BGSTerminal;

struct TERMINAL_MENU_ITEM {
	struct ALIGN1 _MenuEntryFlags {
		enum Flags : uint8_t {
			ADD_NOTE		= 1u << 0,
			FORCE_REDRAW	= 1u << 1,
		};

		bool bAddNote : 1;
		bool bForceRedraw : 1;
	};
	using MenuEntryFlags = _MenuEntryFlags::Flags;

	BSString					strEntryText;
	BSString					strResultText;
	Script						kResScript;
	TESCondition				kConditions;
	BGSNote*					pDisplayNote;
	BGSTerminal*				pSubMenu;
	Bitfield<_MenuEntryFlags>	ucFlags;
};

class BGSTerminal : public TESObjectACTI, public TESDescription {
public:
	BGSTerminal();
	~BGSTerminal();

	struct ALIGN1 _TerminalFlags {
		enum Flags : uint8_t {
			LEVELED			= 1u << 0,
			UNLOCKED		= 1u << 1,
			ALT_COLORS		= 1u << 2,
			HIDE_WELCOME	= 1u << 3,
		};

		bool bLeveled		: 1;
		bool bUnlocked		: 1;
		bool bAltColors		: 1;
		bool bHideWelcome	: 1;
	};
	using TerminalFlags = _TerminalFlags::Flags;

	struct Data {
		uint8_t						ucDifficulty;       // 0: very easy, 1: easy, 2: average, 3: hard, 4: very hard, 5: requires key
		Bitfield<_TerminalFlags>	ucFlags;
		uint8_t						ucType;             // 0-9, corresponds to GECK types 1-10
	};

	BSSimpleList<TERMINAL_MENU_ITEM*>	kMenuEntries;
	BGSNote*							pPassword;
	Data								kData;
};

ASSERT_SIZE(BGSTerminal, 0xA8);
ASSERT_SIZE(TERMINAL_MENU_ITEM, 0x78);