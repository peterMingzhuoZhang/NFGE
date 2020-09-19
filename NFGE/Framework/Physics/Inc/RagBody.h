//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef NFGE_PHYSICS_RIGBODY_H
#define NFGE_PHYSICS_RIGBODY_H

using namespace NFGE::Math;

namespace NFGE::Physics
{
	class PhysicsWorld;
	class Particle;
	class JointRotation;
	struct RagPart
	{
		Particle* pCenter = nullptr;	// particle center

		Particle* pFUL = nullptr;	// particle front up left
		Particle* pFUR = nullptr;	// particle front up right
		Particle* pFBL = nullptr;	// particle front bottom left
		Particle* pFBR = nullptr;	// particle front bottom right
		Particle* pBUL = nullptr;	// particle back up left
		Particle* pBUR = nullptr;	// particle back up right
		Particle* pBBL = nullptr;	// particle back bottom left
		Particle* pBBR = nullptr;	// particle back bottom right

		RagPart* mChildRag = nullptr;
		JointRotation* jointConstrain = nullptr;
		Matrix4 mIdleToParentMatrix;

		size_t mBoneIndex = -1;
		float mLength = 0.0f;
		float mHeight = 0.0f;
		float mWidth = 0.0f;

		Math::Vector3 mLastCenter;

		void BuildVerletBox(size_t boneIndex, PhysicsWorld& world, float length, float height, float width, const NFGE::Math::Matrix4 & boneMatrix, const NFGE::Graphics::SkeletonModel& skeleton);
		void SetBone(NFGE::Graphics::SkeletonModel& skeleton, const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Math::Matrix4& adjustMatrix);
		void ConnectOther(PhysicsWorld & world, const RagPart& other);
		void SimpleConnect(PhysicsWorld & world, const RagPart& other);

		void DebugDraw();



	};

	class RagBody
	{
	public:
		RagBody(PhysicsWorld& world, NFGE::Graphics::SkeletonModel& skeleton);

		void BuildDynamicRigDoll(const Matrix4& adjustMatrix, const Matrix4& toWorldMatrix);
		void SetSkeleton(Vector3& worldPosition);

		void DebugDraw();

		bool mIsActive = false;
	private:
		Physics::PhysicsWorld& mWorld;
		NFGE::Graphics::SkeletonModel& mSkeletonModel;

		NFGE::Math::Matrix4 mToWorldMat;
		NFGE::Math::Matrix4 mAdjustMat;
	public:
		RagPart mHip;
		// Left Leg
		RagPart mLeftHip;
		RagPart mLeftUpLeg;
		RagPart mLeftLowerLeg;
		RagPart mLeftFoot;
		// Right Leg
		RagPart mRightHip;
		RagPart mRightUpLeg;
		RagPart mRightLowerLeg;
		RagPart mRightFoot;
		// Body
		RagPart mSpineLow;
		RagPart mSpineMid;
		RagPart mSpineHigh;
		RagPart mNeck;
		// Left arm
		RagPart mLeftHighSpine;
		RagPart mLeftShoulder;
		RagPart mLeftUpArm;
		RagPart mLeftLowerArm;

		// Right arm
		RagPart mRightHighSpine;
		RagPart mRightShoulder;
		RagPart mRightUpArm;
		RagPart mRightLowerArm;


		void AdjustBoneOrientation(size_t boneIndex, const Vector3& front, const Vector3& up, const Vector3& right);
	};
}

#endif // !NFGE_PHYSICS_RIGBODY_H
