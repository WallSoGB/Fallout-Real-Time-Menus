#pragma once

#include "Gamebryo/NiSmartPointer.hpp"
#include "Gamebryo/NiRenderer.hpp"
#include "BSEnums.hpp"

class BSOcclusionQuery;
class BSRenderedTexture;
class NiRefObject;
class NiNode;
class NiTexture;
class TESObjectREFR;

class VATSEffectTarget {
public:
	TESObjectREFR*		pRef;
	NiPointer<NiNode>	spNode;
};

class VATSEffect {
public:
	enum QueryType {
		LIMB_QUERY_NONE		= 0,
		LIMB_QUERY_FULL		= 1,
		LIMB_QUERY_PARTIAL	= 2,
	};

	enum Mode {
		TARGET	= 0,
		SCAN	= 1,
	};

	uint32_t						eMode;
	VATSEffectTarget				kCurrentTarget;
	int32_t							iTargetCount;
	int32_t							iHighlightedTarget;
	VATSEffectTarget				kAllTargets[32];
	NiPointer<NiRefObject>			spUnk114;
	bool							bIsOcclusionEnabled;
	bool							bUnused119[16];
	bool							bUnk12B;
	BSOcclusionQuery*				pPartialLimbQuery[16];
	BSOcclusionQuery*				pEntireLimbQuery[16];
	bool							bHasExecutedQuery[16];
	uint32_t						uiTotalArea[16];
	uint32_t						uiVisibleArea[16];
	BODY_PART_TYPE					eSelectedLimb;
	uint32_t						uiHighlightedLimbCount;
	uint32_t						uiHighLightedLimbs[16];
	bool							byte284;
	float							time288;
	float							fDistortVertical;
	float							fDistortDuration;
	float							fVATSDistortTime;
	bool							byte298;
	float							time29C;
	float							fBurstDuration;
	float							fBurstIntensity2;
	float							fBurstIntensity;
	float							fPulseIntensity;
	float							fPulseRadius;
	bool							bUnk2B4;
	bool							byte2B5;
	float							fUnk2B8;
	float							fUnk2BC;
	float							fVATSTargetPulseRate;
	NiPointer<NiTexture>			spScanlineTexture;
	uint32_t						uiVisibleAreaScale;
	NiPointer<BSShaderAccumulator>	spAccumulator;

	bool GetBodyPartVisibilityPercent(BODY_PART_TYPE aeBodyPart, float& arfResult) const;

	void SetTarget(TESObjectREFR* apTarget, BODY_PART_TYPE aeBodyPart, bool abUseOcclusion);

	NiNode* GetTargetNode() const {
		return kCurrentTarget.spNode;
	}
};

ASSERT_SIZE(VATSEffect, 0x2D0);