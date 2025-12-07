#pragma once

#include "TESModel.hpp"
#include "BSSimpleList.hpp"

class BGSTextureSet;
class NiAVObject;

struct TEX_SWAP {
	BGSTextureSet*	pTextureSet;
	uint32_t		uiIndex3D;
	char			cTextureName[128];
};

class TESModelTextureSwap : public TESModel {
public:
	TESModelTextureSwap();
	~TESModelTextureSwap();

	BSSimpleList<TEX_SWAP*> kTextureList;
};

ASSERT_SIZE(TESModelTextureSwap, 0x20);