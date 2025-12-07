#pragma once

#include "Gamebryo/NiTPointerList.hpp"
#include "Gamebryo/NiPoint3.hpp"

class TESSound;
class BSSoundMessage;

class BSGameSound {
public:
	BSGameSound();
	virtual				~BSGameSound();
	virtual bool		IsPlaying() const;
	virtual bool		IsInDecay() const;
	virtual bool		IsPaused() const;
	virtual float		GetVolume() const;
	virtual void		SetInaudible(bool abSet);
	virtual void		SetEnvironmentType(uint32_t aeEnvironmentType);
	virtual bool		ComparePriority(uint32_t auiOtherPriority);
	virtual bool		PriorityTest(BSGameSound* apOtherSound);
	virtual bool		Open();
	virtual bool		Prepare();
	virtual void		Copy(BSGameSound* apSource, bool abPrepare);
	virtual bool		Play(bool abLoop);
	virtual bool		Pause();
	virtual bool		Stop();
	virtual bool		SetVolume(float afVolume);
	virtual void		UpdateEmitterPosition();
	virtual bool		Update(bool);
	virtual bool		Unk_12(int); // Stub
	virtual bool		SetEmitterPosition(NiPoint3 akPos);
	virtual bool		SetEmitterPositionF(float afPosX, float afPosY, float afPosZ);
	virtual void		GetEmitterPosition(NiPoint3& arPos) const;
	virtual void		SetEmitterFront(float, float, float); // Stub
	virtual void		SetMinMaxDistance(float afMin, float afMax);
	virtual void		SetAttenuationCurve(uint16_t ausVal0, uint16_t ausVal1, uint16_t ausVal2, uint16_t ausVal3, uint16_t ausVal4);
	virtual bool		SetFrequency(float afFrequency);
	virtual float		GetFrequency() const;
	virtual void		Seek(uint32_t auiMilliseconds);

	struct ALIGN4 _TypeFlags {
		enum Flags : uint32_t {
			IS_2D							= 1u << 0,
			IS_3D							= 1u << 1,
			VOICE							= 1u << 2,
			FOOTSTEPS						= 1u << 3,
			LOOP							= 1u << 4,
			SYSTEM_SOUND					= 1u << 5,
			RANDOM_FREQUENCY_SHIFT			= 1u << 6,
			UNKBIT7							= 1u << 7, // Casing, explosion, gunfire, block
			ONE_SHOT						= 1u << 8,
			UNKBIT9							= 1u << 9,
			UNKBIT10						= 1u << 10,
			MUSIC							= 1u << 11,
			REGION							= 1u << 12,
			MAYBE_UNDERWATER				= 1u << 13,
			IMPACT							= 1u << 14, // Projectile impacts, disable, explosion impacts, collision sounds
			CACHED							= 1u << 15,
			DONT_CACHE						= 1u << 16,
			IS_2D_GUNFIRE					= 1u << 17, // Gunfire 2D
			FIRST_PERSON					= 1u << 18,
			MODULATED						= 1u << 19,
			RADIO							= 1u << 20,
			IGNORE_TIMESCALE				= 1u << 21,
			UNKBIT22						= 1u << 22, // Prevents being stopped? Is muted instead. Used by radio
			MUSIC_B23						= 1u << 23,
			SKIP_TIME						= 1u << 24, // Used by radios to skip to current time
			ENVELOPE_FAST					= 1u << 25,
			ENVELOPE_SLOW					= 1u << 26,
			IS_2D_RADIUS					= 1u << 27,
			BEAM_EMITTER					= 1u << 28,
			UNKBIT29						= 1u << 29,
			ANIMATION_DRIVEN				= 1u << 30,
			UNKBIT31						= 1u << 31, // Player's equip sound
		};

		bool b2D						: 1;
		bool b3D						: 1;
		bool bVoice						: 1;
		bool bFootsteps					: 1;
		bool bLoop						: 1;
		bool bSystemSound				: 1;
		bool bRandomFrequencyShift		: 1;
		bool bUnkBit7					: 1;
		bool bOneShot					: 1;
		bool bUnkBit9					: 1;
		bool bUnkBit10					: 1;
		bool bMusic						: 1;
		bool bRegion					: 1;
		bool bMaybeUnderwater			: 1;
		bool bImpact					: 1;
		bool bIsCached					: 1;
		bool bDontCache					: 1;
		bool b2DGunfire					: 1;
		bool bFirstPerson				: 1;
		bool bModulated					: 1;
		bool bRadio						: 1;
		bool bIgnoreTimescale			: 1;
		bool bUnkBit22					: 1;
		bool bMusicB23					: 1;
		bool bSkipTime					: 1;
		bool bEnvelopeFast				: 1;
		bool bEnvelopeSlow				: 1;
		bool b2DRadius					: 1;
		bool bBeamEmitter				: 1;
		bool bUnkBit29					: 1;
		bool bAnimationDriven			: 1;
		bool bUnkBit31					: 1;
	};
	using TypeFlags = _TypeFlags::Flags;

