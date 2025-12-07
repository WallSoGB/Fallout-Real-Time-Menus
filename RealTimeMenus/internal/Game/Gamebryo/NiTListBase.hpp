#pragma once

#include "NiMemObject.hpp"

typedef void* NiTListIterator;

template <class T_Data>
class NiTListItem : public NiMemObject {
public:
	NiTListItem* m_pkNext;
	NiTListItem* m_pkPrev;
	T_Data       m_element;
};

template <class Allocator, class T_Data>
class NiTListBase : public NiMemObject {
public:
    NiTListBase();
    ~NiTListBase();

    NiTListBase(const NiTListBase&) = delete;
    NiTListBase& operator=(const NiTListBase&) = delete;

    struct AntiBloatAllocator : public Allocator {
        uint32_t m_uiCount;
    };

    NiTListItem<T_Data>* m_pkHead;
    NiTListItem<T_Data>* m_pkTail;
    AntiBloatAllocator   m_kAllocator;
};