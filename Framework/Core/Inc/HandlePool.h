#ifndef NFGE_CORE_HANDLEPOOL_H
#define NFGE_CORE_HANDLEPOOL_H

namespace NFGE::Core
{
	template <class DataType>
	class Handle;

	template <class DataType>
	class HandlePool
	{
	public:
		using HandleType = Handle<DataType>;

		HandlePool(uint32_t capacity);
		~HandlePool();

		HandleType Register(DataType* instance);
		void Unregister(HandleType handle);
		void Flush();

		bool IsValid(HandleType handle) const;
		DataType* Get(HandleType handle);

	private:
		struct Entry
		{
			DataType* instance = nullptr;
			uint32_t generation = 0;
		};

		std::vector<Entry> mEntries;
		std::vector<uint32_t> mFreeSlots;
	};


	template<class DataType>
	inline HandlePool<DataType>::HandlePool(uint32_t capacity)
	{
		mEntries.resize(capacity + 1, {});
		mFreeSlots.reserve(capacity);
		for (size_t i = capacity; i > 0; --i)
		{
			mFreeSlots.push_back(i);
		}

		ASSERT(HandleType::sPool == nullptr, "[HandlePool] Cannot have more than one pool for the same type!");
		HandleType::sPool = this;
	}

	template<class DataType>
	inline HandlePool<DataType>::~HandlePool()
	{
		ASSERT(mFreeSlots.size() == mFreeSlots.capacity(), "[HandlePool] Pool is not clearn when destruct.");

		ASSERT(HandleType::sPool == this, "[HandlePool] Something wrong.");
		HandleType::sPool = nullptr;
	}

	template<class DataType>
	inline Handle<DataType> HandlePool<DataType>::Register(DataType * instance)
	{
		ASSERT(!mFreeSlots.empty(), "[HandlePool] is empty.");
		
		uint32_t roomCard = mFreeSlots.back();
		mFreeSlots.pop_back();

		auto& entry = mEntries[roomCard];
		entry.instance = instance;
		
		Handle<DataType> ret;
		ret.mIndex = roomCard;
		ret.mGeneration = entry.generation;
		return ret;
	}

	template<class DataType>
	inline void HandlePool<DataType>::Unregister(HandleType handle)
	{
		if (IsValid(handle))
		{
			auto& entry = mEntries[handle.mIndex];
			entry.generation++;
			entry.instance = nullptr;
			mFreeSlots.push_back(handle.mIndex);
		}
	}

	template<class DataType>

	void HandlePool<DataType>::Flush()
	{
		// Force increment generation and invalidate all existing handles
		for (auto& entry : mEntries)
		{
			entry.instance = nullptr;
			entry.generation++;
		}

		// Reclaim all slots
		mFreeSlots.clear();
		for (size_t i = mFreeSlots.capacity(); i > 0; --i)
			mFreeSlots.push_back(i);
	}

	template<class DataType>
	inline bool HandlePool<DataType>::IsValid(HandleType handle) const
	{
		uint32_t index = handle.mIndex;
		if (index != 0)
		{
			if (mEntries[index].generation == handle.mGeneration)
			{
				return true;
			}
		}
		return false;
	}

	template<class DataType>
	inline DataType * HandlePool<DataType>::Get(HandleType handle)
	{
		return IsValid(handle) ? mEntries[handle.mIndex].instance : nullptr;
	}

}

#endif // !NFGE_CORE_HANDLEPOOL_H

