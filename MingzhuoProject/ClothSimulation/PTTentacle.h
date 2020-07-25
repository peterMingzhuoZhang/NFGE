#pragma once

#include "NFGE/Inc/NFGE.h"

using namespace NFGE::Graphics;
using namespace NFGE::Math;


// - SelfDefine type --------------------------------------------------------------------------------------------------
// ---||||||||||||||---------------------------------------------------------------------------------------------------
// ---||||||||||||||---------------------------------------------------------------------------------------------------
// ---vvvvvvvvvvvvvv---------------------------------------------------------------------------------------------------
namespace PC	//Peter chan style
{
	struct TentacleSegment
	{
		Bone mBone;
		Matrix4 boneMatrix;
		Vector3		mChildSpwanPoint;
		Vector3		mAnitmatePosition;
		Quaternion	mAnitmateRotation;

		std::vector<std::unique_ptr<Animation>> mAnimations;
		AnimationBuilder mAnimationBuilder;

		TentacleSegment(const Bone& bone, const Matrix4& boneMat, const Vector3& childSpwanPoint, const Vector3& animaitonPos, const Quaternion& animationRot)
			: mBone(bone)
			, boneMatrix(boneMat)
			, mChildSpwanPoint(childSpwanPoint)
			, mAnitmatePosition(animaitonPos)
			, mAnitmateRotation(animationRot)
		{}
		TentacleSegment(const TentacleSegment& other) {};
	};
}
// ---^^^^^^^^^^^^^^---------------------------------------------------------------------------------------------------
// ---||||||||||||||---------------------------------------------------------------------------------------------------
// ---||||||||||||||---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


class PCTentacle	//Peter-Amy Tentacle: [Amy helps me create this one]
{
public:
	void Load(int totalSeg, float baseLength, const char* diffuseMap, const char* nomralMap, const char* specularMap, const char* displaceMentMap);
	void Update(float deltaTime);
	void Render(const Camera& camera);
	void DebugUI();
	void UnLoad();
private:
	void BuildBoneSkelton(int totleBone, float baseLength);
	void ControlSegment(float deltaTime);
	void UpdateSegmentTransform(int index);
private:

	//-------------------------------------------------------------------------------------------
	NFGE::Graphics::BoneMesh mMesh;
	NFGE::Graphics::MeshBuffer mMeshBuffer;
	NFGE::Graphics::Effects::SkeletonModelMeshEffect::SkeletonModelMeshEffectContext mEffectContext;
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------
	int mTotalSeg = 3;
	std::vector<PC::TentacleSegment> mSegs;
	//-------------------------------------------------------------------------------------------
	//- Control ---------------------------------------------------------------------------------
	int mCurrentControlSeg = 0;
	//- Debug -----------------------------------------------------------------------------------
	int mCurrentControlVertex = 0;
	//-------------------------------------------------------------------------------------------
};

