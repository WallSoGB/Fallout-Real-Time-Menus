#pragma once

#include "NiMemObject.hpp"

#if !JIP_CHANGES
#include "NiTPrimitiveArray.hpp"
#include "NiCriticalSection.hpp"
#endif

class NiFixedString;

// Replaced by JIP
class NiGlobalStringTable : public NiMemObject {
public:
	typedef char* GlobalStringHandle;

#if !JIP_CHANGES
	NiTPrimitiveArray<GlobalStringHandle>	m_kHashArray[512];
	void* unk2000[32];
	NiCriticalSection						m_kCriticalSection;
	void* unk20A0[24];
#endif
};

#if !JIP_CHANGES
ASSERT_SIZE(NiGlobalStringTable, 0x2100);
#endif