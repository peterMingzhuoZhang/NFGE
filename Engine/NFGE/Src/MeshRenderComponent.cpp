#include "Precompiled.h"
#include "MeshRenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace NFGE;

META_CLASS_BEGIN(MeshRenderContext)
	META_FIELD_BEGIN
	META_FIELD(mAmbientColor, "Ambient Color")
	META_FIELD(mDiffuseColor, "Diffuse Color")
	META_FIELD(mSpecularColor, "Specular Color")
	META_FIELD(mDiffuseTextureDir, "Diffuse Texture")
	META_FIELD(mSpecularTextureDir, "Specular Texture")
	META_FIELD(mNormalextureDir, "Normal Texture")
	META_FIELD(mDisplacementTextureDir, "displacement Texture")
	META_FIELD(bumpWeight, "Displacement weight")
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
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void NFGE::MeshRenderComponent::Render()
{

}

void NFGE::MeshRenderComponent::InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass *, uint8_t *))
{
	ShowMetaClassInInspector(mControlContext.GetMetaClass(), (uint8_t*)&mControlContext);
}
