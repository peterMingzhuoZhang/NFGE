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
	auto finalVector = NFGE::Math::Vector3::ZAxis * mTransformComponent->finalTransform;
	auto finalCenter = center + NFGE::Math::Vector3::Zero() * mTransformComponent->finalTransform;

	auto finalScale = NFGE::Math::Magnitude(finalVector - finalCenter);
	auto finalExtend = extend * finalScale;
	
	auto finalRotation = NFGE::Math::RotMatToQuaternion(NFGE::Math::Inverse( mTransformComponent->finalRotationMatrix));

	NFGE::Graphics::SimpleDraw::AddOBB({ finalCenter, finalExtend, finalRotation}, Graphics::Colors::Red);
}

void NFGE::ColliderComponent::InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass *, uint8_t *))
{
	ShowMetaClassInInspector(GetMetaClass(), (uint8_t*)this);
}
