#include "Precompiled.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace NFGE;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
	META_FIELD(position, "Position")
	META_FIELD(rotation, "Rotation")
	META_FIELD(scale, "Scale")
	META_FIELD_END
META_CLASS_END


void NFGE::TransformComponent::Initialize()
{
	rotation.mEularAngle = NFGE::Math::GetEular(rotation.mQuaternion);
}

void NFGE::TransformComponent::Render()
{
	auto finalCenter = NFGE::Math::Vector3::Zero() * finalTransform;
	NFGE::Graphics::SimpleDraw::AddSphere({ finalCenter , 0.25f }, NFGE::Graphics::Colors::Green);
}

void NFGE::TransformComponent::InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass *, uint8_t *))
{
	ShowMetaClassInInspector(GetMetaClass(), (uint8_t*)this);
}

void NFGE::TransformComponent::UpdateFinalTransform(GameObject * gameObject)
{
	auto parent = gameObject->GetParent();
	if (parent == nullptr)
	{
		finalTransform = NFGE::Math::Matrix4::sIdentity();
		auto rotationMat = NFGE::Math::MatrixRotationQuaternion(rotation.mQuaternion);

		finalTransform = rotationMat * NFGE::Math::Matrix4::sScaling(scale)  * NFGE::Math::Matrix4::sTranslation(position) * finalTransform;
		finalRotationMatrix = rotationMat * NFGE::Math::Matrix4::sTranslation(position);
	}
	else
	{
		auto rotationMat = NFGE::Math::MatrixRotationQuaternion(rotation.mQuaternion);
		finalTransform = rotationMat * NFGE::Math::Matrix4::sScaling(scale)  * NFGE::Math::Matrix4::sTranslation(position) * parent->GetComponent<TransformComponent>()->finalTransform;
		finalRotationMatrix = NFGE::Math::MatrixRotationQuaternion(rotation.mQuaternion) * NFGE::Math::Matrix4::sTranslation(position) * parent->GetComponent<TransformComponent>()->finalRotationMatrix;
	}

	for (auto child : gameObject->GetChildren())
	{
		child->GetComponent<TransformComponent>()->UpdateFinalTransform(child);
	}
}
 