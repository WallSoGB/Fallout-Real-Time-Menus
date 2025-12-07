#pragma once

template <class T>
class BSTMessageQueue {
public:
	BSTMessageQueue();
	virtual		 ~BSTMessageQueue();
	virtual bool Push(const T& arMessage);
	virtual bool TryPush(const T& arMessage);
	virtual bool Pop(T& arMessage);
	virtual bool TryPop(T& arMessage);
	virtual bool DoTryPush(const T& arMessage);
	virtual bool DoTryPop(T& arMessage);
};

ASSERT_SIZE(BSTMessageQueue<uint32_t>, 0x4);