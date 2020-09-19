//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef NFGE_PHYSICS_RIGBONE_H
#define NFGE_PHYSICS_RIGBONE_H

#include "Common.h"

namespace NFGE::Physics
{
	struct PhysicsShape;
	struct Particle;
	class PhysicsWorld; 
	class Spring;
	class Fixed;
	struct RigBone
	{
		RigBone() = default;
		RigBone(PhysicsWorld& world, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, NFGE::Graphics::Bone* bone);

		NFGE::Graphics::Bone* mBone = nullptr;

		Particle* mChildBoneOri = nullptr;
		std::vector<RigBone*> mChildrenRigBone;
		PhysicsShape* mIdle_front = nullptr;
		PhysicsShape* mIdle_up = nullptr;
		PhysicsShape* mIdle_right = nullptr;
		PhysicsShape* mSuppose_front = nullptr;
		PhysicsShape* mSuppose_up = nullptr;
		PhysicsShape* mSuppose_right = nullptr;
		std::vector<Particle*> mParticles;
		NFGE::Math::Vector3 mInitHeading{};	//TODO:: Seems useless, take it out when confirmed.
		Particle* mOri = nullptr;
		bool mIsEndBone = false;

		NFGE::Math::Vector3 idleDir_front{};
		NFGE::Math::Vector3 idleDir_up{};
		NFGE::Math::Vector3 idleDir_right{};
		NFGE::Math::Matrix4 toWorldMat{};
		NFGE::Math::Matrix4 toLocalParentMat{};
		NFGE::Math::Matrix4 temp{};
		NFGE::Math::Matrix4 currentBoneMatrix{};
		NFGE::Math::Matrix4 mCurrentToParentMatrix{};

		float mRotationSpeed = 1.0f;
		float mTranslationSpeed = 10.0f;
		float mXMinRad = 0.0f;
		float mXMaxRad = 0.0f;
		float mSupposeXRotation = 0.0f;
		float mCurrentXRotation = 0.0f;
		NFGE::Math::Matrix4 mXRotMat{};
		float mYMinRad = 0.0f;
		float mYMaxRad = 0.0f;
		float mSupposeYRotation = 0.0f;
		float mCurrentYRotation = 0.0f;
		NFGE::Math::Matrix4 mYRotMat{};
		float mZMinRad = 0.0f;
		float mZMaxRad = 0.0f;
		float mSupposeZRotation = 0.0f;
		float mCurrentZRotation = 0.0f;
		NFGE::Math::Matrix4 mZRotMat{};

		float adjustToFacingFrontRotation_X = 0.0f;
		float adjustToFacingFrontRotation_Y = 0.0f;
		float adjustToFacingFrontRotation_Z = 0.0f;
		float ModelAdjust_X = 0.0f;
		float ModelAdjust_Y = 0.0f;
		float ModelAdjust_Z = 0.0f;
		float ModelAdjust_X_2 = 0.0f;
		float ModelAdjust_Y_2 = 0.0f;
		float ModelAdjust_Z_2 = 0.0f;
		NFGE::Math::Vector3 tPosfrontVector;
		NFGE::Math::Matrix4 localAdjustMatrix;
		bool isNeedFlipX = false;
		bool isNeedFlipY = false;
		bool isNeedFlipZ = false;
		bool isNeedDirectionFlipX = false;
		bool isFlipInSmallAngle_X = true;
		bool isNeedDirectionFlipY = false;
		bool isFlipInSmallAngle_Y = true;
		bool isNeedDirectionFlipZ = false;
		bool isFlipInSmallAngle_Z = true;

		NFGE::Math::Matrix4 mTranslate{};

		bool mIsDominateByPhysics = false;
		bool mIsOverRotationLimit = false;
		bool mIsLooking = false;
		//bool mIsDead = false;

		NFGE::Math::OBB* mLastHitingOBB = nullptr;
		NFGE::Math::Plane* mLastHitingPlane = nullptr;
		NFGE::Math::Vector3 mLastHitPoint{};
		Particle* mLastHitParticle = nullptr;

		float mOffEffectThrushHoldSqr = 1.0f; // For suppose to idle distance
		float mStableThrushHold = 0.01f; // For accelation
		//Spring* mCurrentToSuppose;
		//Fixed* mFixedSuppose;

		void Binding(const std::vector<NFGE::Math::Matrix4>& boneMatrixes, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, const NFGE::Math::Matrix4& adjustMat, const NFGE::Math::Matrix4& toWorld);
		void LookTo(const NFGE::Math::Vector3& target);
		void RotateWith(const NFGE::Math::Matrix4& animationToParent);
		NFGE::Math::Matrix4 GetRotationTransformQuaternion() const; // Not using anymore
		void CaculateTransform();
		NFGE::Math::Matrix4 GetTransform() const;
		NFGE::Math::Matrix4 GetModelAdjustTransform()  const;
		NFGE::Math::Matrix4 GetModelAdjust_2Transform()  const;

		

		void Update(float deltaTime, bool isLooking = false, const NFGE::Math::Vector3& target = NFGE::Math::Vector3::Zero()); // reset look rotation, smooth the rotation
		void DebugDraw() const;

		NFGE::Math::Vector3 GetPosition() const;
		
		bool IsStable() const;

	private:

		float mRotationAngleX_noConstrain;
		float mRotationAngleY_noConstrain;
		float mRotationAngleZ_noConstrain;
	};
}
#endif // !NFGE_PHYSICS_RIGBONE

