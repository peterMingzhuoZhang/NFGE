#ifndef INCLUDE_NFGE_MESHRENDERCOMPONENT_H
#define INCLUDE_NFGE_MESHRENDERCOMPONENT_H

#include "Component.h"

namespace NFGE
{
	enum MeshRenderGeometryType
	{
		Plane = 0,
		Cube,
		Sphere,
		Cylinder,
		Cone,
		Toro,
		MAX
	};
	
	const std::array<const char*, (uint32_t)MeshRenderGeometryType::MAX> MeshRenderGeometryTypeName = {
		{
			"Plane",
			"Cube",
			"Sphere",
			"Cylinder",
			"Cone",
			"Toro",
		}
	};

	struct MeshRenderContext	// Thing that only expose to use to change
	{
		META_CLASS_DECLARE;
		float mRadius;

		//Editor control variables
		NFGE::Math::Vector4 mAmbientColor{ 1.0f,0.5f,0.5f, 1.0f};
		NFGE::Math::Vector4 mDiffuseColor{ 1.0f,0.5f,0.5f, 1.0f};
		NFGE::Math::Vector4 mSpecularColor{ 1.0f,0.5f,0.5f, 1.0f};
		float mSpecualrPower{10.0f};
		float mBumpWeight{0.0f};
		std::string mDiffuseTextureDir;
		bool mIsUsingDiffuse = false;
		std::string mSpecularTextureDir;
		bool mIsUsingSpecular = false;
		std::string mNormalextureDir;
		bool mIsUsingNormal = false;
		std::string mDisplacementTextureDir;
		bool mIsUsingDisplacement = false;
		bool mIsCastShadow = false;
		NFGE::Graphics::MeshBuffer::Topology mTopology{ NFGE::Graphics::MeshBuffer::Topology::Triangles };
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
		int mGemotryType;
		NFGE::Graphics::Mesh mMesh;
		NFGE::Graphics::Effects::StandardMeshEffect::StandardMeshEffectContext mEffectContext;
		NFGE::Graphics::TextureId mDiffuseTexture;
		NFGE::Graphics::TextureId mSpecularTexture;
		NFGE::Graphics::TextureId mDisplacementTexture;
		NFGE::Graphics::TextureId mNormalTexture;
		
		MeshRenderContext mControlContext;
	
		const TransformComponent* mTransformComponent = nullptr;
		
	};
}

#endif // !INCLUDE_NFGE_MESHRENDERCOMPONENT_H
