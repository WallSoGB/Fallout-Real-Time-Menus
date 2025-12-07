#pragma once

#include "BSSimpleArray.hpp"

// GAME - 0x438660
template<typename T, uint32_t RESIZE_SIZE>
inline BSSimpleArray<T, RESIZE_SIZE>::BSSimpleArray() {
	InitialSetup(0, 0);
}

// GAME - 0x709E80
template<typename T, uint32_t RESIZE_SIZE>
inline BSSimpleArray<T, RESIZE_SIZE>::BSSimpleArray(uint32_t auiAllocSize, uint32_t auiSize) {
	InitialSetup(auiAllocSize, auiSize);
}

// GAME - 0x438690
template<typename T, uint32_t RESIZE_SIZE>
inline BSSimpleArray<T, RESIZE_SIZE>::~BSSimpleArray() {
	Clear();
}

// GAME - 0x8225D0
template<typename T, uint32_t RESIZE_SIZE>
SPEC_RESTRICT inline T* BSSimpleArray<T, RESIZE_SIZE>::Allocate(uint32_t auiCount) {
	return BSMemory::malloc<T>(auiCount); 
};

// GAME - 0x6DB090
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::Deallocate(T* apData) { 
	BSMemory::free(apData); 
};

// GAME - 0x84B630
template<typename T, uint32_t RESIZE_SIZE>
SPEC_RESTRICT inline T* BSSimpleArray<T, RESIZE_SIZE>::Reallocate(T* apData, uint32_t auiCount) {
	return static_cast<T*>(BSMemory::realloc(apData, sizeof(T) * auiCount)); 
};

template<typename T, uint32_t RESIZE_SIZE>
inline uint32_t BSSimpleArray<T, RESIZE_SIZE>::GetSize() const {
	return uiSize;
}

template<typename T, uint32_t RESIZE_SIZE>
inline uint32_t BSSimpleArray<T, RESIZE_SIZE>::GetAllocSize() const {
	return uiAllocSize;
}

// GAME - 0x76B610
template<typename T, uint32_t RESIZE_SIZE>
inline bool BSSimpleArray<T, RESIZE_SIZE>::IsEmpty() const {
	return uiSize == 0;
}

// GAME - 0x438B90
template<typename T, uint32_t RESIZE_SIZE>
inline bool BSSimpleArray<T, RESIZE_SIZE>::IsFull() const {
	return uiSize == uiAllocSize;
}

// GAME - 0x877A30
template<typename T, uint32_t RESIZE_SIZE>
inline T& BSSimpleArray<T, RESIZE_SIZE>::GetAt(const uint32_t auiIndex) const {
	assert(auiIndex >= 0);
	assert(auiIndex < uiSize);
	return pBuffer[auiIndex]; 
}

template<typename T, uint32_t RESIZE_SIZE>
inline T& BSSimpleArray<T, RESIZE_SIZE>::GetFirst() const {
	return GetAt(0);
}

// GAME - 0x6B3140
template<typename T, uint32_t RESIZE_SIZE>
inline T& BSSimpleArray<T, RESIZE_SIZE>::GetLast() const {
	return GetAt(uiSize - 1);
}

// GAME - 0x6A7AD0
template<typename T, uint32_t RESIZE_SIZE>
inline T& BSSimpleArray<T, RESIZE_SIZE>::operator[](uint32_t auiIndex) const {
	return GetAt(auiIndex);
}

// GAME - 0x822780
template<typename T, uint32_t RESIZE_SIZE>
void BSSimpleArray<T, RESIZE_SIZE>::ConstructItems(T* apData, uint32_t auiCount) {
	for (uint32_t i = 0; i < auiCount; i++)
		new (&apData[i]) T;
}

// GAME - 0x822820
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::DestructItems(T* apData, uint32_t auiCount) {
	for (uint32_t i = 0; i < auiCount; ++i)
		apData[i].~T();
}

// GAME - 0x6B3EB0
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::InitialSetup(uint32_t auiAllocSize, uint32_t auiSize) {
	pBuffer = nullptr;
	uiSize = 0;
	uiAllocSize = 0;

	if (auiSize > auiAllocSize)
		auiAllocSize = auiSize;

	if (auiAllocSize) {
		pBuffer = Allocate(auiAllocSize);
		uiAllocSize = auiAllocSize;
	}

	if (auiSize) {
		ConstructItems(pBuffer, auiSize);
		uiSize = auiSize;
	}
}

