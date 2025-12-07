#include "ImageSpaceEffectDepthOfField.hpp"

// GAME - 0x871220
bool ImageSpaceEffectDepthOfField::DoDepth() const {
	return ThisCall<bool>(0x871220, this);
}