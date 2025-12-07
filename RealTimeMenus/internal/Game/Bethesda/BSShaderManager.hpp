#pragma once

class ShadowSceneNode;
class BSRenderedTexture;
class NiDX9Renderer;

class BSShaderManager {
public:
	static ShadowSceneNode* GetShadowSceneNode(uint32_t aeType);

	static void RenderImageSpaceEffects(NiDX9Renderer* apRenderer, BSRenderedTexture* apSourceTarget, BSRenderedTexture* apDestTarget);

	static bool IsRefractionEnabled();
};