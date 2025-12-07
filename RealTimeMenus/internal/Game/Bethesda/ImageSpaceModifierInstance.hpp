#pragma once

#include "Gamebryo/NiObject.hpp"

class ImageSpaceModifierInstance : public NiObject {
public:
	virtual bool	IsExpired() const;
	virtual void	Apply();
	virtual bool	IsForm() const;
	virtual void	PrintInfo(const char* apBuffer) const;

	enum Flags {
		PERMANENT = 1,
	};

	bool			bIsHidden;
	float			fStrength;
	NiObjectPtr		spTarget;
	float			fAge;
	Bitfield32		uiFlags;
};

ASSERT_SIZE(ImageSpaceModifierInstance, 0x1C);