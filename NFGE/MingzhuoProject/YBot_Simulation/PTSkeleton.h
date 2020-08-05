#pragma once
#include "PTBoneMesh.h"

using namespace NFGE::Math;
using namespace NFGE::Graphics;

class PTSkeleton
{
	struct DynamicToParent
	{
		Vector3		mCurrentTranslate;
		Quaternion	mCurrentRotation;
		//TODO:: PhysicsBody	mPhysicsBall;
		//TODO:: Spring			mSpring;
		//TODO:: RotSpring		mRotSpring;
	};

public:
	void Load(const char* fileName);
	void Update(float deltaTime);
	void Render(const Camera& camera);
	void DebugUI();
	void UnLoad();

public:
	PTBoneMesh mBoneMesh;

	Skeleton mSkeleton;
	std::vector<DynamicToParent> mDynamicToParentTransform;
	std::vector<Matrix4> tPosToParentMatrix;
	std::vector<Matrix4> boneMatrix;

	void UpdateToParentTransform(int index);
private:
	void RenderSkeleton(Bone* rootBone, const Camera& camera);
};
