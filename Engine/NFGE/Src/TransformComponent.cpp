#include "Precompiled.h"
#include "TransformComponent.h"

using namespace NFGE;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
	META_FIELD(position, "Position")
	META_FIELD(rotation, "Rotation")
	META_FIELD_END
META_CLASS_END


void NFGE::TransformComponent::Initialize()
{
	rotation.mEularAngle = NFGE::Math::GetEular(rotation.mQuaternion);
}

void NFGE::TransformComponent::Render()
{
}
 