	struct ALIGN4 _StateFlags {
		enum StateFlags : uint32_t {
			OPENED			= 1u << 0,
			UNK_1			= 1u << 1,
			UNK_2			= 1u << 2,
			UNK_3			= 1u << 3,
			STOPPED			= 1u << 4,
			PLAYING			= 1u << 5,
			PAUSED			= 1u << 6,
			UNK_7			= 1u << 7,
			VALID			= 1u << 8,
			FADING			= 1u << 9,
			MUTED			= 1u << 10,
			READY			= 1u << 11,
			OPENING			= 1u << 12,
			PLAYED			= 1u << 13,
			UNK_14			= 1u << 14,
			IS_VALID_BEAM	= 1u << 15,
			PLAY_LATER		= 1u << 16,
			INAUDIBLE		= 1u << 17,
			UNK_18			= 1u << 18,
			DECAY			= 1u << 19,
			EXPLICIT_PAUSE	= 1u << 20,
		};

		bool bOpened		: 1;
		bool bUnk1			: 1;
		bool bUnk2			: 1;
		bool bUnk3			: 1;
		bool bStopped		: 1;
		bool bPlaying		: 1;
		bool bPaused		: 1;
		bool bUnk7			: 1;
		bool bValid			: 1;
		bool bFading		: 1;
		bool bMuted			: 1;
		bool bReady			: 1;
		bool bOpening		: 1;
		bool bPlayed		: 1;
		bool bUnk14			: 1;
		bool bIsValidBeam	: 1;
		bool bPlayLater		: 1;
		bool bInaudible		: 1;
		bool bUnk18			: 1;
		bool bInDecay		: 1;
		bool bExplicitPause : 1;
	};
	using StateFlags = _StateFlags::StateFlags;

	enum Flags00C {
		Flag_MuteWhenSubmerged		= 1u << 0,
		Flag_StartAtRandomPosition	= 1u << 1,
	};

	enum EnvironmentType { // Shifted by 1 compared to DSound ones
		ENVIRONMENT_NONE,
		ENVIRONMENT_DEFAULT,
		ENVIRONMENT_GENERIC,
		ENVIRONMENT_PADDEDCELL,
		ENVIRONMENT_ROOM,
		ENVIRONMENT_BATHROOM,
		ENVIRONMENT_LIVINGROOM,
		ENVIRONMENT_STONEROOM,
		ENVIRONMENT_AUDITORIUM,
		ENVIRONMENT_CONCERTHALL,
		ENVIRONMENT_CAVE,
		ENVIRONMENT_ARENA,
		ENVIRONMENT_HANGAR,
		ENVIRONMENT_CARPETEDHALLWAY,
		ENVIRONMENT_HALLWAY,
		ENVIRONMENT_STONECORRIDOR,
		ENVIRONMENT_ALLEY,
		ENVIRONMENT_FOREST,
		ENVIRONMENT_CITY,
		ENVIRONMENT_MOUNTAINS,
		ENVIRONMENT_QUARRY,
		ENVIRONMENT_PLAIN,
		ENVIRONMENT_PARKINGLOT,
		ENVIRONMENT_SEWERPIPE,
		ENVIRONMENT_UNDERWATER,
		ENVIRONMENT_SMALLROOM,
		ENVIRONMENT_MEDIUMROOM,
		ENVIRONMENT_LARGEROOM,
		ENVIRONMENT_MEDIUMHALL,
		ENVIRONMENT_LARGEHALL,
		ENVIRONMENT_PLATE
	};

	typedef void(*SOUND_CALLBACK)(uint32_t, uint32_t);

	uint32_t							uiSoundID;
	Bitfield<_TypeFlags>				uiTypeFlags;
	Bitfield32							uiFormFlags;
	Bitfield<_StateFlags>				uiStateFlags;
	uint32_t							uiDuration;
	uint16_t							usStaticAttenuation;
	uint16_t							usReverbAttenuation;
	uint16_t							usSystemAttenuation;
	uint16_t							usDistanceAttenuation;
	uint16_t							usFaderAttenuation;
	uint16_t							unk022;
	float								fVolume;
	float								fBeginTime;
	float								fEndTime;
	Bitfield32							uiWeatherFlags;
	uint16_t							usSampleRate; // Samples per second
	char								cFilePath[254];
	TESSound*							pSourceSound;
	float								fFrequencyMod;
	float								fMaxAttenuationDist;
	float								fMinAttenuationDist;
	uint32_t							uiSourceID;
	uint32_t							uiAlternateID;
	uint32_t							uiFileChunkBytes;
	float								fListenerDistance;
	EnvironmentType						eEnvironmentType;
	uint8_t								ucRandFrequencyShift;
	uint16_t							usSamplingFreq;
	uint32_t							uiLastPlayedTime;
	uint32_t							uiAudioDelayMS;
	SOUND_CALLBACK						pSoundCompletionCallback;
	SOUND_CALLBACK						pSoundPlayCallback;
	uint32_t							uiSoundCompletionTime;
	uint32_t							uiSoundPlayTime;
	uint32_t							uiEnvelopeStart;
	uint32_t							uiEnvelopeEnd;
	NiPoint3							kBeamEndPos;
	uint32_t							uiPriority;
	NiTPointerList<BSSoundMessage*>		kMessageList;
};

ASSERT_SIZE(BSGameSound, 0x198);