#include "MobileObject.hpp"

// GAME - 0x8D8520
BaseProcess* MobileObject::GetCurrentProcess() const {
    return pCurrentProcess;
}

// GAME - 0x933840
bool MobileObject::IsInDialoguewithPlayer() const {
	return ThisCall<bool>(0x933840, this);
}

// GAME - 0x931D70
float MobileObject::GetLooking() const {
	return GetAngleOnReference().x;
}

// GAME - 0x931D30
void MobileObject::ModHeading(float afHeading) {
	ThisCall(0x931D30, this, afHeading);
}

// GAME - 0x931E50
void MobileObject::ModLooking(float afLooking) {
	ThisCall(0x931E50, this, afLooking);
}