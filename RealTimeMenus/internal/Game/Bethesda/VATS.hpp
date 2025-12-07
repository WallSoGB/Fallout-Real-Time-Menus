#pragma once

#include "BSSimpleList.hpp"
#include "Gamebryo/NiSmartPointer.hpp"

class BGSCameraShot;
class Projectile;
class TESIdleForm;
class ImageSpaceModifierInstanceForm;
class ShadowSceneLight;
class NiPointLight;
class HitData;
class TESBoundObject;
class ExtraDataList;
class TESObjectREFR;

enum VATS_MODE_ENUM {
	VATS_MODE_NONE		= 0,
	VATS_TARGET_SELECT	= 1,
	VATS_MENU_MODE		= 2,
	VATS_SCAN_MODE		= 3,
	VATS_PLAYBACK		= 4,
	VATS_MODE_COUNT		= 5,
};

struct ActionPoints {
	enum Action {
		ATTACK_UNARMED			= 0,
		ATTACK_ONE_HAND_MELEE	= 1,
		ATTACK_TWO_HAND_MELEE	= 2,
		ATTACK_PISTOL			= 3,
		ATTACK_RIFLE			= 4,
		ATTACK_HANDLE			= 5,
		ATTACK_LAUNCHER			= 6,
		ATTACK_GRENADE			= 7,
		ATTACK_MINE				= 8,
		RELOAD					= 9,
		CROUCH					= 10,
		STAND					= 11,
		SWITCH_WEAPON			= 12,
		TOGGLE_WEAPON_DRAWN		= 13,
		HEAL					= 14,
		UNK_15					= 15,
		UNK_16					= 16,
		UNK_17					= 17,
		UNK_18					= 18,
		ONE_HAND_THROWN			= 19,
		ATTACK_THROWN			= 20,
		UNARMED_ATTACK_GROUND	= 21,
		MAX						= 22,
	};
};

struct VATS_COMMAND {
	uint32_t		eActionType;
	bool			bIsSuccess;
	uint8_t			byte05;
	bool			bIsMysteriousStrangerVisit;
	bool			bParalyzingPalm;
	uint8_t			ucRemainingShotsToFire_Burst;
	uint8_t			count09;
	TESObjectREFR*	pTarget;
	uint32_t		eBodyPart;
	HitData*		pHitData;
	float			unk18;
	float			unk1C;
	float			apCost;
	bool			bIsMissFortuneVisit;
};

class VATS {
public:
	BSSimpleList<VATS_COMMAND*>		kTargetsList;
	VATS_MODE_ENUM					eMode;
	BSSimpleList<BGSCameraShot*>*	pCameraShots;
	BGSCameraShot*					pCurrentCameraShot;
	float							fMaxTime;
	float							fMinTime;
	Projectile*						pFirstProjectile;
	Projectile*						pLastProjectile;
	TESIdleForm*					pMeleeAttack;
	ImageSpaceModifierInstanceForm* pISModInstForm;
	ImageSpaceModifierInstanceForm* pRadialBlur;
	uint32_t						unk30;
	NiPointer<ShadowSceneLight>		spVATSLight;
	bool							byte38;
	uint32_t						uiNumKills;
	TESBoundObject*					pBrokenWeapon;
	ExtraDataList*					pBrokenWeaponData;

	static VATS* GetSingleton();

	bool IsPlaying() const { return eMode == VATS_PLAYBACK; }
	bool HasMode() const { return eMode != VATS_MODE_NONE; };

	void SetMode(VATS_MODE_ENUM aeMode, bool abForce);
	void SetVATSLight(NiPointLight* apLight, bool abRemove);
	ShadowSceneLight* GetVATSLight() const;
	void RemoveLight();
};