// GAME - 0x6A8500
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::FreeArray() {
	Deallocate(pBuffer);
	pBuffer = nullptr;
}

// GAME - 0x6C6200
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::Clear(bool abFree) {
	if (!pBuffer)
		return;

	DestructItems(pBuffer, uiSize);

	if (abFree) {
		FreeArray();
		pBuffer = nullptr;
		uiAllocSize = 0;
	}

	uiSize = 0;
}

// GAME - 0x42FB60
template<typename T, uint32_t RESIZE_SIZE>
SPEC_NOALIAS inline void BSSimpleArray<T, RESIZE_SIZE>::CopyItems(T* apNewBuffer, T* apOldBuffer, uint32_t auiSize) {
	if (!auiSize)
		return;

	if (apNewBuffer < apOldBuffer) {
		for (uint32_t i = 0; i < auiSize; ++i)
			memcpy(&apNewBuffer[i], &apOldBuffer[i], sizeof(T));
	}
	else if (apNewBuffer > apOldBuffer) {
		for (int32_t i = static_cast<int32_t>(auiSize) - 1; i >= 0; --i)
			memcpy(&apNewBuffer[i], &apOldBuffer[i], sizeof(T));
	}
}

// GAME - 0x42FC00
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::Resize(uint32_t auiNewSize, uint32_t auiSize) {
	if (!pBuffer) {
		pBuffer = Allocate(auiNewSize);
		uiAllocSize = auiNewSize;
	}
	else if (auiSize == uiAllocSize) {
		pBuffer = Reallocate(pBuffer, auiNewSize);
	}
	else {
		T* pNewBuffer = Allocate(auiNewSize);
		CopyItems(pNewBuffer, pBuffer, auiSize);
		FreeArray();
		pBuffer = pNewBuffer;
	}
}

// GAME - 0x695230
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::SetSize(uint32_t auiSize, bool abFree) {
	if (!auiSize) {
		Clear(abFree);
	}
	else if (auiSize > uiAllocSize) {
		if (uiAllocSize)
			Resize(auiSize, uiSize);
		else
			pBuffer = Allocate(auiSize);

		uiAllocSize = auiSize;
		ConstructItems(&pBuffer[uiSize], auiSize - uiSize);
		uiSize = auiSize;
	}
	else if (auiSize >= uiSize) {
		ConstructItems(&pBuffer[uiSize], auiSize - uiSize);
		uiSize = auiSize;
	}
	else {
		DestructItems(&pBuffer[auiSize], uiSize - auiSize);
		uiSize = auiSize;
		if (abFree && auiSize <= uiAllocSize >> 2) {
			Resize(auiSize, auiSize);
			uiAllocSize = auiSize;
		}
	}
}

// GAME - 0x6F9790
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::SetAllocSize(uint32_t auiAllocSize) {
	if (auiAllocSize == uiAllocSize)
		return;

	if (auiAllocSize < uiSize)
		uiSize = auiAllocSize;

	Resize(auiAllocSize, uiSize);
	uiAllocSize = auiAllocSize;
}

// GAME - 0x9A3910
template<typename T, uint32_t RESIZE_SIZE>
inline uint32_t BSSimpleArray<T, RESIZE_SIZE>::GetResizeSize() const {
	if (uiAllocSize <= RESIZE_SIZE)
		return 2 * uiAllocSize;
	else
		return uiAllocSize + RESIZE_SIZE;
}

// GAME - 0x761540, 0xB5C360
template<typename T, uint32_t RESIZE_SIZE>
inline uint32_t BSSimpleArray<T, RESIZE_SIZE>::GetNextIndex() {
	if (!IsFull())
		return uiSize++;

	if (uiAllocSize) {
		uint32_t uiResizeSize = GetResizeSize();
		Resize(uiResizeSize, uiSize);
		uiAllocSize = uiResizeSize;
	}
	else {
		constexpr uint32_t DEFAULT_ALLOC_SIZE = 4;
		pBuffer = Allocate(DEFAULT_ALLOC_SIZE);
		uiAllocSize = DEFAULT_ALLOC_SIZE;
	}

	return uiSize++;
}

// GAME - 0x7CB2E0
template<typename T, uint32_t RESIZE_SIZE>
inline uint32_t BSSimpleArray<T, RESIZE_SIZE>::Add(const T& aItem) {
	uint32_t uiNext = GetNextIndex();
	ConstructItems(&pBuffer[uiNext], 1);
	pBuffer[uiNext] = aItem;
	return uiNext;
}

