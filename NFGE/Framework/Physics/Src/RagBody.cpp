#include "Precompiled.h"
#include "RagBody.h"
#include "PhysicsWorld.h"
#include "..\Inc\RagBody.h"

using namespace NFGE::Math;
using namespace NFGE::Graphics;
using namespace NFGE::Physics;

namespace
{
	const size_t NonInitIndex = MAXSIZE_T;

	const size_t TotalBoneCount = 128;

	const size_t RootBoneIndex = 52;
	const size_t HipBoneIndex = 0;
	const size_t Spine3BoneIndex = 1;
	const size_t Spine2BoneIndex = 2;
	const size_t Spine1BoneIndex = 3;
	const size_t NeckBoneIndex = 4;
	const size_t HeadBoneIndex = 5;

	const size_t RightShoulderBoneIndex = 25;
	const size_t RightArmBoneIndex = 26;
	const size_t RightForeArmBoneIndex = 27;
	const size_t RightHandBoneIndex = 28;
	const size_t LeftShoulderBoneIndex = 6;
	const size_t LeftArmBoneIndex = 7;
	const size_t LeftForeArmBoneIndex = 8;
	const size_t LeftHandBoneIndex = 9;
	const size_t RightUpLegBoneIndex = 44;
	const size_t RightLegBoneIndex = 45;
	const size_t RightFootBoneIndex = 46;
	const size_t RightFootToeIndex = 47;
	const size_t LeftUpLegBoneIndex = 48;
	const size_t LeftLegBoneIndex = 49;
	const size_t LeftFootBoneIndex = 50;
	const size_t LeftFootToeIndex = 51;

	int forceOnBoneIndex = 0; //Debug use

	float resetThreshHold = 0.2f;
	
}

