#ifndef INCLUDE_NFGE_CORE_META_H
#define INCLUDE_NFGE_CORE_META_H

#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaPointer.h"
#include "MetaRegistry.h"
#include "MetaType.h"
#include "MetaUtil.h"

#define META_TYPE_DECLARE(Type)\
	template <> const NFGE::Core::Meta::MetaType* NFGE::Core::Meta::DeduceType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template <> const NFGE::Core::Meta::MetaType* NFGE::Core::Meta::DeduceType<Type>()\
	{\
		static const NFGE::Core::Meta::MetaType sMetaType(\
			NFGE::Core::Meta::MetaType::Category::Primitive, #Name, sizeof(Type),\
			[] {return new Type;},\
			[](void* data) { delete static_cast<Type*>(data); },\
			nullptr,\
			NFGE::Core::Meta::Deserialize<Type>);\
		return &sMetaType;\
	}

#define META_CLASS_DECLARE\
	static const NFGE::Core::Meta::MetaClass* StaticGetMetaClass();\
	virtual const NFGE::Core::Meta::MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }

#define META_DERIVED_BEGIN(ClassType, ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const NFGE::Core::Meta::MetaClass* parentMetaClass = ParentType::StaticGetMetaClass();

#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const NFGE::Core::Meta::MetaClass* parentMetaClass = nullptr;

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template <> const NFGE::Core::Meta::MetaType* NFGE::Core::Meta::DeduceType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const NFGE::Core::Meta::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* className = #ClassType;

#define META_FIELD_BEGIN\
		static const std::initializer_list<NFGE::Core::Meta::MetaField> fields {

#define META_FIELD(Var, Name)\
			{ NFGE::Core::Meta::DeduceMemberType(&Class::Var), Name, NFGE::Core::Meta::GetFieldOffset(&Class::Var) },

#define META_FIELD_END\
		};

#define META_NO_FIELD\
		static const std::initializer_list<NFGE::Core::Meta::MetaField> fields;

#define META_CLASS_END\
		static const NFGE::Core::Meta::MetaClass sMetaClass(\
			className, sizeof(Class),\
			[] { return new Class(); },\
			[](void* data) { delete static_cast<Class*>(data); },\
			parentMetaClass, fields);\
		return &sMetaClass;\
	}



#endif // !INCLUDE_NFGE_CORE_META_H
