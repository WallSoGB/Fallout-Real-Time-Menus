#pragma once

#include "NiAVObject.hpp"
#include "NiTObjectArray.hpp"

NiSmartPointer(NiNode);

class NiFixedString;

class NiNode : public NiAVObject {
public:
	NiNode();
	virtual ~NiNode();

	virtual void	AttachChild(NiAVObject* apChild, bool abFirstAvail);
	virtual void	InsertChildAt(uint32_t i, NiAVObject* apChild);
	virtual void	DetachChildAlt(NiAVObject* apChild, NiAVObjectPtr& arResult);
	virtual void	DetachChild(NiAVObject* apChild);
	virtual void	DetachChildAtAlt(uint32_t i, NiAVObjectPtr& arResult);
	virtual void	DetachChildAt(uint32_t i);
	virtual void	SetAtAlt(uint32_t i, NiAVObject* apChild, NiAVObjectPtr& arResult);
	virtual void	SetAt(uint32_t i, NiAVObject* apChild);
	virtual void	UpdateUpwardPass();

	NiTObjectArray<NiAVObjectPtr> m_kChildren;

	CREATE_OBJECT(NiNode, 0xA5F030);
	NIRTTI_ADDRESS(0x11F4428);
	
	uint32_t GetArrayCount() const;
	uint32_t GetChildCount() const;
	NiAVObject* GetAt(uint32_t i) const;
	NiAVObject* GetAtChecked(uint32_t i) const;
};

ASSERT_SIZE(NiNode, 0xAC);