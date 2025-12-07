#pragma once

#include "Gamebryo/NiTPrimitiveArray.hpp"

class ImageSpaceManager;
class NiScreenGeometry;
class NiDX9Renderer;
class ImageSpaceShader;
class ImageSpaceTexture;
class ImageSpaceShaderParam;
class ImageSpaceEffectParam;
class NiTriShape;

class ImageSpaceEffect {
public:
	virtual ~ImageSpaceEffect();
	virtual void Render(NiTriShape* apScreenShape, NiDX9Renderer* apRenderer, ImageSpaceEffectParam* apParam, bool abEndFrame);
	virtual void Setup(ImageSpaceManager* pISManager, ImageSpaceEffectParam* apParam);
	virtual void Shutdown();
	virtual void BorrowTextures(ImageSpaceEffectParam* apParam);
	virtual void ReturnTextures();
	virtual bool IsActive() const;
	virtual bool UpdateParams(ImageSpaceEffectParam* apParam);

	struct EffectInput {
		int32_t	iTexIndex;
		int32_t iFilterMode;
	};

	bool										bIsActive;
	bool										bParamsChanged;
	NiTPrimitiveArray<ImageSpaceShader*>		kEffects;
	NiTPrimitiveArray<ImageSpaceShaderParam*>	kShaderParams;
	NiTPrimitiveArray<ImageSpaceTexture*>		kTextures;
	NiTPrimitiveArray<EffectInput*>				kEffectInputs;
	NiTPrimitiveArray<uint32_t*>				kEffectOutputs;
};

ASSERT_SIZE(ImageSpaceEffect, 0x58)