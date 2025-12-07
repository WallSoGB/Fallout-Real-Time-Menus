#include "Controls.hpp"

// GAME - 0x7FDF30
Controls* Controls::GetSingleton() {
	return *reinterpret_cast<Controls**>(0x11F35CC);
}

// GAME - 0xA23390
int32_t Controls::GetControllerValue(uint32_t auiInputDevice, uint32_t aucInput) const {
	return ThisCall<int32_t>(0xA23390, this, auiInputDevice, aucInput);
}

// GAME - 0xA23A50
int32_t Controls::GetMouseButton(int32_t aiButton, int32_t aiToggleFlags) const {
	return ThisCall<int32_t>(0xA23A50, this, aiButton, aiToggleFlags);
}
