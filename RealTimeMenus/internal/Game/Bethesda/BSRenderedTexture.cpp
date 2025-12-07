#include "BSRenderedTexture.hpp"

// GAME - 0x4E9510
bool BSRenderedTexture::IsOutsideFrame() {
	return CdeclCall<bool>(0x4E9510);
}

// GAME - 0xB6B790
void BSRenderedTexture::StopOffscreen() {
	CdeclCall(0xB6B790);
}