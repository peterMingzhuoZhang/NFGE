#include "Precompiled.h"
#include "MetaRegistration.h"

namespace NFGE::Core::Meta
{
	using namespace NFGE::Graphics;
	template<>
	void Deserialize<MeshBuffer::Topology>(void* instance, const rapidjson::Value& value)
	{
		auto topology = (MeshBuffer::Topology*)(instance);
		*topology = static_cast<MeshBuffer::Topology>(value.GetInt());
	}

	template<>
	void Deserialize<NFGE::Graphics::MeshTextureMaterial::ModelTextureType>(void* instance, const rapidjson::Value& value)
	{
		auto textureType = (NFGE::Graphics::MeshTextureMaterial::ModelTextureType*)(instance);
		*textureType = static_cast<NFGE::Graphics::MeshTextureMaterial::ModelTextureType>(value.GetInt());
	}
}

META_TYPE_DEFINE(NFGE::Graphics::MeshBuffer::Topology, Topology)
META_TYPE_DEFINE(NFGE::Graphics::MeshTextureMaterial::ModelTextureType, TextureType)

void NFGE::Graphics::StaticMetaRegister()
{

}