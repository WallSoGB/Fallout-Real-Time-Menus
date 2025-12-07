#pragma once

class BSNonReentrantSpinLock {
public:
	BSNonReentrantSpinLock();
	~BSNonReentrantSpinLock();

	uint32_t uiLock;
};

ASSERT_SIZE(BSNonReentrantSpinLock, 0x4);