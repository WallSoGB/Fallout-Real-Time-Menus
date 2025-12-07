#pragma once

#include "BGSMenuPacker.hpp"
#include "BSRenderedTexture.hpp"
#include "FOPipboyManager.hpp"
#include "Menu.hpp"
#include "TESObjectREFR.hpp"
#include "Tile.hpp"
#include "VATSEffect.hpp"

class TileMenu;
class TileImage;
class SceneGraph;
class ImageSpaceShaderParam;
class ViewCaster;
class NiDirectionalLight;
class NiAlphaProperty;

enum SpecialInputCode {
	kInputCode_Backspace	= 0x80000000,
	kInputCode_ArrowLeft	= 0x80000001,
	kInputCode_ArrowRight	= 0x80000002,
	kInputCode_ArrowUp		= 0x80000003,
	kInputCode_ArrowDown	= 0x80000004,
	kInputCode_Home			= 0x80000005,
	kInputCode_End			= 0x80000006,
	kInputCode_Delete		= 0x80000007,
	kInputCode_Enter		= 0x80000008,
	kInputCode_PageUp		= 0x80000009,
	kInputCode_PageDown		= 0x8000000A
};

class InterfaceManager {
public:
	InterfaceManager();
	~InterfaceManager();

	enum MenuMode {
		MENU_MODE_DISABLED	= 0,
		MENU_MODE_OFF		= 1,
		MENU_MODE_ON		= 2,
		MENU_MODE_ENTERING	= 3,
		MENU_MODE_LEAVING	= 4,
		MENU_MODE_FIRSTPASS = 5,
	};

	struct Struct0178 {
		NiRefObjectPtr			spUnk0;
		BGSMenuPacker			kPacker;
		uint8_t					byte28;
		uint8_t					byte29;
		uint8_t					byte2A;
		uint8_t					byte2B;
		float					fStartTime;
		NiPoint2				kDuration;
		NiPoint2				kIntensity;
		NiPoint2				kFrequency;
		NiPoint2				kUnk48;
		ImageSpaceShaderParam*	pImageSpaceEffectParam;
		bool					bIsFlyCamEnabled;
		uint8_t					byte55;
		uint8_t					byte56;
		uint8_t					byte57;
		float					fBlurRadiusHUD;
		float					fScanlineFrequencyHUD;
		float					fBlurIntensityHUD;
	};

	struct Tutorials {
		int32_t		tutorialFlags[41];
		uint32_t	currentShownHelpID;
		uint32_t	timeA8;
	};

	struct Timer {
		Menu*	pMenu;
		float	fElapsed;
		float	fEnd;
		Timer*	pPrev;
		Timer*	pNext;
	};

	enum PipboyTabs
	{
		kStats = 1,
		kItems,
		kData
	};

	enum KeyModifier
	{
		kAltHeld = 0x1,
		kControlHeld = 0x2,
		kShiftHeld = 0x4,
	};

	enum Helps
	{
		kHelpPipBoyItems = 0x5,
		kHelpPipBoyRepair = 0x6,
		kSPECIAL = 0xC,
		kHelpChargenTagSkills = 0xE,
		kHelpChargenRace = 0xF,
		kHelpLeveling = 0x10,
		kHelpDialogue = 0x11,
		kHelpHacking = 0x13,
		kHelpLockpickingPC = 0x14,
		kHelpVATSPC = 0x15,
		kHelpContainer = 0x16,
		kHelpBarter = 0x17,
		kHelpTerminal = 0x18,
		kHelpPipBoyStats = 0x19,
		kHelpMapMenu = 0x1A,
		kHelpVATSXbox = 0x1B,
		kHelpLockpickingXBox = 0x1C,
		kHelpItemMod = 0x1D,
		kHelpCaravanBetting = 0x1E,
		kHelpCaravanDeckBuilding = 0x1F,
		kHelpCaravanStartingCaravans = 0x20,
		kHelpCaravanContractWar = 0x21,
		kHelpWeapons = 0x22,
		kHelpApparel = 0x23,
		kHelpAmmo = 0x24,
		kHelpAmmoBox = 0x25,
		kHelpCrafting = 0x26,
		kHelpReputation = 0x27,
		kHelpHardcoreNeeds = 0x28,
	};

