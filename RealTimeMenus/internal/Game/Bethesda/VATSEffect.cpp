#include "VATSEffect.hpp"

// GAME - 0x800A00
bool VATSEffect::GetBodyPartVisibilityPercent(BODY_PART_TYPE aeBodyPart, float& arfResult) const {
	return ThisCall<bool>(0x800A00, this, aeBodyPart, &arfResult);
}

// GAME - 0x800AC0
void VATSEffect::SetTarget(TESObjectREFR* apTarget, BODY_PART_TYPE aeBodyPart, bool abUseOcclusion) {
	ThisCall(0x800AC0, this, apTarget, aeBodyPart, abUseOcclusion);
}