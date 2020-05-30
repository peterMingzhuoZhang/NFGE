#ifndef INCLUDED_NFGE_METATYPE_H
#define INCLUDED_NFGE_METATYPE_H

#include "Debug.h"

namespace NFGE::Core::Meta
{
	class MetaClass;
	class MetaArray;
	class MetaPointer;

	class MetaType
	{
	public:
		using CreateFunc = std::function<void*()>;
		using DestroyFunc = std::function<void(void*)>;
		using SerializeFunc = std::function<void(const void* instance, rapidjson::Value& jsonValue)>;
		using DeserializeFunc = std::function<void(void* instance, const rapidjson::Value& jsonValue)>;

		enum class Category
		{
			Primitive,
			Class,
			Array,
			Pointer
		};

		MetaType(Category category, const char* name, size_t size, CreateFunc create, DestroyFunc destroy, SerializeFunc serialize = nullptr, DeserializeFunc deserialize = nullptr);

		const char* GetName() const		{ return mName.c_str(); }
		Category GetCategory() const	{ return mCategory; }
		size_t GetSize() const			{ return mSize; }

		void* Create() const;
		void Destroy(void* data);
		virtual void Seriialize(const void* instance, rapidjson::Value& jsonValue) const;
		virtual void Deserialize(void* instance, const rapidjson::Value& jsonValue) const;

		//const MetaClass* GetMetaClass() const;
		//const MetaArray* GetMetaArray() const;
		//const MetaPointer* GetMetaPointer() const;

	private:
		const Category mCategory;
		const std::string mName;
		const size_t mSize;
		const CreateFunc mCreate;
		const DestroyFunc mDestroy;
		const SerializeFunc mSerialize;
		const DeserializeFunc mDeserialize;

	};
}
#endif // !INCLUDED_NFGE_METATYPE_H

