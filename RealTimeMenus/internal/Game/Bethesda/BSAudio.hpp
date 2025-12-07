#pragma once

class TESSound;
class BSGameSound;

struct AudioTick {
	AudioTick();
	~AudioTick();
	
	struct ALIGN4 _TickFlags {
		enum Flags : uint32_t {
			PAUSED = 1u << 0,
		};

		bool bPaused : 1;
	};

	uint32_t				uiDelta;
	uint32_t				uiLastTick;
	Bitfield<_TickFlags>	uiFlags;

	void Update();
};

class BSAudioListener;

class BSAudio {
public:
	typedef bool(__cdecl* GET_LOOPPOINTS_FN)(TESSound* apSound, uint32_t& arStart, uint32_t& arEnd);

	BSAudio();
	virtual						~BSAudio();
	virtual void				Init(HWND apWindow);
	virtual void				Shutdown();
	virtual void				SetLoopPointCallback(GET_LOOPPOINTS_FN);
	virtual GET_LOOPPOINTS_FN	GetLoopPointCallback() const;
	virtual BSGameSound*		GetGameSound(const char* apPath);
	virtual void				FixSoundPath(char* apPath);
	virtual void 				Unk_07();		// Stub

	bool					bReady;
	bool					bIsInitialized;
	bool					bIsMultithreaded;
	DWORD					dword8;
	BYTE					byteC;
	BSAudioListener*		pListener;
	AudioTick				kTimePassed;
	bool(__cdecl*			pfnGetSoundFromFormIDCallback)(uint32_t auiFormID, const char* apOutFilePath, uint32_t& arFlags, TESSound*& arSound);
	bool(__cdecl*			pfnGetSoundFromEDIDCallback)(const char* apEDID, const char* apOutFilePath, uint32_t& arFlags, TESSound*& arSound);
	bool(__cdecl*			pfnPickSoundFileFromFolder)(const char* apOutFilePath);
	uint32_t(__cdecl*		pfnFillGameSoundProps)(uint32_t* mapKey, TESSound* soundForm, uint32_t* outFlags0C);
	void (__cdecl*			pfnOnPauseCallback)();
	void (__cdecl*			pfnOnUnpauseCallback)();

	static BSAudio* GetSingleton();

	void SetListenerPosition(float afX, float afY, float afZ);

	void FadeOutSelected(uint32_t aeSoundTypes, uint32_t auiFadeDuration, uint32_t unk);
	void FadeInSelected(uint32_t aeSoundTypes, uint32_t auiFadeDuration);

	void PauseAnimationSounds();
	void UnpauseAnimationSounds();

	void PauseWorldVoices();
	void UnpauseWorldVoices();
};

ASSERT_SIZE(BSAudio, 0x38);