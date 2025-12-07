#pragma once

class TileRect;

class HotKeyWheel {
public:
	TileRect*	pParent;
	TileRect*	pHotkeys[8];
	uint8_t		byte24;
	uint32_t	uiSelectedHotkey;
	uint32_t	uiSelectedHotkeyTrait;
	uint32_t	uiSelectedTextTrait;
};

ASSERT_SIZE(HotKeyWheel, 0x34);