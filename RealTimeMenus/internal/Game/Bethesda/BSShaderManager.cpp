#include "BSShaderManager.hpp"

BSTextureManager* BSShaderManager::GetTextureManager() {
	return *reinterpret_cast<BSTextureManager**>(0x11F91A8);
}

// GAME - 0x450B80
ShadowSceneNode* BSShaderManager::GetShadowSceneNode(uint32_t aeType) {
#ifdef GAME
	return reinterpret_cast<ShadowSceneNode**>(0x11F91C8)[aeType];
#else
	return reinterpret_cast<ShadowSceneNode**>(0xF23C18)[aeType];
#endif
}

// GAME - 0xB55AC0
void BSShaderManager::RenderImageSpaceEffects(NiDX9Renderer* apRenderer, BSRenderedTexture* apSourceTarget, BSRenderedTexture* apDestTarget) {
	CdeclCall(0xB55AC0, apRenderer, apSourceTarget, apDestTarget);
}

// GAME - 0x5B9B00
bool BSShaderManager::IsRefractionEnabled() {
	return CdeclCall<bool>(0x5B9B00);
}
