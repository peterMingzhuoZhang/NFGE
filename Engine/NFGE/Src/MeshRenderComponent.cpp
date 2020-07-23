#include "Precompiled.h"
#include "MeshRenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "NFGE.h"

using namespace NFGE;

META_CLASS_BEGIN(EditorMeshTexture)
	META_FIELD_BEGIN
	META_FIELD(mType, "Texture Type")
	META_FIELD(mFileName, "File Name")
	META_FIELD_END
META_CLASS_END;

META_CLASS_BEGIN(MeshRenderContext)
	META_FIELD_BEGIN
	META_FIELD(mAmbientColor, "Ambient Color")
	META_FIELD(mDiffuseColor, "Diffuse Color")
	META_FIELD(mSpecularColor, "Specular Color")
	META_FIELD(mSpecualrPower, "Specular Power")
	META_FIELD(mDiffuseTextureDir, "Diffuse Texture")
	META_FIELD(mIsUsingDiffuse, "Is Using Diffuse Texture")
	META_FIELD(mSpecularTextureDir, "Specular Texture")
	META_FIELD(mIsUsingSpecular, "Is Using Specular Texture")
	META_FIELD(mNormalextureDir, "Normal Texture")
	META_FIELD(mIsUsingNormal, "Is Using Normal Texture")
	META_FIELD(mDisplacementTextureDir, "Displacement Texture")
	META_FIELD(mIsUsingDisplacement, "Is Using Displacement Texture")
	META_FIELD(mBumpWeight, "Displacement Weight")
	META_FIELD(mIsCastShadow, "Is Casting Shadow")
	META_FIELD(mTopology, "Topology")
	META_FIELD_END
META_CLASS_END;

META_DERIVED_BEGIN(MeshRenderComponent, Component)
	META_FIELD_BEGIN
	META_FIELD(mGemotryType, "GeometryType")
	META_FIELD(mControlContext, "Mesh Render Context")
	META_FIELD_END
META_CLASS_END;

void NFGE::MeshRenderComponent::Initialize()
{
	MeshRenderGeometryType gemotryType = static_cast<MeshRenderGeometryType>(mGemotryType);

	switch (gemotryType)
	{
	case NFGE::Plane:
		mMesh = NFGE::Graphics::MeshBuilder::CreatePlane(5, 5, 1);
		break;
	case NFGE::Sphere:
		mMesh = NFGE::Graphics::MeshBuilder::CreateSphere(28,28,1.0f);
		break;
	case NFGE::Cube:
		mMesh = NFGE::Graphics::MeshBuilder::CreateCube();
		break;
	case NFGE::Cylinder:
		mMesh = NFGE::Graphics::MeshBuilder::CreateCylinder(8, 16, 1.0f,3.0f);
		break;
	case NFGE::Cone:
		mMesh = NFGE::Graphics::MeshBuilder::CreateCone(8, 16, 1.0f, 3.0f);
		break;
	case NFGE::Toro:
		mMesh = NFGE::Graphics::MeshBuilder::CreateTorus(16, 16, 1.0f, 3.0f);
		break;
	default:
		break;
	}

	mMeshBuffer.Initialize(mMesh);

	mEffectContext.AddTexture(Graphics::MeshTextureMaterial::DIFFUSE, Graphics::TextureManager::Get()->LoadTexture(mControlContext.mDiffuseTextureDir.mFileName));
	mEffectContext.AddTexture(Graphics::MeshTextureMaterial::SPECULAR, Graphics::TextureManager::Get()->LoadTexture(mControlContext.mSpecularTextureDir.mFileName));
	mEffectContext.AddTexture(Graphics::MeshTextureMaterial::DISPLACEMENT, Graphics::TextureManager::Get()->LoadTexture(mControlContext.mDisplacementTextureDir.mFileName));
	mEffectContext.bumpWeight = mControlContext.mBumpWeight;
	mEffectContext.AddTexture(Graphics::MeshTextureMaterial::NORMALS, Graphics::TextureManager::Get()->LoadTexture(mControlContext.mNormalextureDir.mFileName));

	mEffectContext.light = &sApp.GetMainLight();

	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void NFGE::MeshRenderComponent::Render()
{
	mMeshBuffer.SetTopology(mControlContext.mTopology);
	mEffectContext.GetShadowPostProcessContextPtr()->isCastShadow = mControlContext.mIsCastShadow;

	mEffectContext.custumToWorldMatrix = mTransformComponent->finalTransform;

	UpdateTexture(mControlContext.mDiffuseTextureDir);
	UpdateTexture(mControlContext.mSpecularTextureDir);
	UpdateTexture(mControlContext.mNormalextureDir);
	UpdateTexture(mControlContext.mDisplacementTextureDir);
	mEffectContext.bumpWeight = mControlContext.mBumpWeight;

	mEffectContext.TextureUsingSwitch(Graphics::MeshTextureMaterial::ModelTextureType::DIFFUSE, mControlContext.mIsUsingDiffuse);
	mEffectContext.TextureUsingSwitch(Graphics::MeshTextureMaterial::ModelTextureType::SPECULAR, mControlContext.mIsUsingSpecular);
	mEffectContext.TextureUsingSwitch(Graphics::MeshTextureMaterial::ModelTextureType::NORMALS, mControlContext.mIsUsingNormal);
	mEffectContext.TextureUsingSwitch(Graphics::MeshTextureMaterial::ModelTextureType::DISPLACEMENT, mControlContext.mIsUsingDisplacement);

	mEffectContext.material.ambient = mControlContext.mAmbientColor;
	mEffectContext.material.diffuse = mControlContext.mDiffuseColor;
	mEffectContext.material.specular = mControlContext.mSpecularColor;
	mEffectContext.material.power = mControlContext.mSpecualrPower;

	auto effectManager = Graphics::EffectManager::Get();
	Graphics::Effect* effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::StandardMesh);

	effectManager->RegisterRenderObject(effect, &mEffectContext, &mMeshBuffer, NFGE::sApp.GetMainCamera());
}

void NFGE::MeshRenderComponent::InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass *, uint8_t *))
{
	ShowMetaClassInInspector(mControlContext.GetMetaClass(), (uint8_t*)&mControlContext);
}

void NFGE::MeshRenderComponent::UpdateTexture(EditorMeshTexture editorTexture)
{
	if (editorTexture.isUpdated)
	{
		mEffectContext.AddTexture(editorTexture.mType, Graphics::TextureManager::Get()->LoadTexture(editorTexture.mFileName));
		editorTexture.isUpdated = false;
	}
}