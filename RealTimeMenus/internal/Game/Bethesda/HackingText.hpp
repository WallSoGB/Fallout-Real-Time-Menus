#pragma once

#include "BSStringT.hpp"
#include "Tile.hpp"

struct HackingText {
	enum HackingTextFlags {
		CHARACTER_BY_CHARACTER	= 1u << 0,
		BLINKS					= 1u << 1,
	};

	BSString		strText;
	Tile*			pTileText;
	Tile::Value*	pTileValueVisibility;
	uint32_t		uiDisplayRate;
	uint32_t		uiFlashOnDuration;
	uint32_t		uiFlashOffDuration;
	uint32_t		uiNextDisplayTime;
	uint32_t		uiNextBlinkTime;
	char*			pCurrentlyDisplayedText;
	Bitfield8		ucFlags;
	bool			bIsTextRemaining;
};

ASSERT_SIZE(HackingText, 0x2C);