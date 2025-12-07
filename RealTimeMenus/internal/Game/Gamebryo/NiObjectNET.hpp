#pragma once

#include "NiObject.hpp"
#include "NiFixedString.hpp"

class NiTimeController;
class NiExtraData;

class NiObjectNET : public NiObject {
public:
	NiObjectNET();
	virtual ~NiObjectNET();

	NiFixedString					m_kName;
	NiPointer<NiTimeController>		m_spControllers;
	NiExtraData**					m_ppkExtra;
	uint16_t						m_usExtraDataSize;
	uint16_t						m_usMaxSize;

	NIRTTI_ADDRESS(0x11F4304);

	NiTimeController* GetControllers() const { return m_spControllers; };
	NiTimeController* GetController(const NiRTTI* apRTTI) const;
	template <class ControllerType>
	ControllerType* GetController() const {
		return static_cast<ControllerType*>(GetController(&ControllerType::ms_RTTI));
	}

};

ASSERT_SIZE(NiObjectNET, 0x18);