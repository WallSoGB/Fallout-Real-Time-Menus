#pragma once

#include "NiMatrix3.hpp"
#include "NiPoint3.hpp"

class NiTransform {
public:
	BS_ALLOCATORS

	NiTransform();

	NiMatrix3	m_Rotate;
	NiPoint3	m_Translate;
	float		m_fScale;
};

ASSERT_SIZE(NiTransform, 0x34);