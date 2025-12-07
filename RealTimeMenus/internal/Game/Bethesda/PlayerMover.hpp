#pragma once

#include "ActorMover.hpp"

class PlayerMover : public ActorMover {
public:
	NiPoint3					kSpeedVector;
	Bitfield<_MovementFlags>	uiPlayerMovementFlags;
	Bitfield32					uiTurnAnimFlags;
	float						fTurnAnimMinTime;

	void SetSpeedVector(const NiPoint3& arVector);
};

ASSERT_SIZE(PlayerMover, 0xA0);