#pragma once

#include "LowProcess.hpp"

class MiddleLowProcess : public LowProcess {
public:
	MiddleLowProcess();
	~MiddleLowProcess();

	virtual bool	ProcessCombatOneHour(Actor* apActor, float);

	uint32_t		uiHourPackageEvaluated;
	ModifierList	kMagicModifiers;
};

ASSERT_SIZE(MiddleLowProcess, 0xC8);