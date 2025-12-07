#pragma once

#include "MiddleHighProcess.hpp"
#include "BSSoundHandle.hpp"

class NiBSBoneLODController;
class MuzzleFlash;
class DetectionEvent;
class Actor;
class bhkSimpleShapePhantom;
class DialogueItem;
class LipSynchAnim;
class MoveToMarker;
class NiBillboardNode;
class DetectionState;

NiSmartPointer(PathingRequest);
NiSmartPointer(ActorPathingMessageQueue);

class HighProcess : public MiddleHighProcess {
public:
	HighProcess();
	~HighProcess();

	virtual BSSimpleList<Actor*>*	GetActorsToFleeFrom() const;

	struct CachedActorValues
	{
		struct Value {
			bool	bIsDirty;
			float	fValue;
		};

		Value kAVs[77];
	};

	struct HeadTracking {
		union {
			TESObjectREFR* pHeadTrackingTargets[HEAD_TRACK_TYPE_COUNT];
			struct {
				TESObjectREFR* pTargetDefault;
				TESObjectREFR* pTargetAction;
				TESObjectREFR* pTargetCombat;
				TESObjectREFR* pTargetDialog;
				TESObjectREFR* pTargetScript;
				TESObjectREFR* pTargetProcedure;
			};
		};
		uint8_t ucHeadTrackingTargetFlags[HEAD_TRACK_TYPE_COUNT];
	};

	enum FadeState {
		FADE_NORMAL			= 0,
		FADE_IN				= 1,
		FADE_OUT			= 2,
		FADE_TELEPORT_IN	= 3,
		FADE_TELEPORT_OUT	= 4,
		FADE_OUT_DISABLE	= 5,
		FADE_OUT_DELETE		= 6,
		FADE_MOVE_TO_MARKER = 7,
		FADE_STATE_COUNT	= 8,
	};

	enum ActorSoundHandle {
		VOICE = 0,
		TORCH = 1,
		COUNT = 2,
	};

	BSSimpleList<DetectionState*>*			pDetectedActors;
	BSSimpleList<DetectionState*>*			pDetectingActors;
	BSSimpleList<Actor*>*					pTalkedToActors;
	BSSimpleList<DetectionState*>*			pQueuedDetectedActors;
	BSSimpleList<DetectionState*>*			pQueuedDetectingActors;
	bool									bRanDetection;
	BSSimpleList<void*>						kAggroList;
	BSSimpleList<void*>						kGroupsToHelp;
	BSSimpleList<void*>						kTargetsToAdd;
	BSSimpleList<void*>						kSpectators;
	float									fDetectListTimer;
	float									fIdleChatterTimer;
	bool									bSayGoodByePlayer;
	bool									bProcessGreetSayTo;
	float									fAITalkToSameNPCTimer;
	TESObjectREFR*							pLastDetected;
	float									fDialogueDelayTimer;
	TESObjectREFR*							pUnkRef2AC;
	float									fMaxAlpha;
	float									fPackageEvalTimer;
	float									fUseItemTimer;
	float									fHoldAttackTimer;
	uint16_t								usAttachedArrows;
	uint16_t								usNumVolleyShots;
	uint16_t								usFiredBursts;
	bool									bCheckDeadTalk;
	bool									bSkippedUpdate;
	float									fSocialTimerForConversations;
	uint32_t								unk2CC;
	float									fDelayTimer;
	float									fDistanceMoved;
	float									fTurnTime;
	uint32_t								cLastTurnDir;
	float									fEvaluateAcquireTimer;
	NiBSBoneLODController*					pBoneLOD;
	int32_t									iLastBoneLOD;
	int16_t									usAnimAction;
	BSAnimGroupSequence*					pAnimSequence;
	bool									bForceFireWeapon;
	float									fDetectionTimer;
	int16_t									usLastDetection;
	NiPoint3								kLastWeaponPos;
	TESObjectREFR*							pGreetActor;
	float									fSoundDelay;
	BSSoundHandle							kSounds[ActorSoundHandle::COUNT];
	bool									bGreetingFlag;
	float									fGreetingTimer;
	float									fIdleTimer;
	float									fDetectGreetTimer;
	float									fBreathTimer;
	bool									bHeadTrack;
	float									fVoiceTimer;
	bool									bLipQueued;
	bool									bWeaponAlertDrawn;
	float									fAwarePlayerTimer;
	TESIdleForm*							pForcedIdleForm;
	NiRefObjectPtr							spUnkObjects[2];
	NiPointer<KFModel>						spUnkKFModels[2];
	bool									bInDialogMenu;
	DialogueItem*							pGreetTopic;
	uint32_t								eLastSpeakingEmotion;
	Actor*									pDialogTarget;
	bool									bContinuingPackageforPC;
	bool 									bActivateAnim;
	float									fScriptPackageEndTime;
	float									fHealthBarAlphaValue;
	NiPointer<NiBillboardNode>				spHealthBarNode;
	float									fActorHealthPercentage;
	float									fHealthBarEmittanceValue;
	BSSimpleList<Actor*>					kActorsInAggroRadius;
	BSSimpleList<Actor*>					kActorsToFleeFrom;
	uint32_t								iNumberGuardsPursuing;
	bool									bStop;
	float									fReequipArmorTimer;
	bool									bUnequippedArmorToSwim;
	uint32_t								uiHasHealingSpell;
	uint32_t								uiHasHealingPotion;
	BSSimpleList<SpellItem*>*				pLeveledSpells;
	int8_t									cLastTurn;
	bool									bCurrentlyReanimating;
	float									fDetectionModifier;
	float									fDetectionModifierTimer;
	float									fLightAmount;
	float									fLightUpdateTimer;
	LipSynchAnim*							pLipSyncAnim;
	bool									bIsLoadingLipFile;
	bool									bLipFileFailed;
	MuzzleFlash*							pMuzzleFlash;
	uint32_t								uiDetectionCounter;
	DetectionEvent*							pActorsGeneratedDetectionEvent;
	bool									bIgnoringCombat;
	DialogueItem*							pQueuedGreetTopic;
	FadeState								eFadeState;
	float									fFadeAlpha;
	TESObjectREFR*							pTeleportFadeRef;
	MoveToMarker*							pMoveToMarker;
	HeadTracking							kHeadTracking;
	float									fHoldHeadTrackTimer;
	TESObjectREFR*							pLastTarget;
	bool									bForceRotate;
	uint32_t								uiQueuedIdleFlags;
	CachedActorValues*						pCachedActorValues;
	float									fCachedHeight;
	uint32_t								eDefaultIdleAction;
	NiPointer<bhkSimpleShapePhantom>		spDoorDetectionPhantom;
	float									fDetectDoorsForPathingTimer;
	float									fRadReevaluationTimer;
	float									fCurrentRadiation;
	bool									bHasLiveGrenade;
	bool									bForceSpeakToPlayerAfterGettingUp;
	float									fTakeBackTimer;
	BSSimpleList<PathingAvoidNodeArray*>*	pAvoidAreas;
	float									fAvoidWaitTimer;
	DialogueItem*							pDialogueItem454;
	bool									bHiding;
	bool									bIsDoingSayTo;
	PathingRequestPtr						spPathingRequest0;
	PathingRequestPtr						spPathingRequest1;
	ActorPathingMessageQueuePtr				spPathingMessageQueue0;
	ActorPathingMessageQueuePtr				spPathingMessageQueue1;
};

ASSERT_SIZE(HighProcess, 0x46C);