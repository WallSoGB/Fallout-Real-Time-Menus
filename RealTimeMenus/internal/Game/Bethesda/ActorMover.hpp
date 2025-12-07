#pragma once

#include "Gamebryo/NiSmartPointer.hpp"
#include "PathingLocation.hpp"

class BGSLoadFormBuffer;
class PathingRequest;
class PathingSolution;
class ActorPathingMessageQueue;
class ActorPathHandler;
class Actor;

class ActorMover {
public:
	ActorMover();
	
	virtual				~ActorMover();
	virtual void		SetMovementFlag(uint16_t ausFlag);
	virtual void		ClearMovementFlag(uint16_t ausFlag);
	virtual void		SetForcedMovementFlags(uint16_t ausFlag);
	virtual void		ClearForcedMovementFlags();
	virtual void		Update(float afDelta);
	virtual void		HandleTurnAnimationTimer(float timePassed);
	virtual uint32_t	GetMovementFlags() const;
	virtual uint32_t	GetMovementState() const;
	virtual void		GetOverrideMovementVector();
	virtual void		SaveGame();
	virtual void		LoadGame();
	virtual void		InitLoadGame(BGSLoadFormBuffer* apBuffer);
	virtual void		FinishLoadGame(BGSLoadFormBuffer* apBuffer);
	virtual void		Reset();

	struct ALIGN4 _MovementFlags {
		// Flags are 16 bit, but ActorMover uses 32 bit for storage
		enum Flags : uint16_t {
			// Input states
			FORWARD			= 1u << 0,
			BACKWARD		= 1u << 1,
			LEFT			= 1u << 2,
			RIGHT			= 1u << 3,
			TURN_LEFT		= 1u << 4,
			TURN_RIGHT		= 1u << 5,

			NON_CONTROLLER	= 1u << 6,

			// Movement states
			WALKING			= 1u << 8,
			RUNNING			= 1u << 9,
			SNEAKING		= 1u << 10,
			SWIMMING		= 1u << 11,
			JUMP			= 1u << 12,
			FLYING			= 1u << 13,
			FALL			= 1u << 14,
			SLIDE			= 1u << 15,

			INPUT_MASK		= 0x003F,
			MOVEMENT_MASK	= 0xFF00,
		};

		bool bForward		: 1;
		bool bBackward		: 1;
		bool bLeft			: 1;
		bool bRight			: 1;
		bool bTurnLeft		: 1;
		bool bTurnRight		: 1;

		bool bNonController : 1;

		bool 				: 1;

		bool bWalking		: 1;
		bool bRunning		: 1;
		bool bSneaking		: 1;
		bool bSwimming		: 1;
		bool bJump			: 1;
		bool bFlying		: 1;
		bool bFall			: 1;
		bool bSlide			: 1;
	};
	using MovementFlags = _MovementFlags::Flags;

	enum State {
		TURNING = 4,
		CLEARED = 6, // Finished? Cancelled?
	};

	NiPoint3							kLookAtPosition;
	NiPoint3							kOverrideMovement;
	NiPointer<PathingRequest>			spPathingRequest;			
	NiPointer<PathingSolution>			spPathingSolution;
	ActorPathHandler*					pPathHandler;
	Actor*								pActor;							
	uint32_t							uiDoorRefIDForPathing;				
	NiPointer<ActorPathingMessageQueue>	spPathingMsgQueue;
	Bitfield<_MovementFlags>			uiMovementFlags;
	Bitfield<_MovementFlags>			uiForcedMovementFlags;
	Bitfield<_MovementFlags>			uiMovementStateFlags;
	int16_t								sFistNodeIndex;
	int16_t								sLastNodeIndex;
	PathingLocation						kPathingLocation;		
	uint32_t							eState6C;							
	bool								bPathingFailed;
	bool								bForcedFlags;
	bool								bTargetBlocked;
	bool								bHasLookAtTarget;
	bool								bIsWaitingOnPath;
	bool								bUpgradePathHandler;
	bool								byte76;							
	bool								bIsOverrideMovement;				
	bool								byte78;							
	uint32_t							uiTime7C;							
	uint32_t							uiTime80;							
	uint32_t							uiCount84;		
};

ASSERT_SIZE(ActorMover, 0x88);