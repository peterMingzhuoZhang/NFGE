#ifndef HELLOSKELETON_PTSFSKELETON
#define HELLOSKELETON_PTSFSKELETON

#include "PTBoneMesh.h"

using namespace NFGE::Math;
using namespace NFGE::Graphics;

class PTSFSkeleton
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
	void Load(const char* fileName, DirectionalLight* light);
	void Update(float deltaTime);
	void Render(const Camera& camera);
	void DebugUI();
	void UnLoad();

private:
	PTBoneMesh mBoneMesh;

	Skeleton mSkeleton;
	std::vector<DynamicToParent> mDynamicToParentTransform;
	std::vector<Matrix4> boneMatrix;
private:
	void RenderSkeleton(Bone* rootBone, const Camera& camera);
	void UpdateToParentTransform(int index);
};

#endif // !HELLOSKELETON_PTSFSKELETON

