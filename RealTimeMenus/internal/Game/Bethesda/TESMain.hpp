#pragma once

#include "BSPackedTaskQueue.hpp"
#include "ScrapHeap.hpp"
#include "SceneGraph.hpp"
#include "Gamebryo/NiSmartPointer.hpp"

class Sun;
class BSAudioManager;
class BSPackedTaskQueue;
class ImageSpaceManager;
class BSClearZNode;
class VATS;
class BSShaderAccumulator;
class NiCamera;
class NiDX9Renderer;
class BSRenderedTexture;
class SceneGraph;

class TESMain {
public:
	bool							bOneMore;
	bool							bQuitGame;
	bool							bResetGame;
	bool							bGameActive;
	bool							bOnIdle;
	bool							unk05;
	bool							bIsFlyCam;
	bool							bFreezeTime;
	HWND							hWnd;
	HINSTANCE						hInstance;
	uint32_t						uiMainThreadID;
	HANDLE							hMainThread;
	ScrapHeap						kPackedTaskHeap;
	BSPackedTaskQueue				kTaskQueue;
	ScrapHeap						kSecondaryPackedTaskHeap;
	BSPackedTaskQueue				kSecondaryTaskQueue;
	NiPointer<BSShaderAccumulator>	spWorldAccum;
	NiPointer<BSShaderAccumulator>	sp1stPersonAccum;
	NiPointer<BSShaderAccumulator>	spAimDOFAccumulator;
	NiPointer<BSShaderAccumulator>	spScreenSplatterAccum;
	NiPointer<BSShaderAccumulator>	sp3DMenuAccumulator;
	bool							bShouldSetViewPort;
	NiPointer<NiCamera>				spFirstPersonCamera;

	static constexpr AddressPtr<NiPointer<NiDX9Renderer>, 0x11C73B4> spRenderer;
	static constexpr AddressPtr<int32_t, 0x11A2FE0> iFrameCounter;
	static constexpr AddressPtr<float, 0x11C3C08> fCellAppTime;
	static constexpr AddressPtr<bool, 0x11DEA28> bStaticMenuBG;
	static constexpr AddressPtr<bool, 0x11DEA29> bMenuBGReady;
	static constexpr AddressPtr<bool, 0x11DEA2A> bSceneIsComplex;
	static constexpr AddressPtr<bool, 0x11DEA2B> bInMenuMode;
	static constexpr AddressPtr<bool, 0x11DEA2C> bInDialogMenu;
	static constexpr AddressPtr<bool, 0x11DEA2D> bIsInPauseFade;
	static constexpr AddressPtr<bool, 0x11DEA2E> bConsoleOpen;
	static constexpr AddressPtr<float, 0x11DEA30> fFrameAnimTime;

	static constexpr AddressPtr<bool, 0x11DE9D1> bOffsetViewModelLights;

	static TESMain* GetSingleton();
	static SceneGraph* GetWorldSceneGraph();
	void UpdateImageSpace(bool abAdvanceTime);
	void DrawWorld_CopyMenuBG(ImageSpaceManager* apImageSpaceManager, NiDX9Renderer* apRenderer, bool abIsMSAA, BSRenderedTexture* apRenderTarget);
	void DrawWorld_Init();
	BSRenderedTexture* DrawWorld_InitRenderTexture(NiDX9Renderer* apRenderer, BSRenderedTexture* apTexture, bool abDisableIS, bool abIsMSAA);
	void DrawWorld_UpdateWater();
	void DrawWorld_DrawScene(Sun* apSun, bool abRenderHands, bool abIsWireframe, bool abVATSQuery);
	void DrawWorld_Refraction(BSShaderAccumulator* apAccumulator, BSRenderedTexture* apRenderTarget);
	void DrawWorld_DoDepth(BSShaderAccumulator* apAccumulator, BSRenderedTexture* apRenderTarget);
	void DrawWorld_End(BSRenderedTexture* apRenderTarget, ImageSpaceManager* apImageSpaceManager, NiDX9Renderer* apRenderer, Sun* apSun);
	void DrawWorld_RestartRenderTexture(BSRenderedTexture* apRenderTarget, bool abIsMSAA, uint32_t abClearFlags);
	void DrawWorld_ScreenSplatter(NiDX9Renderer* apRenderer);
	void DrawWorld_UpdateOffscreenBuffers();
	static void DrawWorld_RenderedMenuInterface();
	void OnIdle_DoInterfaceIdle();
};

ASSERT_SIZE(TESMain, 0xA4);