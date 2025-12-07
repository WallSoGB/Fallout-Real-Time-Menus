#pragma once

#include "ActorMover.hpp"
#include "ActorValueOwner.hpp"
#include "CachedValuesOwner.hpp"
#include "MagicCaster.hpp"
#include "MagicTarget.hpp"
#include "MobileObject.hpp"
#include "ModifierList.hpp"

class BGSPerk;
class bhkRagdollController;
class bhkRagdollPenetrationUtil;
class TESActorBase;
class ContinuousBeamProjectile;
class TESPackage;
class TESRace;
class CombatGroup;
class CombatController;
class BGSEntryPointPerkEntry;
class SpellItem;
class ArrowProjectile;
class BSSoundHandle;
class ItemChange;
class TESObjectBOOK;
class TrespassPackage;
class TESFaction;
class TESSkill;
class FACTION_RANK;
class TESNPC;
class AlchemyItem;
class BGSPerkEntry;

struct DispositionModifier {
	int32_t	iValue;
	Actor*	pTarget;
};

struct DispositionOverride {
	TESRace*		pRace;
	TESActorBase*	pActorBase;
	TESNPC*			pCharacterBase;
	TESForm*		pOwner;
	int32_t			iDispositionBase;
	int32_t			iResponsibility;
};


class Actor : public MobileObject, public MagicCaster, public MagicTarget, public ActorValueOwner, public CachedValuesOwner {
public:
	Actor();
	virtual ~Actor();

