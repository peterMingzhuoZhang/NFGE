#ifndef INCLUDED_NFGE_TYPEALLOCATOR_H
#define INCLUDED_NFGE_TYPEALLOCATOR_H

#include "BlockAllocator.h"
namespace NFGE::Core
{
	template<typename DataType>
	class TypeAllocator : private BlockAllocator
	{
	public:
		TypeAllocator(size_t capacity);

		template<class... Args>
		DataType* New(Args&&... args);

		void Delete (DataType* ptr);
	};
	template<typename DataType>
	inline TypeAllocator<DataType>::TypeAllocator(size_t capacity)
		: BlockAllocator(sizeof(DataType), capacity)
	{

	}
	
	template<typename DataType>
	template<class ...Args>
	inline DataType * TypeAllocator<DataType>::New(Args && ...args)
	{
		//void* mem = Allocate();
		DataType* instance = static_cast<DataType*>(Allocate());
		if (instance)
		{
			new (instance) DataType(std::forward<Args>(args)...);
		}
		return instance;
	}

	template<typename DataType>
	inline void TypeAllocator<DataType>::Delete(DataType * ptr)
	{
		if (ptr == nullptr)
			return;

		ptr->~DataType();
		Free(ptr);
	}
}
#endif // !INCLUDED_NFGE_BLOCKALLOCATOR_H