	Bitfield32						uiFlags;
	NiPointer<SceneGraph>			spSceneGraph;
	NiPointer<SceneGraph>			sp3DSceneGraph;
	uint32_t						uiMenuMode;
	bool							bHasMutedSounds;
	bool							bLockMenuModeForFade;
	int8_t							cStatsPageNumber;
	int8_t							cInventoryPageNumber;
	int8_t							cMagicPageNumber;
	int8_t							cMapPageNumber;
	int32_t							iPickDistance;
	int32_t							iTelekinesisDistance;
	uint8_t							ucMuteAnimationSounds;
	uint8_t							ucMuteWorldSounds;
	uint8_t							byte022;
	uint8_t							byte023;
	NiDirectionalLight*				pPlayerLight;
	TileImage*						pCursor;
	NiPoint3						kCursorPos;
	NiPoint3						kRealCursorPos;
	float							fMouseWheel;		// -120.0 = down; 120.0 = up
	float							fMouseHeldTime;
	Tile*							pDraggedTile;
	int								unk050;
	float							flt054;
	float							flt058;
	int								unk05C;
	int								unk060;
	int								unk064;
	uint32_t						unk068[2];
	BSSimpleList<TESObjectREFR*>	kSelectableRefs;
	uint32_t						uiSelectedRefIndex;
	bool							bDebugTextVisible;
	bool							bMouseVisible;
	NiNode*							pInterfaceRoot;
	NiNode*							pCursorRoot;
	NiNode*							pPlayer3D;
	NiPointer<BSShaderAccumulator>	spTileAccumulator;
	NiPointer<BSShaderAccumulator>	sp3DAccumulator;
	ShadowSceneNode*				pObject3DScene;
	ShadowSceneNode*				pPlayer3DScene;
	Tile*							pMenuRoot;
	Tile*							pGlobalsTile;
	NiNode*							pDebugTextRoot;
	float							fOneToOneDistance;
	NiPointer<NiAlphaProperty>		spAlphaProperty;
	uint16_t						unk0B0;
	uint16_t						unk0B2;
	uint32_t						unk0B4;
	uint32_t						unk0B8;
	Tile*							pMouseOverTarget;
	uint32_t						unk0C0;
	uint32_t						unk0C4;
	uint8_t							byte0C8;
	uint8_t							byte0C9;
	uint8_t							byte0CA;
	uint8_t							byte0CB;
	Tile*							pOverTileTarget;
	Menu*							pOverTileMenu;
	Tile*							pDragOverTileTarget;
	Menu*							pDragOverTileMenu;
	uint8_t							unk0DC;
	uint8_t							unk0DD;
	uint32_t						unk0E0;
	int8_t							cLastMessageButtonClicked;		// -1 if no button pressed
	void(__cdecl*					fpResult)();
	bool							bTextureRelease;
	bool							bMouseInMotion;
	TESObjectREFR*					pPickRef;
	TESObjectREFR*					pReticleRef;
	TESObjectREFR*					pCrossHairRef;
	TESObjectREFR*					pActivateRef;
	TESObjectREFR*					pTelekinesisRef;
	NiPoint3						kActivatePickLocation;
	bool							bFuzzyActivatePick;
	int32_t							iEnterStack[10];
	ViewCaster*						pViewCaster;
	Actor*							pJailSpeaker;
	Actor*							pPayFineSpeaker;
	uint8_t							byte148;
	bool							bShiftDown;
	int32_t							iModifierKeys;
	int32_t							iRepeatingKey;
	uint32_t						uiKeyDownTime;
	uint32_t						uiKeyRepeatStartTime;
	SpecialInputCode				eLastGamepadEvent;
	uint32_t						uiGamepadRepeatStartTime;
	Timer*							pTimers;
	bool							bIsRenderedMenuSet;
	FORenderedMenu*					pRenderedMenu;
	bool							bMouseInMotionInRenderedMenu;
	FOPipboyManager*				pPipboyManager;
	Struct0178						unk178;
	VATSEffect						kVATSEffect;
	float							fScale4AC;
	float							fUnk4B0;
	uint8_t							bIsRenderedMenuOrPipboyManager;
	uint8_t							byte4B5;
	uint8_t							byte4B6;
	uint8_t							byte4B7;
	uint32_t						queuedPipboyTabToSwitchTo;
	uint32_t						uiPipBoyMode;
	void							(*onPipboyOpenCallback)(void);
	BSSimpleList<void*>				kUnk4C4;
	uint8_t							byte4CC;
	uint8_t							byte4CD;
	uint8_t							pad4CE;
	uint8_t							pad4CF;
	float							fScale4D0;
	Tutorials						kHelp;

	static InterfaceManager* GetSingleton();

	VATSEffect* GetVATSEffect();

	bool GetUseRenderedBackground() const;

	bool IsDebugTextVisible() const;

	FOPipboyManager* GetPipboyManager() const;

	FORenderedMenu* GetCurrentRenderedMenu() const;

	int32_t GetEnterStack(uint32_t auiIndex) const;

	bool IsMenuInStack(uint32_t auiMenuID) const;

	static bool IsControllerConnected();

	static float GetScreenWidth();
	static float GetScreenHeight();
};

ASSERT_SIZE(InterfaceManager, 0x580);
ASSERT_SIZE(InterfaceManager::Struct0178, 0x64);