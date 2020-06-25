#include "Precompiled.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace NFGE;

META_DERIVED_BEGIN(ColliderComponent, Component)
	META_FIELD_BEGIN
	META_FIELD(center, "Center")
	META_FIELD(extend, "Extend")
	META_FIELD_END
META_CLASS_END;

void NFGE::ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void NFGE::ColliderComponent::Render()
{
	//NFGE::Graphics::SimpleDraw::AddAABB(center + mTransformComponent->position - extend, center + mTransformComponent->position + extend, Graphics::Colors::Gray, true);
	//NFGE::Graphics::SimpleDraw::AddAABB(center + mTransformComponent->position - extend, center + mTransformComponent->position + extend, Graphics::Colors::Cyan, false);
	NFGE::Graphics::SimpleDraw::AddOBB({ center + mTransformComponent->position, extend, mTransformComponent->rotation.mQuaternion }, Graphics::Colors::Red);
}

void NFGE::ColliderComponent::InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass *, uint8_t *))
{
	ShowMetaClassInInspector(GetMetaClass(), (uint8_t*)this);
}
