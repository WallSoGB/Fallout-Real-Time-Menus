#pragma once

#include "BSSimpleList.hpp"

class FastModifierCollection;
class Modifier;

struct ModifierList : public BSSimpleList<Modifier*> {
public:
	ModifierList();
	~ModifierList();

	enum ClampStyle : uint32_t {
		CLAMP_ABOVE_ZERO = 0, // Clamps values > 0 to 0
		CLAMP_BELOW_ZERO = 1, // Clamps values < 0 to 0
		CLAMP_NONE		 = 2, // No clamping
	};

	bool					bAllowEmpty;
	FastModifierCollection* pFastModifiers;
};

ASSERT_SIZE(ModifierList, 0x10);