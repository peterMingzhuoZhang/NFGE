#include "Precompiled.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "Debug.h"
using namespace NFGE::Core::Meta;



NFGE::Core::Meta::MetaClass::MetaClass(const char * name, size_t size, MetaType::CreateFunc create, MetaType::DestroyFunc destroy, const MetaClass * parent, std::vector<MetaField> fields)
	: MetaType(MetaType::Category::Class, name, size, std::move(create), std::move(destroy))
	, mParent(parent)
	, mFields(std::move(fields))
{
}

void MetaClass::Seriialize(const void* instance, rapidjson::Value& jsonValue) const
{
	ASSERT(false, "TODO");
}
void MetaClass::Deserialize(void* instance, const rapidjson::Value& jsonValue) const
{
	for (auto& member : jsonValue.GetObjectW())
	{
		auto metaField = FindField(member.name.GetString());
		auto metaType = metaField->GetMetaType();
		void* instanceField = static_cast<uint8_t*>(instance) + metaField->GetOffset();
		metaType->Deserialize(instanceField, member.value);
	}
}

const MetaClass * NFGE::Core::Meta::MetaClass::GetParent() const
{
	return mParent;
}

const MetaField * NFGE::Core::Meta::MetaClass::FindField(const char * name) const
{
	for (auto& field : mFields)
	{
		if (strcmp(field.GetName(), name) == 0)
			return &field;
	}

	// If the field is not found, try the parent
	if (mParent != nullptr)
		return mParent->FindField(name);

	return nullptr;
}

const MetaField * NFGE::Core::Meta::MetaClass::GetField(size_t index) const
{
	ASSERT(index < GetFieldCount(), "[MetaClass] Subscript out of range!");
	const size_t parentCount = GetParentFieldCount();
	if (index < parentCount)
		return mParent->GetField(index);
	return mFields.data() + (index - parentCount);
}

size_t NFGE::Core::Meta::MetaClass::GetFieldCount() const
{
	return mFields.size() + GetParentFieldCount();
}

size_t NFGE::Core::Meta::MetaClass::GetParentFieldCount() const
{
	return mParent ? mParent->GetFieldCount() : 0u;
}



