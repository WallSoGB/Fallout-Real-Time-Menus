#pragma once

#include "BSSimpleList.hpp"
#include "BSStringT.hpp"
#include "Gamebryo/NiSmartPointer.hpp"
#include "Gamebryo/NiPoint3.hpp"

class GridCellArray;
class Sky;
class NiNode;
class TESWaterSystem;
class NiDirectionalLight;
class TESObjectCELL;
class NiTexture;
class TESActorBase;
class NavMeshInfoMap;
class LoadedAreaBound;
class NiFogProperty;
class TESWorldSpace;
class ImageSpaceModifierInstance;
class TESObjectREFR;
class QueuedFile;
class GridDistantArray;
class BSTempNodeManager;
class bhkPickData;
class TESRegion;

struct DeadCountObject {
	TESActorBase*	pActorBase;
	uint16_t		uiCount;
};

class TES {
public:
	virtual bool GetMapNameForLocation(BSString& arName, NiPoint3 akLocation, TESWorldSpace* apWorldSpace) const;

	GridDistantArray*									pGridDistantArray;
	GridCellArray*										pGridCellArray;
	NiNode*												pObjRoot;
	NiNode*												pLODRoot;
	NiNode*												pObjLODWaterRoot;
	BSTempNodeManager*									pTempNodeManager;
	NiDirectionalLight*									pObjLight;
	NiFogProperty*										pObjFog;
	int32_t												iCurrentGridX;
	int32_t												iCurrentGridY;
	int32_t												iCurrentQueuedX;
	int32_t												iCurrentQueuedY;
	TESObjectCELL*										pInteriorCell;
	TESObjectCELL**										pInteriorBuffer;
	TESObjectCELL**										pExteriorBuffer;
	uint32_t											uiTempInteriorBufferSize;
	uint32_t											uiTempExteriorBufferSize;
	int32_t												iSaveGridX;
	int32_t												iSaveGridY;
	bool												bCollisionBoxes;
	bool												bRunningCellTests;
	bool												bRunningCellTests2;
	uint32_t											pfnTACCallbackFunc;
	uint32_t											unk58;
	TESRegion*											pTACRegionFilter;
	bool												bShowLandBorders;
	TESWaterSystem*										pWaterSystem;
	Sky*												pSky;
	BSSimpleList<NiPointer<ImageSpaceModifierInstance>> kActiveImageSpaceModifiers;
	uint32_t											uiTotalToLoad;
	uint32_t											uiLoaded;
	bool												bDisablePercentageUpdate;
	bool												bUpdateGridString;
	float												fCellDeltaX;
	float												fCellDeltaY;
	TESWorldSpace*										pWorldSpace;
	BSSimpleList<TESObjectCELL*>						kLastLoadedExteriors;
	BSSimpleList<TESObjectREFR*>						kBedsAndChairs;
	BSSimpleList<DeadCountObject*>						kDeadCount;
	NiPointer<QueuedFile>								spPreloadedAddonNodes;
	NiPointer<NiTexture>								spBloodDecalPreload1;
	NiPointer<QueuedFile>								spPreloadedForms;
	void*												pParticleCacheHead;
	bool												bFadeWhenLoading;
	bool												bAllowUnusedPurge;
	uint32_t											iPlaceableWaterCount;
	NavMeshInfoMap*										pNavMeshInfoMap;
	NiPointer<LoadedAreaBound>							spLoadedAreaBound;

	static TES* GetSingleton();
	void UpdateCellAnimations(float afTimeDelta);
};

ASSERT_SIZE(TES, 0xC4);