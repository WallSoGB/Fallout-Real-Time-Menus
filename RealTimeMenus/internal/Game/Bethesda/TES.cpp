#include "TES.hpp"

TES* TES::GetSingleton() {
#ifdef GAME
	return *reinterpret_cast<TES**>(0x11DEA10);
#else
	return *reinterpret_cast<TES**>(0xECF93C);
#endif
}

// GAME - 0x453550
void TES::UpdateCellAnimations(float afTimeDelta) {
	ThisCall(0x453550, this, afTimeDelta);
}