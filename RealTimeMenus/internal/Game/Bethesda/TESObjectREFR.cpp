#include "TESObjectREFR.hpp"

// GAME - 0x7AF430
TESBoundObject* TESObjectREFR::GetObjectReference() const {
	return kData.pObjectReference;
}

// GAME - 0x430830
const NiPoint3& TESObjectREFR::GetAngleOnReference() const {
	return kData.kRotation;
}

// GAME - 0x567400
float TESObjectREFR::GetScale() const {
	return ThisCall<float>(0x567400, this);
}

// GAME - 0x5723B0
float TESObjectREFR::GetDistanceFromReference(TESObjectREFR* apTarget, bool abEnabledOnly, bool abSharedSpace) const {
	return ThisCall<float>(0x5723B0, this, apTarget, abEnabledOnly, abSharedSpace);
}
