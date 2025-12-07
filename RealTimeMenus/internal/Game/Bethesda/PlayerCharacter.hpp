#pragma once

#include "Character.hpp"
#include "BSSimpleList.hpp"
#include "TeleportPath.hpp"
#include "BSSoundHandle.hpp"
#include "TESRegionList.hpp"
#include "CharacterProgression.hpp"
#include "Gamebryo/NiTMap.hpp"

class AlchemyItem;
class BGSNote;
class BGSQuestObjective;
class bhkMouseSpringAction;
class CameraCaster;
class CombatGroup;
class DialoguePackage;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;
class MagicItem;
class MagicShaderHitEffect;
class MagicTarget;
class MusicMarker;
class QuestObjectiveTargets;
class TESCaravanCard;
class TESClass;
class TESEnchantableForm;
class TESObjectCELL;
class TESObjectREFR;
class TESObjectWEAP;
class TESQuest;
class TESReputation;
class TESTopic;
class Animation;
class BipedAnim;
class NiPointLight;
class TESQuestStageItem;
class TESQuestTarget;
class MapMarkerData;
class BGSPerkEntry;
class PerkRankData;
class TESRegion;
class TESRegionSound;

struct MAP_MARKER_STRUCT {
	MapMarkerData* pData;
	TESObjectREFR* pRef;
};

class PlayerCharacter : public Character {
public:
	PlayerCharacter();
	virtual ~PlayerCharacter();

	struct WobbleNodes {
		NiNode* pWobbleAnimNodes[12];
		NiNode* pWobbleAnimNodes2[12];
	};

	struct CompassTarget {
		CompassTarget() {};
		~CompassTarget() {};

		Actor*	pTarget			= nullptr;
		bool	bIsHostile		= false;
		bool	bIsUndetected	= false;
	};

	struct Unknown {
		uint8_t		unk0;
		uint32_t	unk4;
		bool		bUnk8;
	};


	struct PositionRequest {
		TESWorldSpace*	pWorldSpace;
		TESObjectCELL*	pCell;
		NiPoint3		kPos;
		NiPoint3		kRot;
		bool			bResetWeather;
		void(__cdecl*	pfnCallbackFunc)(void*);
		void*			pCallbackFuncArg;
		TESObjectREFR*	pDestRef;
		TESObjectREFR*	pFastTravelRef;
	};

	enum ModifierGroup : uint32_t {
		MG_MAGIC	= 0,
		MG_SCRIPTS	= 1,
		MG_GAME		= 2,
	};

	enum GrabbingType : uint32_t {
		GRAB_NONE			= 0,
		GRAB_NORMAL			= 1,
		GRAB_INVENTORY_DROP = 2,
		GRAB_TELEKINESIS	= 3,
	};

	// used to flag controls as disabled in disabledControlFlags
	enum {
		kControlFlag_Movement = 1u << 0,
		kControlFlag_Look = 1u << 1,
		kControlFlag_Pipboy = 1u << 2,
		kControlFlag_Fight = 1u << 3,
		kControlFlag_POVSwitch = 1u << 4,
		kControlFlag_RolloverText = 1u << 5,
		kControlFlag_Sneak = 1u << 6,
	};

	enum KillcamMode {
		kKillcamMode_None = 0x0,
		kKillcamMode_PlayerView = 0x1,
		kKillcamMode_Cinematic = 0x2,
	};

	virtual bool							Unk_139(int, int, int, int, int, int, int);
	virtual BSSimpleList<PerkRankData*>*	GetPerkRanks(bool abTeammate);

