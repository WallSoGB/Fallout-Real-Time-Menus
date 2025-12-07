#include "CombatController.hpp"

// GAME - 0x97AE90
Actor* CombatController::GetPackageOwner() const {
	return ThisCall<Actor*>(0x97AE90, this);
}

// GAME - 0x4030B0
TESObjectREFR* CombatController::GetPackageTarget() const {
	return ThisCall<TESObjectREFR*>(0x4030B0, this);
}

// GAME - 0x97FA10
bool CombatController::IsActoraCombatTarget(Actor* apActor) const {
	return ThisCall<bool>(0x97FA10, this, apActor);
}
