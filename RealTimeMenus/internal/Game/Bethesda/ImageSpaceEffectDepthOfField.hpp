#pragma once

#include "ImageSpaceEffect.hpp"

class ImageSpaceEffectOption : public ImageSpaceEffect {
public:
	NiTPrimitiveArray<bool> kEffectOn;
};

class ImageSpaceEffectDepthOfField : public ImageSpaceEffectOption {
public:
	bool DoDepth() const;
};