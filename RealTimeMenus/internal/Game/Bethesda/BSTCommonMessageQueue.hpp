#pragma once

#include "BSTMessageQueue.hpp"
#include "BSNonReentrantSpinLock.hpp"

template <class T> 
class BSTCommonMessageQueue : public BSTMessageQueue<T> {
public:
	BSTCommonMessageQueue();
	~BSTCommonMessageQueue() override;

	BSNonReentrantSpinLock kLock;
};

ASSERT_SIZE(BSTCommonMessageQueue<uint32_t>, 0x8);