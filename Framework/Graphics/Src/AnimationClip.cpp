#include "Precompiled.h"
#include "AnimationClip.h"

using namespace NFGE::Graphics;

bool NFGE::Graphics::AnimationClip::GetTransform(float time, uint32_t boneIndex, Math::Matrix4 & transform) const
{
	auto& boneAnim = boneAnimations[boneIndex];
	if (boneAnim)
	{
		transform = boneAnim->animation->GetAllTransform(time);
		return true;
	}
	return false;
}
