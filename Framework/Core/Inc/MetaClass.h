#ifndef INCLUDE_NFGE_CORE_METACLASS_H
#define INCLUDE_NFGE_CORE_METACLASS_H

#include "MetaType.h"

namespace NFGE::Core::Meta
{
	class MetaField;
	
	class MetaClass : public MetaType
	{
	public:
		MetaClass(
			const char* name,
			size_t size,
			MetaType::CreateFunc create,
			MetaType::DestroyFunc destroy,
			const MetaClass* parent,
			std::vector<MetaField> fields);

		void Seriialize(const void* instance, rapidjson::Value& jsonValue) const override;
		void Deserialize(void* instance, const rapidjson::Value& jsonValue) const override;

		const MetaClass* GetParent() const;
		const MetaField* FindField(const char* name) const;
		const MetaField* GetField(size_t index) const;
		size_t GetFieldCount() const;

	private:
		size_t GetParentFieldCount() const;

		const MetaClass* mParent;
		const std::vector<MetaField> mFields;
	};
}

#endif // !INCLUDE_NFGE_CORE_METACLASS_H
