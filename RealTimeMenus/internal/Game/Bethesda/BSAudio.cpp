#include "BSAudio.hpp"

// GAME - 0x453A70
BSAudio* BSAudio::GetSingleton() {
#ifdef GAME
	return *reinterpret_cast<BSAudio**>(0x11F6D98);
#else
	return *reinterpret_cast<BSAudio**>(0xF226C0);
#endif
}

// GAME - 0xAD78B0
void BSAudio::SetListenerPosition(float afX, float afY, float afZ) {
	ThisCall(0xAD78B0, this, afX, afY, afZ);
}

// GAME - 0xAD8480
void BSAudio::FadeOutSelected(uint32_t aeSoundTypes, uint32_t auiFadeDuration, uint32_t unk) {
	ThisCall(0xAD8480, this, aeSoundTypes, auiFadeDuration, unk);
}

// GAME - 0xAD84B0
void BSAudio::FadeInSelected(uint32_t aeSoundTypes, uint32_t auiFadeDuration) {
	ThisCall(0xAD84B0, this, aeSoundTypes, auiFadeDuration);
}

// GAME - 0x70BBA0
void BSAudio::PauseAnimationSounds() {
	ThisCall(0x70BBA0, this);
}

// GAME - 0x70BBC0
void BSAudio::UnpauseAnimationSounds() {
	ThisCall(0x70BBC0, this);
}

// GAME - 0x70BBE0
void BSAudio::PauseWorldVoices() {
	ThisCall(0x70BBE0, this);
}

// GAME - 0x70BC00
void BSAudio::UnpauseWorldVoices() {
	ThisCall(0x70BC00, this);
}
