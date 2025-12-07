#include "TESForm.hpp"

// GAME - 0x84E3A0
uint32_t TESForm::GetFormID() const{
	return uiFormID;
}

// GAME - 0x401170
FORM_TYPE TESForm::GetFormType() const {
	return static_cast<FORM_TYPE>(eFormType);
}

// GAME - 0x4839C0
TESForm* TESForm::GetFormByNumericID(uint32_t auID) {
	return CdeclCall<TESForm*>(0x4839C0, auID);
}

// GAME - 0x483A00
TESForm* TESForm::GetFormByEditorID(const char* apEDID) {
	return CdeclCall<TESForm*>(0x483A00, apEDID);
}

// GAME - 0x460250
bool TESForm::GetMaster() const {
	return uiFormFlags.IsSet(FormFlags::MASTER);
}

// GAME - 0x460340
bool TESForm::GetAltered() const {
	return uiFormFlags.IsSet(FormFlags::ALTERED);
}

// GAME - 0x4013E0
bool TESForm::GetInitialized() const {
	return uiFormFlags.IsSet(FormFlags::INITIALIZED);
}

// GAME - 0x50D4A0
bool TESForm::GetNoCollision() const {
	return uiFormFlags.IsSet(FormFlags::NO_COLLISION);
}

// GAME - 0x440D80
bool TESForm::GetDeleted() const {
	return uiFormFlags.IsSet(FormFlags::DELETED);
}

// GAME - 0x440DA0
bool TESForm::GetDisabled() const {
	return uiFormFlags.IsSet(FormFlags::DISABLED);
}

// GAME - 0x4077C0
bool TESForm::GetTemporary() const {
	return uiFormFlags.IsSet(FormFlags::TEMPORARY);
}

// GAME - 0x576D30
bool TESForm::IsStillLoading() const {
	return uiFormFlags.IsSet(FormFlags::STILL_LOADING);
}