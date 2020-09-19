#pragma once

#include "PTModel.h"
#include "MovementController.h"
#include "MovementController_AllDirection.h"

class YBot : public  NFGE::AI::Agent3D
{
public:
	YBot(NFGE::AI::AIWorld3D & world, NFGE::Physics::PhysicsWorld& physicsWorld)
		: Agent3D(world)
		, mAnimator()
		, mPhysicsWorld(physicsWorld)
		, mRagBody(physicsWorld, mModel2)
	{
	}

	void Load(const char* idleAnimationFile, const NFGE::Graphics::Color& material0 = NFGE::Graphics::Colors::DeepSkyBlue, const NFGE::Graphics::Color& material1 = NFGE::Graphics::Colors::DarkGray);
	void Load(const NFGE::Graphics::Color& material0 = NFGE::Graphics::Colors::DeepSkyBlue, const NFGE::Graphics::Color& material1 = NFGE::Graphics::Colors::DarkGray);
	void Update(float deltaTime);
	void Render(const Camera& camera);
	void UnLoad();
	void DebugUI();

	void SetInitPosition(const Vector3& pos) { position = pos; destination = pos; }

	Animator& GetAnimator() { return mAnimator; }
	MovementControllerShell* GetController() { return mControllers[mCurrentUseController]; }
	void HookCamera(Camera& camera, float deltaTime);
	void Looking(const std::list<NFGE::Math::Vector3>& target);
	void SenceHitingTarget(const std::list<NFGE::Math::Vector3>& targets);
	void Touching(const std::vector<NFGE::Math::Vector3>& targets);
	
	bool IsHittingReady() const { return mIsReadyHitting; }
	void SetLeftArmTargetting(bool value) { mIsLeftArmTargeting = value; }
	bool GetLeftArmTargetting() const { return mIsLeftArmTargeting; }
	void SetRightArmTargetting(bool value) { mIsRightArmTargeting = value; }
	bool GetRightArmTargetting() const { return mIsRightArmTargeting; }

	const NFGE::Physics::RigBone& GetHeadRigBone() const;
	NFGE::Math::Vector3 GetLeftHandPosition() const { return mLeftHandRig->GetPosition(); }
	NFGE::Math::Vector3 GetRightHandPosition() const { return mRightHandRig->GetPosition(); }

	void SpwanRagDoll();
public:
	Vector3 mLookDir;
	bool mIsLooking = false;
	Vector3 mLookTar = { 0.0f,100.0f,100.0f };
	std::unique_ptr<NFGE::AI::SteeringModule3D> mSteeringModule;

	NFGE::Physics::RagBody mRagBody;

	float mHitPower = 200.0f;
private:
	std::vector<MovementControllerShell*> mControllers;
	std::unique_ptr<MovementController> mController_AI;
	std::unique_ptr<MovementController_AllDirection> mController_Physics;
	int mCurrentUseController = 0;

	Animator mAnimator;
	NFGE::Graphics::SkeletonModel mModel2;
	AnimationSet mAnimmationSet;
	NFGE::Physics::PhysicsWorld& mPhysicsWorld;

	float mSightRange;
	float mHitRange;
	float mTouchRange;

	//--- Physics -------------------------------
	std::unique_ptr<NFGE::Physics::RigBone> mHeadRig;
	bool mIsReadyHitting = false;
	bool mIsLeftArmTargeting = false;
	std::unique_ptr<NFGE::Physics::RigBone> mLeftShoulderRig;
	Vector3 mLeftShouldTar;
	std::unique_ptr<NFGE::Physics::RigBone> mLeftUpArmRig;
	Vector3 mLeftUpArmTar;
	std::unique_ptr<NFGE::Physics::RigBone> mLeftLowerArmRig;
	Vector3 mLeftLowerArmTar;
	std::unique_ptr<NFGE::Physics::RigBone> mLeftHandRig;
	Vector3 mLeftHandTar;

	bool mIsRightArmTargeting = false;
	std::unique_ptr<NFGE::Physics::RigBone> mRightShoulderRig;
	Vector3 mRightShouldTar;
	std::unique_ptr<NFGE::Physics::RigBone> mRightUpArmRig;
	Vector3 mRightUpArmTar;
	std::unique_ptr<NFGE::Physics::RigBone> mRightLowerArmRig;
	Vector3 mRightLowerArmTar;
	std::unique_ptr<NFGE::Physics::RigBone> mRightHandRig;
	Vector3 mRightHandTar;

	float armRigResetTimer = 0.0f;
	float armRigResetTime = 2.0f;

	
	//-------------------------------------------

private:
	void UpdateSimplePhysics(float deltaTime);

	bool isDebugDraw = false;
};
