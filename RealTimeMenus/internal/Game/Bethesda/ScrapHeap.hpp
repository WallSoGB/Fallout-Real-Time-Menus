#pragma once

#include "BSSpinLock.hpp"

class ScrapHeapManager {
public:
	struct Buffer {
		char*		pData;
		uint32_t	uiSize;
	};

	Buffer				kBuffers[64];
	uint32_t			uiBufferCount;
	uint32_t			uiBufferAllocBytes;
	uint32_t			uiSysAllocBytes;
	DWORD				dword208[5];
	BSSpinLock			kLock;
};

ASSERT_SIZE(ScrapHeapManager, 0x240);

class ScrapHeap {
public:
	ScrapHeap();
	ScrapHeap(uint32_t aReserveSize);
	~ScrapHeap();

	struct Block {
		enum Flags {
			SIZE		= 0x7FFFFFFF,
			DEALLOCATED = 0x80000000,
		};

		uint32_t	uiSizeAndFlags;
		Block*		pPrevious;

		bool		IsFree() const { return (uiSizeAndFlags & DEALLOCATED) != 0; }
		void		SetFree(bool abFree) { if (abFree) uiSizeAndFlags |= DEALLOCATED; else uiSizeAndFlags &= ~DEALLOCATED; }
		uint32_t	GetSize() const { return uiSizeAndFlags & SIZE; }
	};

	struct FreeBlock : Block {
		FreeBlock* pLeft;
		FreeBlock* pRight;
	};


	char*	pBaseAddress;
	char*	pCommitStart;
	char*	pCommitEnd;
	Block*	pLastBlock;
};

ASSERT_SIZE(ScrapHeap, 0x10);
ASSERT_SIZE(ScrapHeap::Block, 0x8);