#pragma once

#include "ActorPackageData.hpp"

class TESPackage;
class TESObjectREFR;

class ActorPackage {
public:
	TESPackage*			pPackage;
	ActorPackageData*	pData;
	TESObjectREFR*		pTarget;
	int32_t				iCurrentProcedureIndex;
	float				fPackageStartTime;
	Bitfield32			uiFlags;
};

ASSERT_SIZE(ActorPackage, 0x18);