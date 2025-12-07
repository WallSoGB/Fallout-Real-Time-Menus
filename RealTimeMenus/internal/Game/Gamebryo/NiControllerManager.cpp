#include "NiControllerManager.hpp"

// GAME - 0x47A520
NiControllerSequence* NiControllerManager::GetSequenceByName(const NiFixedString& arName) const {
	return ThisCall<NiControllerSequence*>(0x47A520, this, &arName);
}