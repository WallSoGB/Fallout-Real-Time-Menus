#pragma once

#include "TESCondition.hpp"
#include "TESForm.hpp"

class BGSIdleCollection;
class TESCombatStyle;
class TESObjectREFR;
class TESIdleForm;
class TESTopic;
class Actor;
class NiPoint3;
class TESPackageData;

union PackageObject {
	TESForm*		pForm;
	TESObjectREFR*	pReference;
	uint32_t		uiObjectCode;
};

struct PackageSchedule {
	enum {
		DAY_ANY	 = 0,
		TIME_ANY = 0xFF,
	};

	enum class Month : uint8_t {
		JANUARY = 0,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER,

		SPRING,	// March, April, May
		SUMMER,	// June, July, August
		AUTUMN,	// September, October, November
		WINTER,	// December, January, February

		ANY = 0xFF,
	};

	enum class WeekDay : uint8_t {
		SUNDAYS = 0,
		MONDAYS,
		TUESDAYS,
		WEDNESDAYS,
		THURSDAYS,
		FRIDAYS,
		SATURDAYS,

		WEEKDAYS,
		WEEKENDS,

		MWF,
		TT,

		ANY = 0xFF
	};

	int8_t	eMonth;
	int8_t	eDayOfWeek;
	int8_t	cDate;
	int8_t	cHour;
	int32_t	iDuration;
};

ASSERT_SIZE(PackageSchedule, 0x8);

struct PackageLocation;

struct PackageTarget;

struct PackageEventAction {
	TESIdleForm*	pIdle;
	Script*			pScript;
	TESTopic*		pTopic;
	uint32_t		eType;
};

ASSERT_SIZE(PackageEventAction, 0x10);

class TESPackage : public TESForm {
public:
	TESPackage();
	~TESPackage();

	virtual bool		IsReadyToRun(Actor* apActor);
	virtual bool		IsActorAtLocation(Actor* apActor, bool abIgnoreDistance, float afExtraRadius, bool abInFurniture);
	virtual bool		IsActorAtSecondLocation(Actor* apMobileObject, Actor* apPackageowner, bool abIgnoreDistance, float afExtraRadius, bool abInFurniture);
	virtual bool		IsActorAtRefTarget(Actor* apActor, int32_t aiExtraRadius);
	virtual bool		IsTargetAtLocation(Actor* apActor, int32_t aiExtraRadius);
	virtual uint16_t	GetSaveSize();
	virtual uint16_t	SaveGameTESAlt();
	virtual uint16_t	LoadGameTESAlt();
	virtual void		InitLoadGameTES();
	virtual bool		IsPackageOwner(Actor* apActor = nullptr);

	struct ALIGN4 _PackageFlags {
		enum Flags : uint32_t {
			OFFERS_SERVICES				= 1u << 0,
			MUST_REACH_LOCATION			= 1u << 1,
			MUST_COMPLETE				= 1u << 2,
			LOCK_DOORS_AT_START			= 1u << 3,
			LOCK_DOORS_AT_END			= 1u << 4,
			LOCK_DOORS_AT_LOCATION		= 1u << 5,
			UNLOCK_DOORS_AT_START		= 1u << 6,
			UNLOCK_DOORS_AT_END			= 1u << 7,
			UNLOCK_DOORS_AT_LOCATION	= 1u << 8,
			CONTINUE_IF_PC_NEAR			= 1u << 9,
			ONCE_PER_DAY				= 1u << 10,
			CREATED						= 1u << 11,
			SKIP_FALLOUT_BEHAVIOR		= 1u << 12,
			ALWAYS_RUN					= 1u << 13,
			SCRIPT_PACKAGE				= 1u << 14,
			NEVER_RUN					= 1u << 15,
			WAITING						= 1u << 16,
			ALWAYS_SNEAK				= 1u << 17,
			ALLOW_SWIMMING				= 1u << 18,
			ALLOW_FALLS					= 1u << 19,
			NO_HEAD_TRACK				= 1u << 20,
			WEAPONS_UNEQUIPPED			= 1u << 21,
			DEFENSIVE_COMBAT			= 1u << 22,
			WEAPONS_DRAWN				= 1u << 23,
			NO_IDLE_ANIMS				= 1u << 24,
			PRETEND_IN_COMBAT			= 1u << 25,
			CONTINUE_DURING_COMBAT		= 1u << 26,
			NO_COMBAT_ALERT				= 1u << 27,
			NO_WARN_ATTACK_BEHAVIOR		= 1u << 28,
		};

