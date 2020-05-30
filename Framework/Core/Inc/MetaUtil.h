#ifndef INCLUDE_NFGE_CORE_METAUTIL_H
#define INCLUDE_NFGE_CORE_METAUTIL_H

#include "MetaArray.h"
#include "MetaPointer.h"

namespace NFGE::Core::Meta
{
	class MetaType;

	template<class DataType>
	const MetaType* DeduceType();

	template<class Type>
	void Deserialize(void* insatnce, const rapidjson::Value& value) {};

	namespace Detail
	{
		template<class DataType>
		inline const MetaType* DeduceTypeImpl(DataType*)
		{
			static_assert(sizeof(DataType) == -1, "Cannot deduce type.");
		}

		template <class DataType>
		inline const MetaType* DeduceTypeImpl(DataType**)
		{
			static MetaPointer sMetaPointer(DeduceType<DataType>());
			return &sMetaPointer;
		}

		template <class DataType>
		inline const MetaType* DeduceTypeImpl(std::vector<DataType>*)
		{
			static const auto getElement = [](void* instance, size_t index) -> void*
			{
				auto vector = reinterpret_cast<std::vector<DataType>*>(instance);
				vector->resize(index + 1);
				return vector->data() + index;
			};

			static const auto getCount = [](void* instance) -> size_t
			{
				auto vector = reinterpret_cast<std::vector<DataType>*>(instance);
				return vector->size();
			};
			static MetaArray sMetaArray(DeduceType<DataType>(), getElement, getCount);
			return &sMetaArray;
		}
	}

	template <class ClassType, class DataType>
	inline const MetaType* DeduceMemberType(DataType ClassType::*)
	{
		return DeduceType<DataType>();
	}

	template <class ClassType, class DataType>
	inline size_t GetFieldOffset(DataType ClassType::* field)
	{
		return (size_t)(void*)&(((ClassType*)nullptr)->*field);
	}

	template <class BaseType, class DerivedType>
	inline DerivedType* DynamicCast(BaseType* ptr)
	{
		if (ptr->GetMetaClass() != DerivedType::StaticGetMetaClass())
			return nullptr;
		return static_cast<DerivedType*>(ptr);
	}

	template <class BaseType, class DerivedType>
	inline bool IsBaseOf()
	{
		auto parent = DerivedType::StaticGetMetaClass()->GetParent();
		while (parent)
		{
			if (parent == BaseType::StaticGetMetaClass())
				return true;
			parent = parent->GetParent();
		}
		return false;
	}

	template <class DataType>
	inline const MetaType* DeduceType()
	{
		return Detail::DeduceTypeImpl(static_cast<DataType*>(nullptr));
	}
}

#endif // !INCLUDE_NFGE_CORE_METAUTIL_H
