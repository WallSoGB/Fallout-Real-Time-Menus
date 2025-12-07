#pragma once

#include "NiTMap.hpp"
#include "NiTPointerMap.hpp"

template <class T_Parent, class T_Data>
class NiTStringTemplateMap : public T_Parent {
public:
	NiTStringTemplateMap(uint32_t auiHashSize, bool abCopy);
	virtual ~NiTStringTemplateMap();
	
	bool m_bCopy;
};