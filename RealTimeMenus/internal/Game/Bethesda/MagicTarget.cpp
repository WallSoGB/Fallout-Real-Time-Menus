#include "MagicTarget.hpp"

// GAME - 0x822B40
Actor* MagicTarget::GetTargetAsActor() const {
	return ThisCall<Actor*>(0x822B40, this);
}
