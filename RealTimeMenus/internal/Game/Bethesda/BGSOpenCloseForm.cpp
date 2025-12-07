#include "BGSOpenCloseForm.hpp"

#ifdef GAME
// GAME - 0x47A560
void BGSOpenCloseForm::HandleActivate(TESObjectREFR* apItemActivated, TESObjectREFR* apActionRef, BGSOpenCloseForm* apDoor) {
	CdeclCall(0x47A560, apItemActivated, apActionRef, apDoor);
}

// GAME - 0x47B250
BGSOpenCloseForm::OpenCloseState BGSOpenCloseForm::GetOpenState(TESObjectREFR* apRef) {
	return CdeclCall<BGSOpenCloseForm::OpenCloseState>(0x47B250, apRef);
}
#endif