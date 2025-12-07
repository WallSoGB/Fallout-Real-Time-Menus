#pragma once

#include "Gamebryo/NiPoint3.hpp"

struct FurnitureMark {
	NiPoint3	kPos;
	uint16_t	usRotation;
	uint8_t		ucType;
};

ASSERT_SIZE(FurnitureMark, 0x10);