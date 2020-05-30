#ifndef INCLUDE_NFGE_CORE_METAREGISTRATION_H
#define INCLUDE_NFGE_CORE_METAREGISTRATION_H

#include "Meta.h"

// Primitive Type Declarations
META_TYPE_DECLARE(int)
META_TYPE_DECLARE(float)
META_TYPE_DECLARE(bool)
META_TYPE_DECLARE(std::string)
namespace NFGE::Core
{
	void StaticMetaRegister();
}

#define META_REGISTER(Class)\
	NFGE::Core::Meta::Register(Class::StaticGetMetaClass())

#endif // !INCLUDE_NFGE_CORE_METAREGISTRATION_H
