#pragma once

#include "NiTArray.hpp"

// GAME - 0x658930
template<class T_Data, class Allocator>
inline uint16_t NiTArray<T_Data, Allocator>::GetSize() const {
	return m_usSize;
}

// GAME - 0x9938B0
template<class T_Data, class Allocator>
inline uint16_t NiTArray<T_Data, Allocator>::GetEffectiveSize() const {
	return m_usESize;
}

// GAME - 0x877A30
template<class T_Data, class Allocator>
inline T_Data& NiTArray<T_Data, Allocator>::GetAt(uint32_t auiIndex) const {
	ASSUME_ASSERT(auiIndex < m_usSize);
	return m_pBase[auiIndex];
}