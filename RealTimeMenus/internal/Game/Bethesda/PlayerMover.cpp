#include "PlayerMover.hpp"

// GAME - 0x9EA570
void PlayerMover::SetSpeedVector(const NiPoint3& arVector) {
	ThisCall(0x9EA570, this, &arVector);
}
