#pragma once

#include "ExtraDataList.hpp"

class Actor;
class TESBoundObject;

class ItemChange {
public:
	ItemChange();
	ItemChange(TESBoundObject* apForm, int32_t aiDelta = 0);
	~ItemChange();

	BSSimpleList<ExtraDataList*>*	pExtraLists;
	int32_t							iCountDelta;
	TESBoundObject*					pObject;
	
	enum {
		kHotkeyMin = 0,
		kHotkey0 = kHotkeyMin,
		kHotkey1,
		kHotkey2,
		kHotkey3,
		kHotkey4,
		kHotkey5,
		kHotkey6,
		kHotkey7,
		kHotkeyMax = kHotkey7,
		kHotkey8,
		kHotkeyStewie = kHotkey8
	};
};

ASSERT_SIZE(ItemChange, 0xC);