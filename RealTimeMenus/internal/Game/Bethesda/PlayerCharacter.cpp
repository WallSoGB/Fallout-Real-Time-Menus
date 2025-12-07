#include "PlayerCharacter.hpp"

PlayerCharacter* PlayerCharacter::GetSingleton() {
	return *reinterpret_cast<PlayerCharacter**>(0x011DEA3C);
}

// GAME - 0x950BB0
NiAVObject* PlayerCharacter::Get3D(bool abFirstPerson) const {
	return ThisCall<NiAVObject*>(0x950BB0, this, abFirstPerson);
}

// GAME - 0x950B00
BipedAnim* PlayerCharacter::GetBiped(bool abFirstPerson) const {
	if (abFirstPerson)
		return p1stPersonBipedAnim;
	else
		return pBipedAnim;
}

// GAME - 0x950A60
Animation* PlayerCharacter::GetAnimation(bool abFirstPerson) const {
	if (abFirstPerson)
		return p1stPersonAnimation;
	return static_cast<const Actor*>(this)->GetAnimation();
}

// GAME - 0x94AE40
void PlayerCharacter::UpdateCamera(bool abIsFinishLoadGame, bool abSkipUpdateLOD) {
	ThisCall(0x94AE40, this, abIsFinishLoadGame, abSkipUpdateLOD);
}

// GAME - 0x962720
bool PlayerCharacter::IsPlayerFollower(Actor* apActor) {
	return ThisCall<bool>(0x962720, this, apActor);
}

// GAME - 0x967AE0
bool PlayerCharacter::HasPipBoyOpen() const {
	return ThisCall<bool>(0x967AE0, this);
}

// GAME - 0x9481D0
void PlayerCharacter::ForceGrenadeHold() {
	ThisCall(0x9481D0, this);
}

// GAME - 0x969C30
void PlayerCharacter::UpdateHardcoreMode() {
	ThisCall(0x969C30, this);
}

// GAME - 0x953D40
void PlayerCharacter::UpdateGreetTimer() {
	ThisCall(0x953D40, this);
}

// GAME - 0x953D00
void PlayerCharacter::HandleResetGreetTimer() {
	ThisCall(0x953D00, this);
}

// GAME - 0x9444D0
void PlayerCharacter::UpdatePlayerCombat(float afTimeDelta) {
	ThisCall(0x9444D0, this, afTimeDelta);
}
