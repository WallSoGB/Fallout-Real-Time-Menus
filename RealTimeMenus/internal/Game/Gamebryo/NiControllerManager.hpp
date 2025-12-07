#pragma once

#include "NiTimeController.hpp"
#include "NiTObjectArray.hpp"
#include "NiTStringMap.hpp"
#include "NiControllerSequence.hpp"
#include "NiTSet.hpp"

class BSAnimNoteListener;
class NiDefaultAVObjectPalette;

class NiControllerManager : public NiTimeController {
public:
	NiTObjectArray<NiPointer<NiControllerSequence>>		m_kSequenceArray;
	NiTPrimitiveSet<NiControllerSequence*>				m_kActiveSequences;
	NiTStringPointerMap<NiControllerSequence*>			m_kIndexMap;
	BSAnimNoteListener*									pListener;
	bool												m_bCumulative;
	NiTObjectSet<NiPointer<NiControllerSequence>>		m_kTempBlendSeqs;
	NiPointer<NiDefaultAVObjectPalette>					m_spObjectPalette;

	CREATE_OBJECT(NiControllerManager, 0xA2F6C0);
	NIRTTI_ADDRESS(0x11F36AC);

	NiControllerSequence* GetSequenceByName(const NiFixedString& arName) const;
};

ASSERT_SIZE(NiControllerManager, 0x7C);