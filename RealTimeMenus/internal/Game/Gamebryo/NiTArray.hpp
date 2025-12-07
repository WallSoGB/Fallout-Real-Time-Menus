#pragma once

#include "NiMemObject.hpp"

template <class T_Data, class Allocator>
class NiTArray : public NiMemObject {
public:
	NiTArray(uint32_t auiMaxSize = 0, uint32_t auiGrowBy = 1);
	virtual ~NiTArray();
	NiTArray(const NiTArray&) = delete;
	NiTArray& operator=(const NiTArray&) = delete;

	T_Data*		m_pBase;
	uint16_t	m_usMaxSize;
	uint16_t	m_usSize;
	uint16_t	m_usESize;
	uint16_t	m_usGrowBy;

	// Array size, including empty elements
	uint16_t GetSize() const;
	// Array size, excluding empty elements
	uint16_t GetEffectiveSize() const;
	// Total allocated size
	uint16_t GetAllocatedSize() const;
	
	T_Data& GetAt(uint32_t auiIndex) const;
};

#include "NiTArray.inl"