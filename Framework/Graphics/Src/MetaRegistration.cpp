#include "Precompiled.h"
#include "MetaRegistration.h"

namespace NFGE::Core::Meta
{
	using namespace NFGE::Graphics;
	template<>
	void Deserialize<MeshBuffer::Topology>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto topology = (MeshBuffer::Topology*)(instance);
		*topology = static_cast<MeshBuffer::Topology>(object.FindMember("value")->value.GetInt());
	}

}

META_TYPE_DEFINE(NFGE::Graphics::MeshBuffer::Topology, Topology)

void NFGE::Graphics::StaticMetaRegister()
{

}