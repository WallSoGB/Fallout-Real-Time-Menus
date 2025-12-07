#include "TESMain.hpp"

TESMain* TESMain::GetSingleton() {
	return *reinterpret_cast<TESMain**>(0x11DEA0C);
}

// GAME - 0x45C670
SceneGraph* TESMain::GetWorldSceneGraph() {
	return *reinterpret_cast<SceneGraph**>(0x11DEB7C);
}

// GAME - 0x86FD90
void TESMain::UpdateImageSpace(bool abAdvanceTime) {
	ThisCall(0x86FD90, this, abAdvanceTime);
}

// GAME - 0x874B90
void TESMain::DrawWorld_CopyMenuBG(ImageSpaceManager* apImageSpaceManager, NiDX9Renderer* apRenderer, bool abIsMSAA, BSRenderedTexture* apRenderTarget) {
	ThisCall(0x874B90, this, apImageSpaceManager, apRenderer, abIsMSAA, apRenderTarget);
}

// GAME - 0x872B00
void TESMain::DrawWorld_Init() {
	ThisCall(0x872B00, this);
}

// GAME - 0x872F50
BSRenderedTexture* TESMain::DrawWorld_InitRenderTexture(NiDX9Renderer* apRenderer, BSRenderedTexture* apTexture, bool abDisableIS, bool abIsMSAA) {
	return ThisCall<BSRenderedTexture*>(0x872F50, this, apRenderer, apTexture, abDisableIS, abIsMSAA);
}

// GAME - 0x8727D0
void TESMain::DrawWorld_UpdateWater() {
	ThisCall(0x8727D0, this);
}

// GAME - 0x873200
void TESMain::DrawWorld_DrawScene(Sun* apSun, bool abRenderHands, bool abIsWireframe, bool abVATSQuery) {
	ThisCall(0x873200, this, apSun, abRenderHands, abIsWireframe, abVATSQuery);
}

// GAME - 0x875E40
void TESMain::DrawWorld_Refraction(BSShaderAccumulator* apAccumulator, BSRenderedTexture* apRenderTarget) {
	ThisCall(0x875E40, this, apAccumulator, apRenderTarget);
}

// GAME - 0x875E40
void TESMain::DrawWorld_DepthOfField(BSShaderAccumulator* apAccumulator, BSRenderedTexture* apRenderTarget) {
	ThisCall(0x875E40, this, apAccumulator, apRenderTarget);
}

// GAME - 0x876850
void TESMain::DrawWorld_End(BSRenderedTexture* apRenderTarget, ImageSpaceManager* apImageSpaceManager, NiDX9Renderer* apRenderer, Sun* apSun) {
	ThisCall(0x876850, this, apRenderTarget, apImageSpaceManager, apRenderer, apSun);
}

// GAME - 0x874B50
void TESMain::DrawWorld_RestartRenderTexture(BSRenderedTexture* apRenderTarget, bool abIsMSAA, uint32_t abClearFlags) {
	ThisCall(0x874B50, this, apRenderTarget, abIsMSAA, abClearFlags);
}

// GAME - 0x876A20
void TESMain::DrawWorld_ScreenSplatter(NiDX9Renderer* apRenderer) {
	ThisCall(0x876A20, this, apRenderer);
}

// GAME - 0x871290
void TESMain::DrawWorld_UpdateOffscreenBuffers() {
	ThisCall(0x871290, this);
}

// GAME - 0x871A50
void TESMain::DrawWorld_RenderedMenuInterface() {
	CdeclCall(0x871A50);
}

// GAME - 0x86FD70
void TESMain::OnIdle_DoInterfaceIdle() {
	ThisCall(0x86FD70, this);
}