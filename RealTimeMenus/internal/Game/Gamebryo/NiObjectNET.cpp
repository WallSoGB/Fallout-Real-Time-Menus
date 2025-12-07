#include "NiObjectNET.hpp"

// GAME - 0xA5C570
NiTimeController* NiObjectNET::GetController(const NiRTTI* apRTTI) const {
	return ThisCall<NiTimeController*>(0xA5C570, this, apRTTI);
}
