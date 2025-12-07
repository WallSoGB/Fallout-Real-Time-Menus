#include "NiNode.hpp"

// GAME - 0x43B480
uint32_t NiNode::GetArrayCount() const {
    return m_kChildren.GetSize();
}

// GAME - 0x453470
uint32_t NiNode::GetChildCount() const {
    return m_kChildren.GetEffectiveSize();
}

// GAME - 0x43B4A0
NiAVObject* NiNode::GetAt(uint32_t i) const {
    return m_kChildren.GetAt(i).m_pObject;
}

// GAME - 0x45BC00
NiAVObject* NiNode::GetAtChecked(uint32_t i) const {
    if (GetArrayCount() <= i)
        return nullptr;

    return GetAt(i);
}