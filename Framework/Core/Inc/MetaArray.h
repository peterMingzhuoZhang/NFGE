#ifndef INCLUDE_NFGE_CORE_METAARRAY_H
#define INCLUDE_NFGE_CORE_METAARRAY_H

#include "MetaType.h"

namespace NFGE::Core::Meta
{
	class MetaArray : public MetaType
	{
	public:
		using GetElementFunc = std::function<void*(void*, size_t)>;
		using GetCountFunc = std::function<size_t(void*)>;

		MetaArray(const MetaType* elementType, GetElementFunc getElement, GetCountFunc getCount);

		const MetaType* GetElementType() const { return mElemenType; }
		size_t GetCount(void* instance) const { return mGetCount(instance); }
		void* GetElement(void* instance, size_t index) const { return mGetElement(instance, index); }

	private:
		const MetaType* mElemenType;
		GetElementFunc mGetElement;
		GetCountFunc mGetCount;
	};
}

#endif // !INCLUDE_NFGE_CORE_METACLASS_H
