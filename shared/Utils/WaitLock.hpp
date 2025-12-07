#pragma once

#include <atomic>

#define WAIT_LOCK_USE_WINAPI 1

#if WAIT_LOCK_USE_WINAPI
#include <synchapi.h>
#pragma comment(lib, "Synchronization.lib")
#endif

class WaitLock {
private:
	bool Release() {
		uint8_t ucCmp = LOCKED;
		return ucLock.compare_exchange_strong(ucCmp, OPEN);
	}

public:
	WaitLock() = default;
	~WaitLock() = default;

	enum LockState : uint8_t {
		OPEN	= 0,
		LOCKED	= 1
	};

	std::atomic<uint8_t> ucLock = OPEN;
	
	bool TryLock() {
		uint8_t ucCmp = OPEN;
		return ucLock.compare_exchange_strong(ucCmp, LOCKED);
	}

	void Lock(const char* apName = nullptr) {
		if (TryLock()) {
			return;
		}
		// Wait
		while (true) {
			if (TryLock()) {
#ifdef _DEBUG
				_MESSAGE("[ WaitLock ] Lock %s acquired by thread %u after waiting", apName ? apName : "UNK", GetCurrentThreadId());
#endif
				return;
			}
			else {
#ifdef _DEBUG
				_MESSAGE("[ WaitLock ] Lock %s busy, thread %u waiting", apName ? apName : "UNK", GetCurrentThreadId());
#endif
			}

			uint8_t ucCmp = LOCKED;
#if WAIT_LOCK_USE_WINAPI
			[[maybe_unused]]BOOL bResult = WaitOnAddress(&ucLock, &ucCmp, sizeof(uint8_t), INFINITE);
#ifdef _DEBUG
			if (!bResult) {
				char cErrorMessage[96];
				our_snprintf(cErrorMessage, sizeof(cErrorMessage), "[ WaitLock ] WaitOnAddress failed for thread %u, error code %u", GetCurrentThreadId(), GetLastError());
				MessageBox(nullptr, cErrorMessage, "WaitLock Error", MB_OK | MB_ICONERROR);
				DebugBreak();
			}
#endif
#else
			ucLock.wait(ucCmp);
#endif
		}
	}

	void Unlock() {
		if (Release()) {
#if WAIT_LOCK_USE_WINAPI
			WakeByAddressSingle(&ucLock);
#else
			ucLock.notify_one();
#endif
		}
		else {
#ifdef _DEBUG
			char cErrorMessage[96];
			our_snprintf(cErrorMessage, sizeof(cErrorMessage), "[ WaitLock ] Unlock called by thread %u, but lock is not owned", GetCurrentThreadId());
			MessageBox(nullptr, cErrorMessage, "WaitLock Error", MB_OK | MB_ICONERROR);
			DebugBreak();
#endif
		}
	}
};

class WaitLockRecurse : public WaitLock {
public:
	std::atomic<uint16_t>	usLockCount		= 0;
	std::atomic<uint32_t>	uiOwnerThreadID = 0;

	bool TryLock() {
		const uint32_t uiThreadID = GetCurrentThreadId();
		if (uiOwnerThreadID == uiThreadID) {
			usLockCount++;
			return true;
		}
		if (WaitLock::TryLock()) {
			uiOwnerThreadID = uiThreadID;
			usLockCount = 1;
			return true;
		}
		return false;
	}

	void Lock(const char* apName = nullptr) {
		const uint32_t uiThreadID = GetCurrentThreadId();
		if (uiOwnerThreadID == uiThreadID) {
			usLockCount++;
			return;
		}
		WaitLock::Lock(apName);
		uiOwnerThreadID = uiThreadID;
		usLockCount = 1;
	}

	void Unlock() {
		const uint32_t uiThreadID = GetCurrentThreadId();
		if (uiOwnerThreadID != uiThreadID) {
#ifdef _DEBUG
			char cErrorMessage[96];
			our_snprintf(cErrorMessage, sizeof(cErrorMessage), "[ WaitLockRecurse ] Unlock called by thread %u, but lock is owned by thread %u", uiThreadID, uiOwnerThreadID.load());
			MessageBox(nullptr, cErrorMessage, "WaitLockRecurse Error", MB_OK | MB_ICONERROR);
			DebugBreak();
#endif
			return;
		}
		if (--usLockCount == 0) {
			uiOwnerThreadID = 0;
			WaitLock::Unlock();
		}
	}
};

template<typename T>
class WaitLockScope {
public:
	WaitLockScope(const T* apLock) : pLock(apLock)	{ const_cast<T*>(pLock)->Lock(); };
	WaitLockScope(const T& arLock) : pLock(&arLock)	{ const_cast<T*>(pLock)->Lock(); };
	~WaitLockScope() { const_cast<T*>(pLock)->Unlock(); };

protected:
	const T* pLock = nullptr;
};

// Imitates a BSSpinLock
class ALIGN4 BSWaitLock : public WaitLockRecurse {
private:
	char filler[0x18];
};

ASSERT_SIZE(BSWaitLock, 0x20);

// Imitates a critical section
class ALIGN4 CSLock : public WaitLockRecurse {
private:
	char filler[0x10];
};

ASSERT_SIZE(CSLock, 0x18);