#pragma once

#include "Gamebryo/NiColor.hpp"
#include "Gamebryo/NiColorA.hpp"
#include "Gamebryo/NiSmartPointer.hpp"

class TileMenu;
class BSRenderedTexture;
class BSFadeNode;
class NiTriBasedGeom;
class NiScreenElements;
class NiTexture;
class NiPointLight;
class NiCamera;
class SceneGraph;
class ShadowSceneNode;
class BSShaderAccumulator;

class FORenderedMenu {
public:
	virtual				~FORenderedMenu();
	virtual bool		IsRaceSexMenu() const;
	virtual BSFadeNode*	GetMenuRoot();
	virtual void		Render(BSRenderedTexture* apTexture, uint32_t aeClearFlags, BSRenderedTexture* apTarget);
	virtual void		Unk_04();
	virtual bool		Update();
	virtual bool		Unk_06(bool, float&, float&);
	virtual void		Unk_07();
	virtual void		Unk_08();
	virtual void		InitModels();
	virtual void		ClearModels();
	virtual float		Unk_0B();
	virtual void		OnMenuOpen();
	virtual void		OnMenuClose();
	virtual void		HandleStaticEffect(float afTimeDelta);
	virtual void		HandleVerticalHoldEffect(float afTimeDelta);
	virtual void		HandleShudderEffect(float afTimeDelta);
	virtual void		HandlePulseEffect(float afTimeDelta);
	virtual void		HandleScreenLightEffect(float afTimeDelta);
	virtual void		HandleScanlineEffect(float afTimeDelta);
	virtual void		RenderPostProcess(BSRenderedTexture* apSource, BSRenderedTexture* apTarget, uint32_t aeClearFlags);

	NiPointer<NiTriBasedGeom>		spScreenGeometry;
	NiPointer<BSFadeNode>			spMenuRoot;
	NiPointer<NiScreenElements>		spScreenElements;
	NiPointer<NiTexture>			spSrcTexture;
	NiPointer<SceneGraph>			spSceneGraph;
	TileMenu*						pTileMenu;
	NiPointer<NiPointLight>			spMenuLight;
	NiPointer<ShadowSceneNode>		spShadowScene;
	NiPointer<NiCamera>				spCamera;
	bool							bIsInitialized;
	uint8_t							byte029;
	uint8_t							byte02A;
	uint8_t							gap02B;
	NiColor							kScreenLightColor;
	float							fScreenLightBaseIntensity;
	float							unk03C;
	float							fBlurRadius;
	float							fBlurIntensity;
	float							fScanlineFrequency;
	uint8_t							byte04C;
	uint8_t							bIsShowStaticEffect;
	float							fStaticEffectStartTime;
	float							fBurstDuration;
	float							fBurstIntensity2;
	float							fBurstIntensity;
	bool							bIsShowVerticalHoldEffect;
	float							fVerticalHoldStartTime;
	float							fVerticalHoldDuration;
	float							fVerticalHoldSpeed;
	float							fUnk070;
	uint8_t							bIsShowShudderHoldEffect;
	float							fShudderHoldStartTime;
	float							fShudderHoldDuration;
	float							fShudderHoldIntensity;
	float							fShudderHoldFrequency;
	float							fUnk088;
	float							fPulseBrightenIntensity;
	float							fPulseRadiusIntensity;
	uint8_t							byte094;
	bool							bIsScanlineEffect;
	float							fScanLineStartTime;
	float							fInterval09C;
	float							fDistortDuration;
	float							fNextScanlineTime;
	NiColorA						kTileColor;
	uint8_t							bIsRenderedMenuOrPipboyOpen;
	uint8_t							byte0B9;
	NiPointer<NiTexture>			spScanlineTexture;
	NiColorA						kTileColor2;
	bool							bIsQuantityMenuShown;
	bool							bIsTutorialMenuShown;
	bool							bIsMessageMenuShown;
	NiPointer<BSShaderAccumulator>  spShaderAccumulator;
};

ASSERT_SIZE(FORenderedMenu, 0xD8);