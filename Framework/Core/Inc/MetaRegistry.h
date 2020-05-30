#ifndef INCLUDE_NFGE_CORE_METAREGISTRY_H
#define INCLUDE_NFGE_CORE_METAREGISTRY_H

namespace NFGE::Core::Meta
{
	class MetaClass;

	void Register(const MetaClass* metaClass);

	const MetaClass* FindMetaClass(const std::string& className);
}


#endif // !INCLUDE_NFGE_CORE_METAREGISTRY_H
