#include "Precompiled.h"
#include "BlockAllocator.h"
#include "Debug.h"

using namespace NFGE::Core;

NFGE::Core::BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	: mBlockSize(blockSize)
	, mCapacity(capacity)
{
	mData = (uint8_t*) malloc(blockSize * capacity);
	mFreeSlots.reserve(capacity);
	for (size_t i = 0; i < capacity; i++)
	{
		mFreeSlots.push_back(i);
	}
}

NFGE::Core::BlockAllocator::~BlockAllocator()
{
	free(mData);
}

void * NFGE::Core::BlockAllocator::Allocate()
{
	if (mFreeSlots.empty())
		return nullptr;
	
	size_t roomCard = mFreeSlots.back();
	mFreeSlots.pop_back();
	return mData + mBlockSize * roomCard;
}

void NFGE::Core::BlockAllocator::Free(void * ptr)
{
	ASSERT(ptr >= mData, "Trying Free a pointer does not belong to this blockAllocator.");
	ptrdiff_t addrDiff = (uint8_t*)ptr - mData;
	ASSERT(addrDiff % mBlockSize == 0, "Trying Free a pointer does not belong to this blockAllocator.");
	size_t returnRoomCard = (size_t)(addrDiff) / mBlockSize;
	ASSERT(returnRoomCard < mCapacity, "Trying Free a pointer does not belong to this blockAllocator.");
	mFreeSlots.push_back(returnRoomCard);
}