	Unknown								unk1C8[3];
	PositionRequest*					pPositionRequest;
	TESObjectWEAP*						pWeap1F0;
	float								fAmmoRegenTime;
	uint8_t								byte1F8;
	uint32_t							iSandmanDetectionValue;
	int32_t								iCombatPursue;
	bool								bTravelUseDoor;
	bool								bOnElevator;
	bool								bQuestTargetsNeedRecalculated;
	uint8_t								byte207;
	DialoguePackage*					pClosestConversation;
	bool								bIsPlayerMovingIntoNewSpace;
	BSSimpleList<ActiveEffect*>*		pActiveEffects;
	MagicItem*							pCurrentSpell;
	MagicTarget*						pDesiredTarget;
	CameraCaster*						pCameraCaster;
	uint32_t							unk220;
	uint32_t							unk224;
	uint32_t							unk228;
	float								fTime22C;
	uint32_t							unk230;
	float								time234;
	BSSimpleList<MagicItem*>*			pEatDrinkItems;
	BSSimpleList<TESEnchantableForm*>*	pQueuedWornEnchantments;
	bool								bShowQuestItemsInInventory;
	float								fMagicActorValueModifiers[77];
	float								fScriptActorValueModifiers[77];
	float								fHealthModifier;
	float								fGameActorValueModifiers[77];
	BSSimpleList<uint32_t>				kNotes;
	ImageSpaceModifierInstanceDOF*		pIronSightsDOFInstance;
	ImageSpaceModifierInstanceDOF*		pVATSDOFInstance;
	ImageSpaceModifierInstanceDRB*		pVATSDRBInstance;
	bool								bIsDetected;
	bool								bPreventRegionSoundUpdates;
	BSSimpleList<TESObjectREFR*>		kTeammates;
	TESObjectREFR*						pLastDoorActivated;
	bool								bIsPlayingCombatAttackSound;
	void*								pActionList;
	BSSimpleList<uint32_t>*				pCasinoDataList;
	BSSimpleList<TESCaravanCard*>*		pCaravanCards1;
	BSSimpleList<TESCaravanCard*>*		pCaravanCards2;
	uint32_t							uiCaravanCapsEarned;
	uint32_t							uiCaravanCapsLost;
	uint32_t							uiNumCaravanWins;
	uint32_t							uiNumCaravanLosses;
	uint32_t							uiLargestCaravanWin;
	uint32_t							unk630;
	NiPointer<bhkMouseSpringAction>		spGrabSpring;
	TESObjectREFR*						pGrabbedObject;
	GrabbingType						eGrabType;
	float								fGrabObjectWeight;
	float								fGrabDistance;
	uint8_t								byte648;
	uint8_t								byte649;
	bool								b3rdPerson;
	bool								bIs3rdPerson;
	bool								bThirdPerson;
	bool								bTemp3rdPerson;
	bool								bTemp3rdPersonSwitchBack;
	bool								bIsForceFirstPerson;
	bool								byte650;
	bool								bAlwaysRun;
	bool								bAutoMove;
	bool								byte653;
	int32_t								iSleepHours;
	bool								bIsSleeping;
	bool								byte659;
	bool								byte65A;
	bool								byte65B;
	float								fFOV;
	float								fSecondsRunning;
	float								fSecondsSwimming;
	float								fSecondsSneaking;
	bool								bActorinSneakRange;
	uint8_t								flag66D;
	bool								bCanSleepWait;
	uint8_t								byte66F;
	float								fWorldFOV;
	float								f1stPersonFOV;
	float								fOverShoulderFOV;
	int32_t								iNumberTraining;
	Bitfield8							ucControlFlags;
	bool								bIsWaitingForOpenContainerAnim;
	bool								byte682;
	bool								byte683;
	float								fWaitingForContainerOpenAnimPreventActivateTimer;
	TESObjectREFR*						pForceActivateRef;
	BipedAnim*							p1stPersonBipedAnim;
	Animation*							p1stPersonAnimation;
	NiNodePtr							sp1stPerson3D;
	float								fEyeHeight;
	NiNodePtr							spInventoryPC;
	Animation*							pInventoryAnimation;
	MagicShaderHitEffect*				pMagicShaderHitEffect;
	BSSimpleList<TESTopic*>				kTopics;
	BSSimpleList<TESQuestStageItem*>	kQuestLog;
	TESQuest*							pActiveQuest;
	BSSimpleList<uint32_t>				kQuestObjectiveList;
	BSSimpleList<TESQuestTarget*>		kQuestTargetList;
	bool								bGreetingPlayer;
	float								fPlayerGreetTimer;
	uint32_t							uiHoursToSleep;
	bool								bIsAMurderer;
	int32_t								iAmountStolenSold;
	float								fSortActorDistanceTimer;
	float								fSitHeadingDelta;
	bool								bBeenAttacked;
	MagicItem*							pSelectedSpell;
	TESObjectBOOK*						pSelectedScroll;
	TESObjectREFR*						pPlacedMarker;
	TeleportPath						kPlacedMarkerTarget;
	float								fTimeGrenadeHeld;
	int32_t								iNumAdvance;
	uint32_t							eSkillAdvance;
	TESClass*							pDefaultClass;
	TESClass*							pClassBasedOn;
	uint32_t							uiCrimeCounts[5];
	AlchemyItem*						pPendingPoison;
	bool								bInCharGen;
	uint8_t								byte75D;
	uint8_t								bTelekinesisSelected;
	uint8_t								byte75F;
	TESRegion*							pCurrentRegion;
	TESRegionList						kRegionsList;
	BSSimpleList<TESRegionSound*>		kRegionSounds;
	BSSoundHandle						kHeartBeatSound;
	void*								pInitialStateBuffer;
	uint32_t							uiLastPlayingTimeUpdate;
	uint32_t							uiTotalPlayingTime;
	uint32_t							uiCharacterSeed;
	bool								bAiControlledToPos;
	bool								bAiControlledFromPos;
	bool								bIsDrinkingPlacedWater;
	bool								bIsPackagePlaying;
	bool								bInBorderContainedCell;
	bool								bReturnToLastKnownGoodPosition;
	NiPoint3							kLastKnownGoodPosition;
	TESForm*							pLastKnownGoodLocation;
	NiTPrimitiveArray<TESRegion>*		pBorderRegions;
	void*								pMusicType;
	uint32_t							uiGameDifficulty;
	BOOL								bIsHardcore;
	KillcamMode							eKillCamMode;
	bool								bBeingChased;
	bool								bIsYoung;
	bool								bIsToddler;
	bool								bCanUsePA;
	BSSimpleList<MAP_MARKER_STRUCT*>	kMapMarkers;
	TESWorldSpace*						pMapWorld;
	BSSimpleList<MusicMarker*>			kMusicMarkers;
	MusicMarker*						pCurrMusicMarker;
	float								fFlyCameraHeading;
	float								fFlyCameraPitch;
	NiPoint3							kFlyCameraPos;
	uint32_t							uiSelectedSpellCastSoundID;
	BSSoundHandle						kSelectedSpellCastSound;
	BSSoundHandle						kMagicFailureSounds[6];
	BSSimpleList<TESObjectREFR*>		kDroppedRefs;
	NiTMap<uint32_t, uint8_t>			kRandomDoorSpaces;
	NiPointer<NiPointLight>				spThirdPersonLight;
	NiPointer<NiPointLight>				spFirstPersonLight;
	bool								bInsufficientChargeMessageShown;
	float								fDropAngleMod;
	float								fLastDropAngleMod;
	CharacterProgression				kCharacterProgression;
	BSSimpleList<PerkRankData*>			kPerkRanksPC;
	BSSimpleList<BGSPerkEntry*>			kPerkEntriesPC[74];
	BSSimpleList<PerkRankData*>			kPerkRanksTM;
	BSSimpleList<BGSPerkEntry*>			kPerkEntriesTM[74];
	Actor*								pAutoAimActor;
	NiPoint3							kBulletAutoAim;
	NiNodePtr							spAutoAimNode;
	bool								bIsActorWithinInterfaceManagerPickDistance;
	Actor*								pReticleActor;
	BSSimpleList<CompassTarget*>*		pCompassTargets;
	float								fPipboyLightHeldTime;
	float								fAmmoSwapTimer;
	bool								bShouldOpenPipboy;
	char								byteD55;
	char								byteD56;
	char								byteD57;
	NiPoint3							kPtD58;
	CombatGroup*						pCombatGroup;
	uint32_t							uiTeammateCount;
	float								fCombatYieldTimer;
	float								fCombatYieldRetryTimer;
	WobbleNodes							kWobbleAnims;
	NiPoint3							kCamera1stPos;
	NiPoint3							kCameraPos;
	NiPointer<bhkRigidBody>				spRigidBody;
	bool								bInCombat;
	bool								bUnseen;
	bool								bIsLODApocalypse;
	bool								byteDF3;
	BSSimpleArray<uint32_t>				kRockitLauncherContainer;
	float								fRockItLauncherWeight;
	bool								bHasNightVisionApplied;
	char								byteE09;
	char								byteE0A;
	char								byteE0B;
	TESReputation*						pModifiedReputation;
	int									unkE10;
	int									unkE14;
	float								fKillCamTimer;
	float								fKillCamCooldown;
	char								byteE20;
	char								bIsUsingTurbo;
	char								byteE22;
	char								byteE23;
	float								fLastHelloTime;
	float								fCounterAttackTimer;
	char								byteE2C;
	bool								bIsCateyeEnabled;
	bool								bIsSpottingImprovedActive;
	char								byteE2F;
	float								fItemDetectionTimer;
	NiNode*								pIronSightNode;
	bool								bNoHardcoreTracking;
	bool								bSkipHCNeedsUpdate;
	char								byteE3A;
	char								byteE3B;
	BSSimpleArray<uint32_t>				kHotkeyedWeaponAmmos;
	TESQuest*							pPatch04DebugQuest;

