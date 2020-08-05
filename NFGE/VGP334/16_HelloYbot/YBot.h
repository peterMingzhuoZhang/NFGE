#ifndef NFGE_HELLOYBOT_YBOT_H
#define NFGE_HELLOYBOT_YBOT_H

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
	MovementControllerShell* GetController() {return mControllers[mCurrentUseController];}
	void HookCamera(Camera& camera, float deltaTime);
	void Looking(const std::list<Vector3>& target);

	NFGE::Physics::RigBone* GetHeadRigBone() const { return mHeadRig.get(); };
public:
	Vector3 mLookDir;
	bool mIsLooking = true;
	Vector3 mLookTar = { 0.0f,100.0f,100.0f };
	std::unique_ptr<NFGE::AI::SteeringModule3D> mSteeringModule;

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
	//--- Physics -------------------------------
	std::unique_ptr<NFGE::Physics::RigBone> mHeadRig;
	//-------------------------------------------

private:
	void UpdateSimplePhysics(float deltaTime);
};

#endif // !NFGE_HELLOYBOT_YBOT_H
