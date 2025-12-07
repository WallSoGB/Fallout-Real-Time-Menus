#include "NiAVObject.hpp"

// GAME - 0x43D450
NiBound* NiAVObject::GetWorldBound() const {
	return ThisCall<NiBound*>(0x43D450, this);
}