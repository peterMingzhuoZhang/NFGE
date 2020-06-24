#ifndef INCLUDE_NFGE_MESHRENDERCOMPONENT_H
#define INCLUDE_NFGE_MESHRENDERCOMPONENT_H

#include "Component.h"

namespace NFGE
{
	enum MeshRenderGeometryType
	{
		Sphere = 0,
		Cube,
		Cylinder,
		Cone,
		Toro
	};
	struct MeshRenderContext
	{
		float mRadius;

	};

	class TransformComponent;
	class MeshRenderComponent : public Component
	{
	public:
		META_CLASS_DECLARE;
		void Initialize() override;
		void Render() override;

		NFGE::Graphics::MeshBuffer mMeshBuffer;
		MeshRenderContext mContext;
		NFGE::Graphics::Mesh mMesh;
		MeshRenderGeometryType mGemotryType;
		NFGE::Graphics::Effects::StandardMeshEffect::StandardMeshEffectContext mEffectContext;
		NFGE::Graphics::Material mMaterial;
		NFGE::Graphics::TextureId mDiffuseTexture;
		NFGE::Graphics::TextureId mSpecularTexture;
		NFGE::Graphics::TextureId mDisplacementTexture;
		NFGE::Graphics::TextureId mNormalTexture;
		
		//Editor control variables
		NFGE::Math::Vector4 mAmbientColor;
		NFGE::Math::Vector4 mDiffuseColor;
		NFGE::Math::Vector4 mSpecularColor;
		float bumpWeight;
		std::string mDiffuseTextureDir;
		std::string mSpecularTextureDir;
		std::string mDisplacementTextureDir;
		std::string mNormalTextureDir;


		const TransformComponent* mTransformComponent = nullptr;
	};
}

#endif // !INCLUDE_NFGE_MESHRENDERCOMPONENT_H
