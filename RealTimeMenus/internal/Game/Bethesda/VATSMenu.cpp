#include "VATSMenu.hpp"

// GAME - 0x7F3C90
BSSimpleList<VATSTarget*>* VATSMenu::FindTarget(TESObjectREFR* apRef) {
	return ThisCall<BSSimpleList<VATSTarget*>*>(0x7F3C90, this, apRef);
}
