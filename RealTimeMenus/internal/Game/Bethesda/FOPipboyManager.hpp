#pragma once

#include "FORenderedMenu.hpp"
#include "BSSoundHandle.hpp"

class NiNode;
class NiAVObject;

class FOPipboyManager : public FORenderedMenu {
public:
	NiPointer<NiNode>		spPipboyScreenNode0D8;
	NiPointer<NiAVObject>	spNode_0DC;
	NiPointer<NiAVObject>	spUnk0C0;
	NiPointer<NiAVObject>	spUnk0C4;
	NiPointer<NiAVObject>	spPipboyLightButton[3];
	NiPointer<NiNode>		spPipboyLightGlow[3];
	NiPointer<NiAVObject>	spScrollKnobs[3];
	float					fUnk10C[3];
	float					fPipboyKnobScrollPositions[3];
	float					fPipboyKnobScrollRates[3];
	float					fTabKnobMinPosition;
	uint32_t				unk134;
	uint32_t				unk138;
	uint32_t				unk13C;
	float					fTabKnobMaxPosition;
	uint8_t					byte144;
	uint32_t				unk148;
	uint32_t				unk14C;
	uint8_t					byte150;
	BSSoundHandle			kPipBoyHumLoopSound;
	bool					byte160;
	bool					byte161;
	bool					byte162;
	float					fLightFadeStartTime;
	float					fLightEffectFadeDuration;
	uint8_t					bIsResetPipboyManager;
};

ASSERT_SIZE(FOPipboyManager, 0x170);