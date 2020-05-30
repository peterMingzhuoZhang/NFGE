#ifndef INCLUDE_NFGE_CORE_METAPOINTER_H
#define INCLUDE_NFGE_CORE_METAPOINTER_H

#include "MetaType.h"

namespace NFGE::Core::Meta
{
	class MetaPointer : public MetaType
	{
	public:
		MetaPointer(const MetaType* pointer);

		const MetaType* GetPointerType() const { return mPointerType; }

	private:
		const MetaType* mPointerType;
	};
}

#endif // !INCLUDE_NFGE_CORE_METACLASS_H
