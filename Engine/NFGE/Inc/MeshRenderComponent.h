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
		META_CLASS_DECLARE;
		float mRadius;

		//Editor control variables
		NFGE::Math::Vector4 mAmbientColor;
		NFGE::Math::Vector4 mDiffuseColor;
		NFGE::Math::Vector4 mSpecularColor;
		float bumpWeight;
		std::string mDiffuseTextureDir;
		std::string mSpecularTextureDir;
		std::string mNormalextureDir;
		std::string mDisplacementTextureDir;

	};

	class TransformComponent;
	class MeshRenderComponent : public Component
	{
	public:
		META_CLASS_DECLARE;
		void Initialize() override;
		void Render() override;
		void InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass*, uint8_t*)) override;

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
		
		MeshRenderContext mControlContext;
	


		const TransformComponent* mTransformComponent = nullptr;
	};
}

#endif // !INCLUDE_NFGE_MESHRENDERCOMPONENT_H
