#include "Precompiled.h"
#include "MetaRegistry.h"

#include "Debug.h"
#include "MetaClass.h"

using namespace NFGE::Core;

namespace
{
	using MetaClassMap = std::map<std::string, const Meta::MetaClass*>;

	inline MetaClassMap& GetRegistry()
	{
		static MetaClassMap sMetaClassMap;
		return sMetaClassMap;
	}
}

void Meta::Register(const Meta::MetaClass* metaClass)
{
	GetRegistry().emplace(metaClass->GetName(), metaClass);
}

const Meta::MetaClass* Meta::FindMetaClass(const std::string& className)
{
	MetaClassMap::const_iterator iter = GetRegistry().find(className);
	ASSERT(iter != GetRegistry().end(), "[MetaRegistry] Meta class for %s not found.", className.c_str());
	return iter->second;
}