#include "VATSMenu.hpp"

// GAME - 0x7F3C90
BSSimpleList<VATSTarget*>* VATSMenu::FindTarget(TESObjectREFR* apRef) {
	return ThisCall<BSSimpleList<VATSTarget*>*>(0x7F3C90, this, apRef);
}

// GAME - 0x7F1840
void VATSMenu::InitBodyPartTiles() {
	ThisCall(0x7F1840, this);
}

// GAME - 0x7F0EA0
void VATSMenu::UpdateHitChanceTiles() {
	ThisCall(0x7F0EA0, this);
}

// GAME - 0x7F06D0
bool VATSMenu::IsBodyPartVisible(VATSBodyPart* apBodyPart) const {
	return ThisCall<bool>(0x7F06D0, this, apBodyPart);
}

// GAME - 0x7F1290
float VATSMenu::UpdateHitChanceForBodyPartData(BSSimpleList<VATSBodyPart*>* apBodyPartEntry) {
	return ThisCall<float>(0x7F1290, this, apBodyPartEntry);
}
