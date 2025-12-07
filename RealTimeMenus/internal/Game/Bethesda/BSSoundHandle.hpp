#pragma once

#include "BSMemObject.hpp"
#include "Gamebryo/NiPoint3.hpp"

class BSSoundHandle {
public:
	enum State {
		NONE	= 0,
		PLAYING = 1,
		STOPPED = 2,
		PAUSED	= 3,
	};

	uint32_t	uiSoundID;
	bool		bAssumeSuccess;
	uint32_t	uiState;

	BSSoundHandle() : uiSoundID(UINT32_MAX), bAssumeSuccess(false), uiState(0) {}
	BSSoundHandle(const BSSoundHandle& arHandle) : uiSoundID(arHandle.uiSoundID), bAssumeSuccess(arHandle.bAssumeSuccess), uiState(arHandle.uiState) {}
	explicit BSSoundHandle(const BSSoundHandle* apHandle) : uiSoundID(apHandle->uiSoundID), bAssumeSuccess(apHandle->bAssumeSuccess), uiState(apHandle->uiState) {}
	~BSSoundHandle() {}

	BSSoundHandle operator=(const BSSoundHandle& arHandle) {
		uiSoundID = arHandle.uiSoundID;
		bAssumeSuccess = arHandle.bAssumeSuccess;
		uiState = arHandle.uiState;
		return *this;
	}

	BSSoundHandle operator=(const BSSoundHandle* apHandle) {
		uiSoundID = apHandle->uiSoundID;
		bAssumeSuccess = apHandle->bAssumeSuccess;
		uiState = apHandle->uiState;
		return *this;
	}
};

ASSERT_SIZE(BSSoundHandle, 0xC);