		bool bOffersServices		: 1;
		bool bMustReachLocation		: 1;
		bool bMustComplete			: 1;
		bool bLockDoorsAtStart		: 1;
		bool bLockDoorsAtEnd		: 1;
		bool bLockDoorsAtLocation	: 1;
		bool bUnlockDoorsAtStart	: 1;
		bool bUnlockDoorsAtEnd		: 1;
		bool bUnlockDoorsAtLocation : 1;
		bool bContinueIfPCNear		: 1;
		bool bOncePerDay			: 1;
		bool bCreated				: 1;
		bool bSkipFalloutBehavior	: 1;
		bool bAlwaysRun				: 1;
		bool bScriptPackage			: 1;
		bool bNeverRun				: 1;
		bool bWaiting				: 1;
		bool bAlwaysSneak			: 1;
		bool bAllowSwimming			: 1;
		bool bAllowFalls			: 1;
		bool bNoHeadTrack			: 1;
		bool bWeaponsUnequipped		: 1;
		bool bDefensiveCombat		: 1;
		bool bWeaponsDrawn			: 1;
		bool bNoIdleAnims			: 1;
		bool bPretendInCombat		: 1;
		bool bContinueDuringCombat	: 1;
		bool bNoCombatAlert			: 1;
		bool bNoWarnAttackBehavior	: 1;
	};
	using PackageFlags = _PackageFlags::Flags;

	struct _PackageSpecificFlags {
		enum Flags : uint16_t {
			SANDBOX_ALLOW_EATING			= 1u << 0,
			SANDBOX_ALLOW_SLEEPING			= 1u << 1,
			SANDBOX_ALLOW_CONVERSATION		= 1u << 2,
			SANDBOX_ALLOW_IDLE_MARKERS		= 1u << 3,
			SANDBOX_ALLOW_FURNITURE			= 1u << 4,
			SANDBOX_ALLOW_WANDERING			= 1u << 5,
			SANDBOX_LOCATION_IS_LINKED_REF	= 1u << 6,

			SEARCH_ALLOW_BUYING				= 1u << 8,
			SEARCH_ALLOW_KILLING			= 1u << 9,
			SEARCH_ALLOW_STEALING			= 1u << 10,

			GUARD_TARGET_IS_LINKED_REF		= 1u << 1,
			GUARD_REMAIN_NEAR_REFERENCE		= 1u << 2,
		};

		struct ALIGN2 SandboxFlags {
			bool bAllowEating			: 1;
			bool bAllowSleeping			: 1;
			bool bAllowConversation		: 1;
			bool bAllowIdleMarkers		: 1;
			bool bAllowFurniture		: 1;
			bool bAllowWandering		: 1;
			bool bLocationIsLinkedRef	: 1;
		};

		struct ALIGN2 SearchFlags {
			bool						: 7;
			bool bAllowBuying			: 1;
			bool bAllowKilling			: 1;
			bool bAllowStealing			: 1;
		};

		struct ALIGN2 GuardFlags {
			bool bTargetIsLinkedRef		: 1;
			bool bRemainNearReference	: 1;
		};

		union {
			SandboxFlags	uiSandbox;
			SearchFlags		uiSearch;
			GuardFlags		uiGuard;
		};
	};
	using PackageSpecificFlags	= _PackageSpecificFlags::Flags;
	using SandboxPackageFlags	= _PackageSpecificFlags::SandboxFlags;
	using SearchPackageFlags	= _PackageSpecificFlags::SearchFlags;
	using GuardPackageFlags		= _PackageSpecificFlags::GuardFlags;

	struct _TargetType {
		enum Type {
			REFERENCE	= 0,
			OBJECT		= 1,
			TYPE		= 2,
			LINKEDREF	= 3,
		};
	};
	using TargetType = _TargetType::Type;

	struct Data {
		Bitfield<_PackageFlags>				uiPackageFlags;
		int8_t								eType;
		Bitfield16							usBehaviorFlags;
		Bitfield<_PackageSpecificFlags>		usSpecificFlags;
	};

	int32_t				eProcedureType;
	Data				kData;
	TESPackageData*		pPackageData;
	PackageLocation*	pPackageLocation;
	PackageTarget*		pPackageTarget;
	BGSIdleCollection*	pIdleCollection;
	PackageSchedule		kPackSched;
	TESCondition		kConditions;
	TESCombatStyle*		pCombatStyle;
	PackageEventAction	kOnBegin;
	PackageEventAction	kOnEnd;
	PackageEventAction	kOnChange;
	uint32_t			uiRefCount;

	PackageLocation* GetPackageSecondLocation() const;
};

ASSERT_SIZE(TESPackage, 0x80);