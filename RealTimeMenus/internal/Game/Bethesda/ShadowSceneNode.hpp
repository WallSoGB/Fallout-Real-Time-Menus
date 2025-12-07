#pragma once

#include "Gamebryo/NiNode.hpp"

NiSmartPointer(ShadowSceneNode);

class ShadowSceneNode : public NiNode {
public:
	void UpdateObjectLighting(NiAVObject* apObject, bool abIsMoving);
};