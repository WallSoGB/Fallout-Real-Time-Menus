#pragma once

#include "TESBoundAnimObject.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "TESScriptableForm.hpp"
#include "TESModelTextureSwap.hpp"
#include "BGSOpenCloseForm.hpp"
#include "TESFullName.hpp"

class TESWaterForm;
class BGSTalkingActivator;
class TESSound;

class TESObjectACTI : public TESBoundAnimObject, public TESFullName, public TESModelTextureSwap, public TESScriptableForm, public BGSDestructibleObjectForm, public BGSOpenCloseForm {
public:
	TESObjectACTI();
	~TESObjectACTI();

	TESSound*				pSoundLoop;
	TESSound*				pSoundActivate;
	TESSound*				pRadioTemplate;
	TESWaterForm*			pWaterForm;
	BGSTalkingActivator*	pRadioStation;
	BSString				strActivationPrompt;
};

ASSERT_SIZE(TESObjectACTI, 0x90);