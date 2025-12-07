#pragma once

#include "BaseFormComponent.hpp"
#include "BSStringT.hpp"

class TESFullName : public BaseFormComponent {
public:
	TESFullName();
	~TESFullName();

	BSString	strFullName;
};

ASSERT_SIZE(TESFullName, 0xC);