	virtual bool							IsGuard() const;
	virtual void							SetGuard(bool abVal);
	virtual void							SetEquippedWeight(float afVal);
	virtual float							GetEquippedWeight() const;
	virtual int32_t							GetInfamy() const;
	virtual int32_t							GetFame() const;
	virtual void							SetIgnoreCrime(bool abVal);
	virtual bool							GetIgnoreCrime() const;
	virtual void							Resurrect(bool abResetInventory, bool abAttach3D, bool abGetupResurect);
	virtual void							SetAdvanceNumber(uint32_t auiVal);
	virtual uint32_t						GetAdvanceNumber() const;
	virtual void							SetSkillAdvanceDelayed(ActorValue::Index aeIndex);
	virtual ActorValue::Index				GetSkillAdvanceDelayed() const;
	virtual bool							DoDamage(float afHealth, float afFatigue, Actor* apSource);
	virtual void							UseActionPoints(float afAmount);
	virtual void							UseActionPointsAlt(uint32_t auiAction);
	virtual uint32_t						GetDisposition(Actor* apTarget, DispositionOverride* apOverride = nullptr);
	virtual void							UpdateMovement(float afTimeDelta, bool);
	virtual void							UpdateNonRenderSafeMovement(float afTimeDelta, bool);
	virtual void							UpdateAnimationMovementNoWorldUpdate();
	virtual float							GetTurningSpeed() const;
	virtual bool							IsOverEncumbered() const;
	virtual bool							IsHumanoidCreature() const;
	virtual bool							IsPc() const;
	virtual bool							GetCannibal() const;
	virtual void							SetCannibal(bool abVal);
	virtual bool							GetSandman() const;
	virtual void							SetSandman(bool abVal);
	virtual void							InitiateSandmanPackage(Actor* apTarget, TESObjectREFR* apFurnitureRef, FurnitureMark* apFurnitureMark, uint8_t aucFurnitureMarkerIndex);
	virtual void							InitiateCannibalPackage(Actor* apTarget);
	virtual TESRace*						GetRace() const;
	virtual float							GetReach() const;
	virtual void							SetRefraction(bool abEnable, float afRefractionPower);
	virtual void							SetHasRagdoll(bool abVal);
	virtual bool							GetHasRagdoll() const;
	virtual ACTOR_TYPE						GetActorType() const;
	virtual void							SetActorValueF(uint32_t auiAVCode, float afValue);
	virtual void							SetActorValueI(uint32_t auiAVCode, int32_t aiValue);
	virtual void							TempModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker);
	virtual void							TempModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void							PermanentModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker);
	virtual void							PermanentModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void							DamageModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker);
	virtual void							DamageModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void							ModActorBaseValueF(uint32_t auiAVCode, float afModifier);
	virtual void							ModActorBaseValueI(uint32_t auiAVCode, int32_t aiModifier);
	virtual ItemChange*						GetBestWeapon(uint32_t aeWeaponType) const;
	virtual ItemChange*						GetBestAmmo() const;
	virtual void							ResetArmorRating();
	virtual bool							DamageEquipment(ItemChange* apEquipment, float afDamage, bool abIgnoreArmorSkill = false);
	virtual TESObjectREFR*					DropObject(TESForm* apItem, ExtraDataList* apExtraList, int32_t aiCount, const NiPoint3* apPoint, const NiPoint3* apRotate);
	virtual void							PickUpObject(TESObjectREFR* apObject, int32_t aiCount, bool abPlayPickUpSounds);
	virtual void							CastScroll(TESObjectBOOK* apBook, MagicTarget* apTarget);
	virtual void							CheckCastWhenStrikesEnchantment(ItemChange* apWeapon, Actor* apTarget, ArrowProjectile* apArrow, bool& abInsufficientCharge);
	virtual void							UpdateWornEnchantments(float afElapsedTime);
	virtual bool							AddSpell(SpellItem* apSpell);
	virtual bool							RemoveSpell(SpellItem* apSpell);
	virtual bool							ReloadWeapon(TESObjectWEAP* apWeapon, uint32_t, bool);
	virtual bool							ReloadWeaponNV(TESObjectWEAP* apWeapon, uint32_t, bool, bool);
	virtual void							UseAmmo(uint32_t auiCount = UINT32_MAX);
	virtual void							CheckTempModifiers();
	virtual CombatGroup*					GetCombatGroup() const;
	virtual void							SetCombatGroup(CombatGroup* apGroup);
	virtual bool							InitiateTresPassPackage(TrespassPackage* apPackage);
	virtual void							UpdateNonRenderSafeDialogueUpdate(float afDelta);
	virtual BSSoundHandle					VoiceSoundFunctionEx(TESTopic* apTopic, Actor* apTarget, bool abIs2D, bool abNeverSubtitle, int unused, bool abSayTo);
	virtual void							InitiateSpectator();
	virtual void							InitiateFlee(TESObjectREFR* apFleeRef, bool abRunOnce, bool abKnowsTarget, bool abCombatMode, TESObjectCELL* apCell, TESObjectREFR* apRef, float afFleeFromDist, float afFleeToDist);
	virtual void							InitiateSearchForAttacker(TESObjectREFR* apAttacker, NiPoint3);
	virtual void							InitiateGetUpPackage();
	virtual void							SetAlpha(float afValue);
	virtual float							GetAlpha();
	virtual void							StartCombat(Actor* apTarget, CombatGroup* apCombatGroup, bool abForceCombat, bool abIsAggressor, bool abFleeing, int32_t aiPriority, bool abCastSpell, TESPackage* apPackage);
	virtual CombatController*				GetCombatController() const;
	virtual Actor*							GetCombatTarget() const;
	virtual void							UpdateCombat(float afElapsedTime);
	virtual void							StopCombat(Actor* apAttacker);
	virtual void							PerformOneRoundofCombat();
	virtual float							GetArmorRating();
	virtual float							GetArmorThreshold();
	virtual float							GetAttackDamage() const;
	virtual bool							IsTrespassing() const;
	virtual void							SetTrespassing();
	virtual bool							GetUsesAttackPercents() const;
	virtual void							GetPowerAttackPercent(uint32_t);
	virtual float							CalculateWalkSpeed();
	virtual float							CalculateRunSpeed();
	virtual void							ModifyDispositionTowardActor(Actor* apActor, float afValue);
	virtual float							GetDispositionModifierTowardActor(Actor* apActor) const;
	virtual void							RemoveDispositionModifierForActor(Actor* apActor);
	virtual void							SetActorStartingPosition();
	virtual bool							HasBeenAttacked() const;
	virtual void							SetBeenAttacked(bool abVal);
	virtual void							HitMe(HitData* apHitData, float afHealthDmg, float afBlockDTMod, NiPoint3& unk, NiPoint3& arWeaponPos);
	virtual void							UseSkill0(ActorValue::Index aeSkill, uint32_t);
	virtual void							UseSkill1(ActorValue::Index aeSkill, uint32_t aeAction, float afModifier);
	virtual void							UseSkill2(ActorValue::Index aeSkill, float afUsage, TESSkill* apSkill);
	virtual void							RewardExperience();
	virtual float							GetBaseValueOverride(ActorValue::Index aeIndex, bool& arFound);
	virtual bool							SetBaseValueOverride(ActorValue::Index aeIndex, float afValue);
	virtual void							AddGunWobble();
	virtual void							AddPerk(BGSPerk* apPerk, uint8_t aucRank, bool abTeammate);
	virtual void							RemovePerk(BGSPerk* apPerk, bool abTeammate);
	virtual uint8_t							GetPerkRank(BGSPerk* apPerk, bool abTeammate);
	virtual void							AddPerkEntry(BGSEntryPointPerkEntry* apPerkEntry, bool abTeammate);
	virtual void							RemovePerkEntry(BGSEntryPointPerkEntry* apPerkEntry, bool abTeammate);
	virtual BSSimpleList<BGSPerkEntry*>*	GetPerkEntryList(uint8_t ucID, bool abTeammate) const;
	virtual void							StartAnimOn1stPerson(uint16_t aeAnimGroup, uint32_t aeActionFlags);
	virtual bool							IsImmobile() const;
	virtual void							HandleHealthDamage(Actor* apAttacker, float afDamage);
	virtual void							AttackedBy(Actor* apAttacker, ActiveEffect* apEffect = nullptr);
	virtual void							HandleBlockedAttack(float afFullDamage, float afPercentageBlocked, Actor* apBlockingActor, ArrowProjectile* apArrow);
	virtual void							PrecacheData();
	virtual void							ProcessTracking(float afTime);
	virtual void							ProcessEmotions(float afTime);
	virtual void							CreateActorMover();
	virtual void							DestroyActorMover();
	virtual NiPoint3						CalculateAnticipatedLocation(float afTime) const;

	struct ALIGN4 _VisibilityFlags {
		enum Flags : uint32_t {
			FRUSTUM		= 1u << 0,
			OCCLUSION	= 1u << 1,
			FADED		= 1u << 2,
			ALL			= 7
		};

		bool bFrustum	: 1;
		bool bOcclusion : 1;
		bool bFaded		: 1;
	};
	using VisibilityFlags = _VisibilityFlags::Flags;

	struct ALIGN4 _RuntimeFlags {
		enum Flags : uint32_t {
			MAJOR_CRIME_COUNT_MASK	= 0x3FFFFFFF,
			SHOW_SUBTITLES			= 0x40000000,
			REF_ESSENTIAL			= 0x80000000,
		};

		uint32_t	uiMajorCrimeCount	: 30;
		BOOL		bShowSubtitles		: 1;
		BOOL		bRefEssential		: 1;
	};
	using RuntimeFlags = _RuntimeFlags::Flags;

	bhkRagdollController*				pRagdollController;
	bhkRagdollPenetrationUtil*			pRagdollPenetrationUtil;
	uint32_t							unk0B4;
	float								flt0B8;
	bool								bIsAIProcessing;
	Actor*								pKiller;
	bool								bMurderAlarm;
	float								fCheckMyDeadBodyTimer;
	float								fCheckDeadBodyTimer;
	ModifierList						kScriptModifiers;
	ModifierList						kSetActorValues;
	bool								bBlocking;
	BSSimpleList<uint32_t>				kCastPowers;
	BSSimpleList<DispositionModifier*>	kDispositionModifiers;
	bool								bIsInCombat;
	Bitfield8							ucJIPActorFlags1;
	Bitfield8							ucJIPActorFlags2;
	Bitfield8							ucJIPActorFlags3;
	uint32_t							eLifeState;
	uint32_t							eCriticalStage;
	uint32_t							uiQueuedAttackAnim;
	float								fLastUpdate;
	bool								bDeadFlag;
	uint8_t								byte119;
	uint16_t							usJIP11A;
	Bitfield<_VisibilityFlags>			uiVisibilityMask;
	uint32_t							hoursPassed120;
	bool								bForceRun;
	bool								bForceSneak;
	bool								bForceUpdateQuestTarget;
	bool								bSearchingInCombat;
	Actor*								pCombatTarget;
	BSSimpleArray<Actor*>*				pCombatTargets;
	BSSimpleArray<Actor*>*				pCombatAllies;
	bool								bCaughtPickpocketing;
	uint8_t								byte135;
	uint16_t							usJIP136;
	uint32_t							uiPickPocketCaughtDate;
	uint32_t							uiMinorCrimeCount;
	Bitfield<_RuntimeFlags>				uiRuntimeFlags;
	bool								bIgnoreCrime;
	bool								bBufferedPackageEvaluation;
	bool								bIsResetAI;
	bool								byte147;
	TESActorBase*						pTemplateActorBase;
	bool								bInWater;
	bool								bIsSwimming;
	bool								bAddToShadowSceneNode;
	uint32_t							iActionValue;
	float								fTimerOnAction;
	float								fHeadTrackTimer;
	bool								bWasInFrustum;
	uint8_t								bShouldRotateToTrack;
	uint16_t							usJIP15E;
	NiPoint3							kEditorLocationCoords;
	float								fEditorLocationZRot;
	TESForm*							pEditorLocation;
	bool								bSetOnDeath;
	bool								bContainerReset;
	uint16_t							usJIP176;
	float								fCachedSpread_Projectile;
	float								fCachedSpread_Scoped;
	float								fCachedSpread_WeaponModel;
	float								fCachedSpread_VATS;
	float								flt188;
	bool								bFootIKInRange;
	bool								bIsTeammate;
	bool								bLightingUpdatedNonMoving;
	bool								byte18F;
	ActorMover*							pActorMover;
	uint32_t							unk194;
	TESPackage*							pPackage198;
	float								fRadiationEffectivness;
	ContinuousBeamProjectile*			pContinuousBeamProjectile;
	uint32_t							eEmotion;
	uint32_t							uEmotionValue;
	uint32_t							eSitSleepState;
	bool								bIsTurret;
	bool								bForceHit;
	bool								byte1B2;
	bool								byte1B3;

	bool IsUnconscious() const;

	bool IsMoving() const;

	bool IsInCombat() const;

	bool IsPlayerTeammate() const;

	FACTION_RELATION GetFactionRelation(Actor* apTarget, bool& arbPlayerEnemy) const;

	void UpdateAnimationMovement(Animation* apAnimation, float afDelta = 0.f);

	void PickAnimations(float afWalkSpeed, float afTurnSpeed);

	float GetMaxCarryWeight() const;

	void SetWalkRunFlagDirty();

	void HandlePostAnimationActions();

	void UpdateMagic(bool abSleeping, bool abCompanionMenu);
};

ASSERT_SIZE(Actor, 0x1B4);
ASSERT_OFFSET(Actor, pRagdollController, 0xAC);
ASSERT_OFFSET(Actor, pActorMover, 0x190);