	static constexpr AddressPtr<NiAVObject*, 0x11E07D0> p1stPersonCamera;
	static constexpr AddressPtr<NiAVObject*, 0x11E07D4> p3rdPersonCamera;
	static constexpr AddressPtr<NiAVObject*, 0x11E07D8> p1stPersonBip;

	static constexpr AddressPtr<float, 0x11E0D48> fDialogZoomPercent;

	static PlayerCharacter* GetSingleton();

	NiAVObject* Get3D(bool abFirstPerson) const;

	BipedAnim* GetBiped(bool abFirstPerson) const;

	Animation* GetAnimation(bool abFirstPerson) const;

	void UpdateCamera(bool abIsFinishLoadGame, bool abSkipUpdateLOD);

	bool IsPlayerFollower(Actor* apActor);

	bool HasPipBoyOpen() const;

	void ForceGrenadeHold();

	void UpdateHardcoreMode();

	void UpdateGreetTimer();
	void HandleResetGreetTimer();

	void UpdatePlayerCombat(float afTimeDelta);
};

ASSERT_OFFSET(PlayerCharacter, fLastDropAngleMod, 0x874);
ASSERT_OFFSET(PlayerCharacter, fKillCamTimer, 0xE18);
ASSERT_SIZE(PlayerCharacter, 0xE50);