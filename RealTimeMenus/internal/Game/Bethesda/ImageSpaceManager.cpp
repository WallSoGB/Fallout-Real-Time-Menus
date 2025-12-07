#include "ImageSpaceManager.hpp"

ImageSpaceManager* ImageSpaceManager::GetSingleton() {
#ifdef GAME
	return *reinterpret_cast<ImageSpaceManager**>(0x11F91AC);
#else
	return *reinterpret_cast<ImageSpaceManager**>(0xF23BFC);
#endif
}

// GAME - 0x4EBBC0
ImageSpaceEffect* ImageSpaceManager::GetEffect(uint32_t auiEffectID) const {
	return ThisCall<ImageSpaceEffect*>(0x4EBBC0, this, auiEffectID);
}