void NFGE::Physics::RagPart::BuildVerletBox(size_t boneIndex, PhysicsWorld & world, float length, float height, float width, const NFGE::Math::Matrix4 & boneMatrix, const NFGE::Graphics::SkeletonModel& skeleton)
{
	mIdleToParentMatrix = skeleton.mContext.mSkeleton.bones[boneIndex]->toParentTransform;

	mBoneIndex = boneIndex;

	mLength = length;
	mHeight = height;
	mWidth = width;

	float halfLength = length * 0.5f;
	float halfWidth = width * 0.5f;
	float halfHeight = height* 0.5f;
	float vertexRadius = 0.1f;
	float vertexInveMass = 1.0f;

	pCenter = world.AddParticle(new Particle
		{
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	pFUL = world.AddParticle(new Particle
		{
			{ -halfLength, halfHeight, -halfWidth },
			{ -halfLength, halfHeight, -halfWidth },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	pFUR = world.AddParticle(new Particle
		{
			{ halfLength, halfHeight, -halfWidth },
			{ halfLength, halfHeight, -halfWidth },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	pFBL = world.AddParticle(new Particle
		{
			{ -halfLength, -halfHeight, -halfWidth },
			{ -halfLength, -halfHeight, -halfWidth },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	pFBR = world.AddParticle(new Particle
		{
			{ halfLength, -halfHeight, -halfWidth },
			{ halfLength, -halfHeight, -halfWidth },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	pBUL = world.AddParticle(new Particle
		{
			{ -halfLength, halfHeight, halfWidth },
			{ -halfLength, halfHeight, halfWidth },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	pBUR = world.AddParticle(new Particle
		{
			{ halfLength, halfHeight, halfWidth },
			{ halfLength, halfHeight, halfWidth },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	pBBL = world.AddParticle(new Particle
		{
			{ -halfLength, -halfHeight, halfWidth },
			{ -halfLength, -halfHeight, halfWidth },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	pBBR = world.AddParticle(new Particle
		{
			{ halfLength, -halfHeight, halfWidth },
			{ halfLength, -halfHeight, halfWidth },
			{ 0.0f, 0.0f, 0.0f },
			vertexRadius,
			vertexInveMass
		});

	world.AddConstraint(new Spring(pFUL, pFUR));
	world.AddConstraint(new Spring(pFUR, pFBR));
	world.AddConstraint(new Spring(pFBR, pFBL));
	world.AddConstraint(new Spring(pFUL, pFBL));
	world.AddConstraint(new Spring(pFUL, pBUL));
	world.AddConstraint(new Spring(pFBL, pBBL));
	world.AddConstraint(new Spring(pBUL, pBBL));
	world.AddConstraint(new Spring(pBUL, pBUR));
	world.AddConstraint(new Spring(pBBL, pBBR));
	world.AddConstraint(new Spring(pBUR, pBBR));
	world.AddConstraint(new Spring(pBUR, pFUR));
	world.AddConstraint(new Spring(pBBR, pFBR));

	world.AddConstraint(new Spring(pFUL, pFBR));
	world.AddConstraint(new Spring(pFUR, pFBL));
	world.AddConstraint(new Spring(pFUL, pBBL));
	world.AddConstraint(new Spring(pFBL, pBUL));
	world.AddConstraint(new Spring(pBUL, pBBR));
	world.AddConstraint(new Spring(pBBL, pBUR));
	world.AddConstraint(new Spring(pBUR, pFBR));
	world.AddConstraint(new Spring(pBBR, pFUR));
	world.AddConstraint(new Spring(pFUL, pBUR));
	world.AddConstraint(new Spring(pFUR, pBUL));
	world.AddConstraint(new Spring(pFBR, pBBL));
	world.AddConstraint(new Spring(pFBL, pBBR));
	world.AddConstraint(new Spring(pFUL, pBBR));
	world.AddConstraint(new Spring(pFUR, pBBL));
	world.AddConstraint(new Spring(pBUL, pFBR));
	world.AddConstraint(new Spring(pBUR, pFBL));

	world.AddConstraint(new Spring(pFUL, pCenter));
	world.AddConstraint(new Spring(pFUR, pCenter));
	world.AddConstraint(new Spring(pFBL, pCenter));
	world.AddConstraint(new Spring(pFBR, pCenter));
	world.AddConstraint(new Spring(pBUL, pCenter));
	world.AddConstraint(new Spring(pBUR, pCenter));
	world.AddConstraint(new Spring(pBBL, pCenter));
	world.AddConstraint(new Spring(pBBR, pCenter));

	Matrix4 finalToWorldMat = boneMatrix;

	pCenter->SetPosition(pCenter->position * finalToWorldMat);
	pFUL->SetPosition(pFUL->position * finalToWorldMat);
	pFUR->SetPosition(pFUR->position * finalToWorldMat);
	pFBL->SetPosition(pFBL->position * finalToWorldMat);
	pFBR->SetPosition(pFBR->position * finalToWorldMat);
	pBUL->SetPosition(pBUL->position * finalToWorldMat);
	pBUR->SetPosition(pBUR->position * finalToWorldMat);
	pBBL->SetPosition(pBBL->position * finalToWorldMat);
	pBBR->SetPosition(pBBR->position * finalToWorldMat);

	pCenter->gravityScale = 0.0f;			//|
	pFUL->gravityScale = 0.0f;			//|
	pFUR->gravityScale = 0.0f;			//|
	pFBL->gravityScale = 0.0f;			//|
	pFBR->gravityScale = 0.0f;			//|--- Debug purpose
	pBUL->gravityScale = 0.0f;			//|
	pBUR->gravityScale = 0.0f;			//|
	pBBL->gravityScale = 0.0f;			//|
	pBBR->gravityScale = 0.0f;			//|

	mLastCenter = pCenter->position;
}

void NFGE::Physics::RagPart::SetBone(NFGE::Graphics::SkeletonModel& skeleton, const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Math::Matrix4& adjustMatrix)
{
	//pCenter->SetVelocity({ 0.0f,0.0f,0.0f });
	//pFUL->SetVelocity({ 0.0f,0.0f,0.0f });
	//pFUR->SetVelocity({ 0.0f,0.0f,0.0f });
	//pFBL->SetVelocity({ 0.0f,0.0f,0.0f });
	//pFBR->SetVelocity({ 0.0f,0.0f,0.0f });
	//pBUL->SetVelocity({ 0.0f,0.0f,0.0f });
	//pBUR->SetVelocity({ 0.0f,0.0f,0.0f });
	//pBBL->SetVelocity({ 0.0f,0.0f,0.0f });
	//pBBR->SetVelocity({ 0.0f,0.0f,0.0f });


	auto boneMatrix = skeleton.mContext.boneMatrix[skeleton.mContext.mSkeleton.bones[mBoneIndex]->parentIndex];
	auto inverseBoneMatrix = NFGE::Math::Inverse( NFGE::Math::Inverse(adjustMatrix) * boneMatrix * adjustMatrix * toWorldMatrix);

	auto localBUL = pBUL->position * inverseBoneMatrix;
	auto localBBL = pBBL->position * inverseBoneMatrix;
	auto localBBR = pBBR->position * inverseBoneMatrix;
	auto localFBL = pFBL->position * inverseBoneMatrix;

	auto upVector = NFGE::Math::Normalize(localBUL - localBBL);
	auto rightVector = -NFGE::Math::Normalize(localBBR - localBBL);
	auto frontVector = NFGE::Math::Normalize(localFBL - localBBL);

	auto translateMatrix = NFGE::Math::Matrix4::sTranslation(pCenter->position *inverseBoneMatrix - Vector3(0.0f, 0.0f, 0.0f));

	Vector3 z = Normalize(frontVector);
	Vector3 x = Normalize(Cross(upVector, z));
	Vector3 y = Normalize(Cross(z, x));
	Matrix4 rotationMatrix
	(
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	auto finalMat = rotationMatrix * translateMatrix * adjustMatrix;

	skeleton.mContext.mSkeleton.bones[mBoneIndex]->toParentTransform = finalMat;

}

void NFGE::Physics::RagPart::ConnectOther(PhysicsWorld & world, const RagPart & other)
{
	
	//world.AddConstraint(new Spring(pFUL, other.pFUL));
	//world.AddConstraint(new Spring(pFUR, other.pFUR));
	//world.AddConstraint(new Spring(pFBL, other.pFBL));
	//world.AddConstraint(new Spring(pFBR, other.pFBR));
	//world.AddConstraint(new Spring(pBUL, other.pBUL));
	//world.AddConstraint(new Spring(pBUR, other.pBUR));
	//world.AddConstraint(new Spring(pBBL, other.pBBL));
	//world.AddConstraint(new Spring(pBBR, other.pBBR));

	world.AddConstraint(new Spring(pCenter, other.pFUL));
	world.AddConstraint(new Spring(pCenter, other.pFUR));
	world.AddConstraint(new Spring(pCenter, other.pFBL));
	world.AddConstraint(new Spring(pCenter, other.pFBR));
	world.AddConstraint(new Spring(pCenter, other.pBUL));
	world.AddConstraint(new Spring(pCenter, other.pBUR));
	world.AddConstraint(new Spring(pCenter, other.pBBL));
	world.AddConstraint(new Spring(pCenter, other.pBBR));
	world.AddConstraint(new Spring(pCenter, other.pCenter));

}

void NFGE::Physics::RagPart::SimpleConnect(PhysicsWorld & world, const RagPart& other)
{
	world.AddConstraint(new Spring(pCenter, other.pCenter));
}

void NFGE::Physics::RagPart::DebugDraw()
{
	NFGE::Graphics::SimpleDraw::AddSphere(Sphere{ pFUL->position,.2f }, NFGE::Graphics::Colors::Red);
	NFGE::Graphics::SimpleDraw::AddSphere(Sphere{ pFUR->position,.2f }, NFGE::Graphics::Colors::Green);
	NFGE::Graphics::SimpleDraw::AddSphere(Sphere{ pFBL->position,.2f }, NFGE::Graphics::Colors::Orange);
	NFGE::Graphics::SimpleDraw::AddSphere(Sphere{ pFBR->position,.2f }, NFGE::Graphics::Colors::Blue);
}

NFGE::Physics::RagBody::RagBody(PhysicsWorld & world, NFGE::Graphics::SkeletonModel & skeleton)
	: mWorld(world)
	, mSkeletonModel(skeleton)
{
	
	
}

void NFGE::Physics::RagBody::BuildDynamicRigDoll(const Matrix4& adjustMatrix, const Matrix4& toWorldMatrix)
{
	mAdjustMat = adjustMatrix;
	mToWorldMat = toWorldMatrix;
	
	auto& boneMatries = mSkeletonModel.mContext.boneMatrix;
	auto finalToWorldMat = NFGE::Math::Inverse(mAdjustMat) * toWorldMatrix;
	// First build
	mHip.BuildVerletBox(HipBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat * boneMatries[HipBoneIndex] * finalToWorldMat, mSkeletonModel);
	// Left Leg
	mLeftHip.BuildVerletBox(LeftUpLegBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[LeftUpLegBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mLeftHip, &mSkeletonModel, DEG2RAD(90), DEG2RAD(90), DEG2RAD(10), DEG2RAD(10), DEG2RAD(60), DEG2RAD(10)));
	mLeftUpLeg.BuildVerletBox(LeftLegBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[LeftLegBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mLeftUpLeg, &mSkeletonModel, DEG2RAD(10), DEG2RAD(120), DEG2RAD(30), DEG2RAD(30), DEG2RAD(30), DEG2RAD(30)));
	mLeftLowerLeg.BuildVerletBox(LeftFootBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[LeftFootBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mLeftLowerLeg, &mSkeletonModel, DEG2RAD(30), DEG2RAD(90), DEG2RAD(60), DEG2RAD(60), DEG2RAD(30), DEG2RAD(30)));
	mLeftFoot.BuildVerletBox(LeftFootToeIndex, mWorld, 5.0f, 5.0f, 10.0f, mAdjustMat  * boneMatries[LeftFootToeIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mLeftFoot, &mSkeletonModel, DEG2RAD(30), DEG2RAD(30), DEG2RAD(10), DEG2RAD(10), DEG2RAD(30), DEG2RAD(30)));

	// Right Leg
	mRightHip.BuildVerletBox(RightUpLegBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[RightUpLegBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mRightHip, &mSkeletonModel, DEG2RAD(90), DEG2RAD(90), DEG2RAD(10), DEG2RAD(10), DEG2RAD(60), DEG2RAD(10)));
	mRightUpLeg.BuildVerletBox(RightLegBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[RightLegBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mRightUpLeg, &mSkeletonModel, DEG2RAD(10), DEG2RAD(120), DEG2RAD(30), DEG2RAD(30), DEG2RAD(30), DEG2RAD(30)));
	mRightLowerLeg.BuildVerletBox(RightFootBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[RightFootBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mRightLowerLeg, &mSkeletonModel, DEG2RAD(30), DEG2RAD(90), DEG2RAD(60), DEG2RAD(60), DEG2RAD(30), DEG2RAD(30)));
	mRightFoot.BuildVerletBox(RightFootToeIndex, mWorld, 5.0f, 5.0f, 10.0f, mAdjustMat  * boneMatries[RightFootToeIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mRightFoot, &mSkeletonModel, DEG2RAD(30), DEG2RAD(30), DEG2RAD(10), DEG2RAD(10), DEG2RAD(30), DEG2RAD(30)));
	
	//// Body 
	mSpineLow.BuildVerletBox(Spine3BoneIndex, mWorld, 15.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[Spine3BoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mSpineLow, &mSkeletonModel, DEG2RAD(60), DEG2RAD(60), DEG2RAD(90), DEG2RAD(90), DEG2RAD(20), DEG2RAD(20)));
	mSpineMid.BuildVerletBox(Spine2BoneIndex, mWorld, 15.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[Spine2BoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mSpineMid, &mSkeletonModel, DEG2RAD(60), DEG2RAD(60), DEG2RAD(90), DEG2RAD(90), DEG2RAD(20), DEG2RAD(20)));
	mSpineHigh.BuildVerletBox(Spine1BoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[Spine1BoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mSpineHigh, &mSkeletonModel, DEG2RAD(60), DEG2RAD(60), DEG2RAD(90), DEG2RAD(90), DEG2RAD(20), DEG2RAD(20)));
	mNeck.BuildVerletBox(NeckBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[NeckBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mNeck, &mSkeletonModel, DEG2RAD(60), DEG2RAD(60), DEG2RAD(90), DEG2RAD(90), DEG2RAD(20), DEG2RAD(20)));
	//
	// Left Arm
	mLeftHighSpine.BuildVerletBox(LeftShoulderBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[LeftShoulderBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mLeftHighSpine, &mSkeletonModel, DEG2RAD(360), DEG2RAD(360), DEG2RAD(30), DEG2RAD(30), DEG2RAD(20), DEG2RAD(20)));
	mLeftShoulder.BuildVerletBox(LeftArmBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[LeftArmBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mLeftShoulder, &mSkeletonModel, DEG2RAD(360), DEG2RAD(360), DEG2RAD(90), DEG2RAD(10), DEG2RAD(90), DEG2RAD(90)));
	mLeftUpArm.BuildVerletBox(LeftForeArmBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[LeftForeArmBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mLeftUpArm, &mSkeletonModel, DEG2RAD(360), DEG2RAD(360), DEG2RAD(90), DEG2RAD(10), DEG2RAD(90), DEG2RAD(90)));
	mLeftLowerArm.BuildVerletBox(LeftHandBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[LeftHandBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mLeftLowerArm, &mSkeletonModel, DEG2RAD(360), DEG2RAD(360), DEG2RAD(90), DEG2RAD(10), DEG2RAD(90), DEG2RAD(90)));
	//
	// Right Arm
	mRightHighSpine.BuildVerletBox(RightShoulderBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[RightShoulderBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mRightHighSpine, &mSkeletonModel, DEG2RAD(360), DEG2RAD(360), DEG2RAD(30), DEG2RAD(30), DEG2RAD(20), DEG2RAD(20)));
	mRightShoulder.BuildVerletBox(RightArmBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[RightArmBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mRightShoulder, &mSkeletonModel, DEG2RAD(360), DEG2RAD(360), DEG2RAD(90), DEG2RAD(10), DEG2RAD(90), DEG2RAD(90)));
	mRightUpArm.BuildVerletBox(RightForeArmBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[RightForeArmBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mRightUpArm, &mSkeletonModel, DEG2RAD(360), DEG2RAD(360), DEG2RAD(90), DEG2RAD(10), DEG2RAD(90), DEG2RAD(90)));
	mRightLowerArm.BuildVerletBox(RightHandBoneIndex, mWorld, 5.0f, 5.0f, 5.0f, mAdjustMat  * boneMatries[RightHandBoneIndex] * finalToWorldMat, mSkeletonModel);
	mWorld.AddConstraint(new JointRotation(mWorld, &mRightLowerArm, &mSkeletonModel, DEG2RAD(360), DEG2RAD(360), DEG2RAD(90), DEG2RAD(10), DEG2RAD(90), DEG2RAD(90)));


	// Connect Left Leg
	mLeftHip.ConnectOther(mWorld, mHip);
	mLeftUpLeg.ConnectOther(mWorld, mLeftHip);
	mLeftLowerLeg.ConnectOther(mWorld, mLeftUpLeg);
	mLeftFoot.ConnectOther(mWorld, mLeftLowerLeg);
	// Connect Right Leg
	mRightHip.ConnectOther(mWorld, mHip);
	mRightUpLeg.ConnectOther(mWorld, mRightHip);
	mRightLowerLeg.ConnectOther(mWorld, mRightUpLeg);
	mRightFoot.ConnectOther(mWorld, mRightLowerLeg);
	//// Connect Body
	mSpineLow.ConnectOther(mWorld, mHip);
	mSpineMid.ConnectOther(mWorld, mSpineLow);
	mSpineHigh.ConnectOther(mWorld, mSpineMid);
	mNeck.ConnectOther(mWorld, mSpineHigh);
	//// Connect Left Arm
	mLeftHighSpine.ConnectOther(mWorld, mSpineHigh);
	mLeftShoulder.ConnectOther(mWorld, mLeftHighSpine);
	mLeftUpArm.ConnectOther(mWorld, mLeftShoulder);
	mLeftLowerArm.ConnectOther(mWorld, mLeftUpArm);
	//// Connect Right Arm
	mRightHighSpine.ConnectOther(mWorld, mSpineHigh);
	mRightShoulder.ConnectOther(mWorld, mRightHighSpine);
	mRightUpArm.ConnectOther(mWorld, mRightShoulder);
	mRightLowerArm.ConnectOther(mWorld, mRightUpArm);

	// Freeze Rotation Connection
	//mLeftLowerLeg.SimpleConnect(mWorld, mLeftHip);
	//mLeftLowerLeg.SimpleConnect(mWorld, mRightHip);
	//mLeftFoot.SimpleConnect(mWorld, mLeftUpLeg);
	//mRightLowerLeg.SimpleConnect(mWorld, mRightHip);
	//mRightLowerLeg.SimpleConnect(mWorld, mLeftHip);
	//mRightFoot.SimpleConnect(mWorld, mRightUpLeg);

	//mLeftLowerArm.SimpleConnect(mWorld, mLeftShoulder);
	////mLeftLowerArm.SimpleConnect(mWorld, mSpineLow);
	//mLeftLowerArm.ConnectOther(mWorld, mSpineLow);
	//mLeftUpArm.ConnectOther(mWorld, mSpineMid);
	//mLeftUpArm.SimpleConnect(mWorld, mSpineHigh);
	//
	//mRightLowerArm.SimpleConnect(mWorld, mRightShoulder);
	////mRightLowerArm.SimpleConnect(mWorld, mSpineLow);
	//mRightLowerArm.ConnectOther(mWorld, mSpineLow);
	//mRightUpArm.ConnectOther(mWorld, mSpineMid);
	//mRightUpArm.SimpleConnect(mWorld, mSpineHigh);


	mIsActive = true;
}

void NFGE::Physics::RagBody::SetSkeleton(Vector3& worldPosition)
{
	mHip.SetBone(mSkeletonModel, mToWorldMat ,mAdjustMat);
	// Left Leg
	mLeftHip.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mLeftUpLeg.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mLeftLowerLeg.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mLeftFoot.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	// Right Leg
	mRightHip.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mRightUpLeg.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mRightLowerLeg.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mRightFoot.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	//// Body
	mSpineLow.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mSpineMid.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mSpineHigh.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mNeck.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	//// Left Arm
	mLeftHighSpine.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mLeftShoulder.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mLeftUpArm.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mLeftLowerArm.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	//// Right Arm
	mRightHighSpine.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mRightShoulder.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mRightUpArm.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
	mRightLowerArm.SetBone(mSkeletonModel, mToWorldMat, mAdjustMat);
}

void NFGE::Physics::RagBody::DebugDraw()
{
	mHip.DebugDraw();
	mLeftHip.DebugDraw();
	mLeftUpLeg.DebugDraw();
	mLeftLowerLeg.DebugDraw();
	mLeftFoot.DebugDraw();
	//mRightHip.DebugDraw();
	//mRightUpLeg.DebugDraw();
	//mRightLowerLeg.DebugDraw();
	//mRightFoot.DebugDraw();
}

void NFGE::Physics::RagBody::AdjustBoneOrientation(size_t boneIndex, const Vector3& front, const Vector3& up, const Vector3& right)
{

}


