#include "Precompiled.h"
#include "MetaRegistration.h"

namespace NFGE::Core::Meta 
{
	using namespace NFGE::Math;
	template<>
	void Deserialize<Vector2>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vector2 = (Vector2*)(instance);
		vector2->x = object.FindMember("x")->value.GetFloat();
		vector2->y = object.FindMember("y")->value.GetFloat();
	}
	template<>
	void Deserialize<Vector3>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vector2 = (Vector3*)(instance);
		vector2->x = object.FindMember("x")->value.GetFloat();
		vector2->y = object.FindMember("y")->value.GetFloat();
		vector2->z = object.FindMember("z")->value.GetFloat();
	}
	template<>
	void Deserialize<Vector4>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vector2 = (Vector4*)(instance);
		vector2->x = object.FindMember("x")->value.GetFloat();
		vector2->y = object.FindMember("y")->value.GetFloat();
		vector2->z = object.FindMember("z")->value.GetFloat();
		vector2->w = object.FindMember("w")->value.GetFloat();
	}
	template<>
	void Deserialize<Quaternion>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vector2 = (Quaternion*)(instance);
		vector2->x = object.FindMember("x")->value.GetFloat();
		vector2->y = object.FindMember("y")->value.GetFloat();
		vector2->z = object.FindMember("z")->value.GetFloat();
		vector2->w = object.FindMember("w")->value.GetFloat();
	}
}

// Primitive Type Definitions
META_TYPE_DEFINE(NFGE::Math::Vector2, Vector2)
META_TYPE_DEFINE(NFGE::Math::Vector3, Vector3)
META_TYPE_DEFINE(NFGE::Math::Vector4, Vector4)
META_TYPE_DEFINE(NFGE::Math::Quaternion, Quaternion)
META_TYPE_DEFINE(NFGE::Math::Matrix4, Matrix4)

void NFGE::Math::StaticMetaRegister()
{
	
}