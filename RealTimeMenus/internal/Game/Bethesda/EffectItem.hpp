#pragma once

#include "BSStringT.hpp"
#include "TESCondition.hpp"
#include "MagicSystem.hpp"

class Actor;
class EffectSetting;

struct EffectItemData {
	uint32_t			uiMagnitude;
	uint32_t			uiArea;
	uint32_t			uiDuration;
	MagicSystem::Range	eRange;
	uint32_t			uiActorValue;
};

class EffectItem {
public:
	EffectItem();
	~EffectItem();

	struct ScriptEffectInfo {
		uint32_t	uiScriptRefID;
		uint32_t	uiSchool;
		BSString	strEffectName;
		uint32_t	uiVisualEffectCode;
		uint32_t	bIsHostile;
	};

	EffectItemData	kData;
	EffectSetting*	pEffectSetting;
	float			fRawCost;
	TESCondition	kConditions;
};

ASSERT_SIZE(EffectItem, 0x24);