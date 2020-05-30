#ifndef INCLUDED_NFGE_GRAPHICS_ANIMATIONBUILDER_H
#define INCLUDED_NFGE_GRAPHICS_ANIMATIONBUILDER_H

#include "Common.h"

namespace NFGE::Graphics
{
	class Animation;

	class AnimationBuilder
	{
	public:
		AnimationBuilder();
		~AnimationBuilder();

		AnimationBuilder& AddPositionKey(Math::Vector3 position, float time);
		AnimationBuilder& AddRotationKey(Math::Quaternion rotation, float time);
		AnimationBuilder& AddScaleKey(Math::Vector3 scale, float time);
		AnimationBuilder& AddColorKey(Math::Vector4 color, float time);
		AnimationBuilder& AddKey(Math::Vector3 position, Math::Quaternion rotation, float time);
		
		void AdaptExistingAniamtion(std::unique_ptr<Animation>& animationPtr);
		std::unique_ptr<Animation> Build();
	private:
		std::unique_ptr<Animation> mAnimationPtr;
	private:
		void UpdateLastFrame(float time);

	};
}

#endif // !INCLUDED_NFGE_GRAPHICS_ANIMATIONBUILDER_H
