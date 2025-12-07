#pragma once

#include "TESObjectREFR.hpp"
#include "BaseProcess.hpp"

class CFilter;
struct PackageLocation;
class TESTopic;
class DialogueResponse;

class MobileObject : public TESObjectREFR {
public:
	MobileObject();
	virtual ~MobileObject();

	virtual bool						MoveToHigh();
	virtual bool						MoveToLow();
	virtual bool						MoveToMiddleLow();
	virtual bool						MoveToMiddleHigh();
	virtual bhkCharacterController*		Move(float afTimeDelta, NiPoint3& arDeltaMove, uint8_t abDefer);
	virtual bhkCharacterController*		Jump();
	virtual void						SetAvoidanceDisabled(bool abVal);
	virtual void						Process(float afTimeDelta);
	virtual void						ChangeProcessLevel();
	virtual void						UpdateActorInDialogue(float afTime, DialogueResponse* apResponse, bool);
	virtual void						UpdateAnimationAlt(float = 0.f);
	virtual bool						GetBufferedPackageEvaluation() const;
	virtual bool						Unk_9C();
	virtual bool						IsNPCInCombat(bool abNoSearch) const;
	virtual void						SetDialoguewithPlayer(bool abInDialog);
	virtual void						InitiateDialogueFromPackage(TESPackage* apPackage);
	virtual bool						InitiateDialogue(Actor* apTalkingActor, PackageLocation& arSubjectLocation, PackageLocation& arTargetLocation, bool abHeadTrack, bool abAllowMovement, bool a7, TESTopic* apTopic, bool a9, bool a10);
	virtual float						SpeakSoundFunction(const char* apSoundFile, BSSoundHandle* apSoundHandle, uint32_t aeEmotionType, uint32_t auiEmotionValue, uint32_t auiResponseLength, TESIdleForm* apSpeakerAnimation, TESIdleForm* apListenerAnimation, Actor* apListener, bool abSetEmotion, bool ab2D, bool abQueue, bool abLip, bool abForceSpeakerAnim);
	virtual void						EndDialogue();
	virtual void						SetRunOnceExtraPackage(TESPackage* apPackage, uint8_t aucDay);
	virtual bool						HasEditorLocation();
	virtual TESWorldSpace*				GetEditorLocationWorld();
	virtual TESObjectCELL*				GetEditorLocationInteriorCell();
	virtual NiPoint3					GetEditorLocationCoord();
	virtual bool						DetachCharController();
	virtual bool						RemoveCharController();
	virtual void						SetPosition(const NiPoint3& arPosition);
	virtual void						GetLinearVelocity(NiPoint3& arVelocity) const;
	virtual void						EvaluateDetection(float afDelta);
	virtual float						GetDetectionTimer() const;
	virtual void						ReduceDetectionTimer();
	virtual float						GetHeading(bool arg1) const;
	virtual void						SetHeadingToRef(TESObjectREFR* apRef);
	virtual void						SetHeading(float afValue);
	virtual void						GetCurrentTarget();
	virtual bool						IsAtPoint(NiPoint3& arPoint, float afRadius, bool abExpandRadius, bool abAlwaysTestHeight);
	virtual uint32_t					GetEmotion() const;
	virtual void						SetEmotion(uint32_t);
	virtual uint32_t					GetEmotionValue() const;
	virtual void						SetEmotionValue(uint32_t);
	virtual void						SetSize(float afSize);
	virtual float						GetAcrobatics() const;
	virtual bool						IsDying() const;
	virtual bool						IsTransient() const;
	virtual void						SetDisplayGeometry(bool abShow);
	virtual void						PutCreatedPackage(TESPackage* apPackage, bool abTempPackage, bool abIsaCreatedPackage);
	virtual void						Update(float afDelta);
	virtual void						ClearCombatData();
	virtual Actor*						SetUpTalkingActivatorActor(Actor* apActor, Actor*&);

	BaseProcess*	pCurrentProcess;
	TESObjectREFR*	pActivatorRef;
	MobileObject*	pDialogueItemTarget;
	float			fUpdateTargetTimer;
	float			unk78;
	bool			bSpeakingDone;
	bool			bTalkingToPlayer;
	bool			bUnk7E;
	bool			bSoundFileDone;
	bool			bVoiceFileDone;
	bool			bTalkingThroughActivator;
	bool			bChangingProcess;
	bool			bDoNotRunSayToCallBack;
	uint8_t			ucSoundCallbackSet;
	bool			bSoundCallbackSuccess;
	Bitfield8		ucExtraInfoGeneralTopicDataElementFlags;
	bool			bHasLowLevelProcessing;

	BaseProcess* GetCurrentProcess() const;

	bool IsInDialoguewithPlayer() const;

	float GetLooking() const;

	void ModHeading(float afHeading);
	void ModLooking(float afLooking);
};

ASSERT_SIZE(MobileObject, 0x88);