#pragma once

#include "BSTextureManager.hpp"

class ShadowSceneNode;
class BSRenderedTexture;
class NiDX9Renderer;
class NiCamera;

class BSShaderManager {
public:
	static inline constexpr AddressPtr<NiCamera*, 0x11F917C> pCamera;
	static inline constexpr AddressPtr<BSRenderedTexture*, 0x11F9438> pCurrentRenderTarget;

	static BSTextureManager* GetTextureManager();

	static ShadowSceneNode* GetShadowSceneNode(uint32_t aeType);

	static void RenderImageSpaceEffects(NiDX9Renderer* apRenderer, BSRenderedTexture* apSourceTarget, BSRenderedTexture* apDestTarget);

	static bool IsRefractionEnabled();
};