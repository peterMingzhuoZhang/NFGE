#include "Precompiled.h"
#include "MetaRegistration.h"

using namespace NFGE;

namespace NFGE::Core::Meta 
{
	template <>
	void Deserialize<int>(void* instance, const rapidjson::Value& value)
	{
		*(int*)instance = value.GetInt();
	}
	template <>
	void Deserialize<float>(void* instance, const rapidjson::Value& value)
	{
		*(float*)instance = value.GetFloat();
	}
	template <>
	void Deserialize<bool>(void* instance, const rapidjson::Value& value)
	{
		*(bool*)instance = value.GetBool();
	}
	template <>
	void Deserialize<std::string>(void* instance, const rapidjson::Value& value)
	{
		*(std::string*)instance = value.GetString();
	}

}

// Primitive Type Definitions
META_TYPE_DEFINE(int, Integer)
META_TYPE_DEFINE(float, Number)
META_TYPE_DEFINE(bool, Boolean)
META_TYPE_DEFINE(std::string, String)

void NFGE::Core::StaticMetaRegister()
{

}