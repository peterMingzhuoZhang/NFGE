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
		RigBone(PhysicsWorld& world, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, NFGE::Graphics::Bone* bone);
		RigBone(PhysicsWorld& world, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, NFGE::Graphics::Bone* bone, RigBone* childBone);

		NFGE::Graphics::Bone* mBone = nullptr;

		PhysicsShape* mIdle_front = nullptr;
		PhysicsShape* mIdle_up = nullptr;
		PhysicsShape* mIdle_right = nullptr;
		PhysicsShape* mSuppose_front = nullptr;
		PhysicsShape* mSuppose_up = nullptr;
		PhysicsShape* mSuppose_right = nullptr;
		std::vector<Particle*> mParticles;
		NFGE::Math::Vector3 mInitHeading{};	//TODO:: Seems useless, take it out when confirmed.
		Particle* mOri = nullptr;
		Particle* mChildBoneOri = nullptr;

		NFGE::Math::Vector3 idleDir_front{};
		NFGE::Math::Vector3 idleDir_up{};
		NFGE::Math::Vector3 idleDir_right{};
		NFGE::Math::Matrix4 toWorldMat{};
		NFGE::Math::Matrix4 temp{};
		NFGE::Math::Matrix4 currentBoneMatrix{};

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

		bool mIsDominateByPhysics = false;
		//Spring* mCurrentToSuppose;
		//Fixed* mFixedSuppose;

		void Binding(const std::vector<NFGE::Math::Matrix4>& boneMatrixes, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, const NFGE::Math::Matrix4& toWorld);
		void LookTo(const NFGE::Math::Vector3& target);
		void RotateWith(const NFGE::Math::Matrix4& animationToParent);
		NFGE::Math::Matrix4 GetRotationTransformQuaternion() const; // Not using anymore
		void CaculateRotationTransformAixes();
		NFGE::Math::Matrix4 GetRotationTransform() const;

		

		void Update(float deltaTime, bool isLooking = false, const NFGE::Math::Vector3& target = NFGE::Math::Vector3::Zero()); // reset look rotation, smooth the rotation
		void DebugDraw() const;

		NFGE::Math::Vector3 GetPosition() const;

	private:
		void ConnectChildRigBone(PhysicsWorld& world, RigBone* childBone,const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes); // Function that takes a rigbone that suppose to be the chhild bone of current bone
	};
}
#endif // !NFGE_PHYSICS_RIGBONE

