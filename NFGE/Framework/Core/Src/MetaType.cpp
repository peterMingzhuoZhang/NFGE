#include "Precompiled.h"
#include "MetaType.h"
#include "MetaArray.h"
#include "MetaPointer.h"
#include "MetaClass.h"

using namespace NFGE::Core::Meta;

MetaType::MetaType(Category category, const char* name, size_t size, CreateFunc create, DestroyFunc destroy, SerializeFunc serialize, DeserializeFunc deserialize)
	:mCategory(category)
	,mName(name)
	,mSize(size)
	,mCreate(std::move(create))
	,mDestroy(std::move(destroy))
	,mSerialize(std::move(serialize))
	,mDeserialize(std::move(deserialize))
{

}

void* MetaType::Create() const
{
	ASSERT(mCreate, "[MetaType] no creation callable registered for '%s'.", GetName());
	return mCreate();
}

void MetaType::Destroy(void* data)
{
	ASSERT(mDestroy, "[MetaType] no destruction callable registered for '%s'.", GetName());
	mDestroy(data);
}
void NFGE::Core::Meta::MetaType::Seriialize(const void * instance, rapidjson::Value & jsonValue) const
{
	ASSERT(mSerialize, "[MetaType] no serialize callable registered for '%s'.", GetName());
	mSerialize(instance, jsonValue);
}
void NFGE::Core::Meta::MetaType::Deserialize(void * instance, const rapidjson::Value & jsonValue) const
{
	ASSERT(mDeserialize, "[MetaType] no deserialize callable registered for '%s'.", GetName());
	mDeserialize(instance, jsonValue);
}
//const MetaClass * NFGE::Core::Meta::MetaType::GetMetaClass() const
//{
//	ASSERT(mCategory == Category::Class, "[MetaType] Invalid type cast.");
//	return static_cast<const MetaClass*>(this);
//}
//
//const MetaArray * NFGE::Core::Meta::MetaType::GetMetaArray() const
//{
//	ASSERT(mCategory == Category::Array, "[MetaType] Invalid type cast.");
//	return static_cast<const MetaArray*>(this);
//}
//
//const MetaPointer * NFGE::Core::Meta::MetaType::GetMetaPointer() const
//{
//	ASSERT(mCategory == Category::Pointer, "[MetaType] Invalid type cast.");
//	return static_cast<const MetaPointer*>(this);
//}