// GAME - 0x6DB540
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::SetAt(uint32_t auiIndex, const T& aItem) {
	if (auiIndex == uiSize) {
		Add(aItem);
	}
	else if (IsFull()) {
		uint32_t uiResizeSize = GetResizeSize();
		T* pNewBuffer = Allocate(uiResizeSize);
		CopyItems(pNewBuffer, pBuffer, auiIndex);
		ConstructItems(&pBuffer[auiIndex], 1);
		CopyItems(&pNewBuffer[auiIndex + 1], &pBuffer[auiIndex], uiSize - auiIndex);
		FreeArray();
		pBuffer = pNewBuffer;
		uiAllocSize = uiResizeSize;
	}
	else {
		CopyItems(&pBuffer[auiIndex + 1], &pBuffer[auiIndex], uiSize - auiIndex);
		ConstructItems(&pBuffer[auiIndex], 1);
	}
	++uiSize;
	pBuffer[auiIndex] = aItem;
}

// GAME - 0x6F3170
template<typename T, uint32_t RESIZE_SIZE>
inline bool BSSimpleArray<T, RESIZE_SIZE>::HasSufficientCapacity() const {
	if (uiAllocSize <= RESIZE_SIZE) {
		if (uiSize <= uiAllocSize >> 2)
			return true;
	}
	else if (uiSize <= (uiAllocSize - RESIZE_SIZE * 2))
		return true;

	return false;
}

// GAME - 0x869600
template<typename T, uint32_t RESIZE_SIZE>
inline uint32_t BSSimpleArray<T, RESIZE_SIZE>::GetOptimalCapacity() const {
	if (uiAllocSize <= RESIZE_SIZE * 2)
		return uiAllocSize >> 1;
	else
		return uiAllocSize - RESIZE_SIZE;
}

// GAME - 0x6BF8F0
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::RemoveAt(uint32_t auiIndex, bool abResize) {
	if (abResize && HasSufficientCapacity()) {
		uint32_t uiOptimalCapacity = GetOptimalCapacity();
		T* pNewBuffer = Allocate(uiOptimalCapacity);
		CopyItems(pNewBuffer, pBuffer, auiIndex);
		DestructItems(&pBuffer[auiIndex], 1);
		CopyItems(&pNewBuffer[auiIndex], &pBuffer[auiIndex + 1], uiSize - 1);
		FreeArray();
		pBuffer = pNewBuffer;
		uiAllocSize = uiOptimalCapacity;
	}
	else
	{
		DestructItems(&pBuffer[auiIndex], 1);
		CopyItems(&pBuffer[auiIndex], &pBuffer[auiIndex + 1], uiSize - auiIndex - 1);
	}
	--uiSize;
}

// GAME - 0x98F1F0
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::RemoveFast(uint32_t auiIndex, uint32_t auiCount) {
	uint32_t uiToMove = std::min(auiCount, uiSize - auiIndex - auiCount);
	DestructItems(&pBuffer[auiIndex], auiCount);
	CopyItems(&pBuffer[auiIndex], &pBuffer[uiSize - uiToMove], uiToMove);
	uiSize -= auiCount;
}

// GAME - 0x719B20
template<typename T, uint32_t RESIZE_SIZE>
inline int32_t BSSimpleArray<T, RESIZE_SIZE>::Find(const T& aItem, uint32_t auiStartPosition, bool(__cdecl* apCompareFunc)(const T&, const T&)) const {
	while (auiStartPosition < uiSize) {
		if (pBuffer && apCompareFunc(pBuffer[auiStartPosition], aItem))
			return auiStartPosition;
		++auiStartPosition;
	}
	return -1;
}

// GAME - 0x9962F0
template<typename T, uint32_t RESIZE_SIZE>
inline bool BSSimpleArray<T, RESIZE_SIZE>::IsInArray(const T& aItem) const {
	return Find(aItem, 0, [](const T& left, const T& right) -> bool { return left == right; }) != -1;
}

// GAME - 0x6C8980
template<typename T, uint32_t RESIZE_SIZE>
inline void BSSimpleArray<T, RESIZE_SIZE>::Sort(_CoreCrtNonSecureSearchSortCompareFunction apCompareFunc) {
	std::qsort(pBuffer, uiSize, sizeof(T), apCompareFunc);
}