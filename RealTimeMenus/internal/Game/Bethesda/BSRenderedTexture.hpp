#pragma once

#include "Gamebryo/NiObject.hpp"

class NiRenderTargetGroup;
class NiRenderedTexture;

NiSmartPointer(BSRenderedTexture);

class BSRenderedTexture : public NiObject {
public:
	NiPointer<NiRenderTargetGroup>	spRenderTargetGroups[6];
	NiPointer<NiRenderTargetGroup>	spPreRestoreGroup;
	NiPointer<NiRenderTargetGroup>	spObject024; // Xbox360 EDRAM RT
	NiPointer<NiRenderTargetGroup>	spObject028; // Xbox360 EDRAM RT
	int32_t							eType; // BSTextureManager::eTextureType
	NiPointer<NiRenderedTexture>	spRenderedTextures[4];


	static bool IsOutsideFrame();

	static void StopOffscreen();
};

ASSERT_SIZE(BSRenderedTexture, 0x40);