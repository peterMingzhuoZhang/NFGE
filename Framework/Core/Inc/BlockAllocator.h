#ifndef INCLUDED_NFGE_BLOCKALLOCATOR_H
#define INCLUDED_NFGE_BLOCKALLOCATOR_H

namespace NFGE::Core
{
	class BlockAllocator
	{
	public:
		BlockAllocator(size_t blockSize, size_t capacity);
		~BlockAllocator();

		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;

		void* Allocate();
		void Free(void* ptr);

		size_t GetCapacity() const { return mCapacity; }
	protected:
		uint8_t* mData;
		size_t mBlockSize;
		size_t mCapacity;
		std::vector<size_t> mFreeSlots;
	};
}
#endif // !INCLUDED_NFGE_BLOCKALLOCATOR_H
