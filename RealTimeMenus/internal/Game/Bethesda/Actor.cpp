#include "Actor.hpp"

// GAME - 0x437BD0
bool Actor::IsUnconscious() const {
	return ThisCall<bool>(0x437BD0, this);
}

// GAME - 0x4938E0
bool Actor::IsMoving() const {
	return ThisCall<bool>(0x4938E0, this);
}

// GAME - 0x493BB0
bool Actor::IsInCombat() const {
	return ThisCall<bool>(0x493BB0, this);
}

// GAME - 0x566950
bool Actor::IsPlayerTeammate() const {
	return ThisCall<bool>(0x566950, this);
}

// GAME - 0x8B87A0
FACTION_RELATION Actor::GetFactionRelation(Actor* apTarget, bool& arbPlayerEnemy) const {
	return ThisCall<FACTION_RELATION>(0x8B87A0, this, apTarget, &arbPlayerEnemy);
}

// GAME - 0x8885E0
void Actor::UpdateAnimationMovement(Animation* apAnimation, float afDelta) {
	ThisCall(0x8885E0, this, apAnimation, afDelta);
}

// GAME - 0x895110
void Actor::PickAnimations(float afWalkSpeed, float afTurnSpeed) {
	ThisCall(0x895110, this, afWalkSpeed, afTurnSpeed);
}

// GAME - 0x8A0C20
float Actor::GetMaxCarryWeight() const {
	return ThisCall<float>(0x8A0C20, this);
}

// GAME - 0x884F80
void Actor::SetWalkRunFlagDirty() {
	ThisCall(0x884F80, this);
}

// GAME - 0x8BA600
void Actor::HandlePostAnimationActions() {
	ThisCall(0x8BA600, this);
}

// GAME - 0x8C3C40
void Actor::UpdateMagic(bool abSleeping, bool abCompanionMenu) {
	ThisCall(0x8C3C40, this, abSleeping, abCompanionMenu);
}
