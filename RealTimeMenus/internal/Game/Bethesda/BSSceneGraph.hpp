#pragma once

#include "Gamebryo/NiNode.hpp"

class BSCullingProcess;
class NiCamera;
class NiVisibleArray;

NiSmartPointer(BSSceneGraph);

class BSSceneGraph : public NiNode {
public:
	virtual double GetFarDistance();
	virtual void   SetViewDistanceBasedOnFrameRate(float afTime);

	NiPointer<NiCamera> spCamera;
	NiVisibleArray*		pVisArray;
	BSCullingProcess*	pCuller;
	bool				bMenuSceneGraph;
	float				fCurrentFOV;

	NIRTTI_ADDRESS(0x1203180);

	__forceinline NiCamera* GetCamera() const { return spCamera; };
	__forceinline BSCullingProcess* GetCullingProcess() const { return pCuller; };

	NiNode* GetCameraRoot() const {
		return static_cast<NiNode*>(GetAtChecked(0));
	}
};

ASSERT_SIZE(BSSceneGraph, 0xC0);