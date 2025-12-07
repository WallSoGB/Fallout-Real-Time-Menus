#pragma once

#include "TESPackage.hpp"

class CombatController : public TESPackage {
public:
	Actor*			GetPackageOwner() const;
	TESObjectREFR*	GetPackageTarget() const;

	bool IsActoraCombatTarget(Actor* apActor) const;
};