#include "YBot.h"

using namespace NFGE::AI;

namespace
{
	const char* SkeletonFileName = "../../Assets/Model/ybot.NFSkeleton";
	const char* modelFileName_0 = "../../Assets/Model/ybot.NFModelRV";
	const wchar_t* shaderFileName = L"../../Assets/Shaders/Standard_Model_With_Skin_ZMZ.fx";

	const char* idleAnimation = "../../Assets/Model/Idle.NFAnimationSet";

	const std::vector<const char*> animationFiles = {
		"../../Assets/Model/Walking.NFAnimationSet",
		"../../Assets/Model/Running.NFAnimationSet",
		"../../Assets/Model/NarutoRun.NFAnimationSet",
		"../../Assets/Model/JoyfulJump.NFAnimationSet",
		"../../Assets/Model/BboyUprock.NFAnimationSet",
		"../../Assets/Model/Punching_left.NFAnimationSet",
		"../../Assets/Model/Punching_right.NFAnimationSet",
		"../../Assets/Model/Hurricane_Kick.NFAnimationSet",
		"../../Assets/Model/Idle_gun.NFAnimationSet",								//|9
		"../../Assets/Model/Walk_forward.NFAnimationSet",							//|10
		"../../Assets/Model/Walk_forward_right.NFAnimationSet",						//|11
		"../../Assets/Model/Walk_right.NFAnimationSet",								//|12
		"../../Assets/Model/Walk_backward_right.NFAnimationSet",					//|13
		"../../Assets/Model/Walk_backward.NFAnimationSet",							//|14
		"../../Assets/Model/Walk_backward_left.NFAnimationSet",						//|15
		"../../Assets/Model/Walk_left.NFAnimationSet",								//|16
		"../../Assets/Model/Walk_forward_left.NFAnimationSet",						//|17

		"../../Assets/Model/Run_forward.NFAnimationSet",							//|18
		"../../Assets/Model/Run_forward_right.NFAnimationSet",						//|19
		"../../Assets/Model/Run_right.NFAnimationSet",								//|20
		"../../Assets/Model/Run_backward_right.NFAnimationSet",						//|21
		"../../Assets/Model/Run_backward.NFAnimationSet",							//|22
		"../../Assets/Model/Run_backward_left.NFAnimationSet",						//|23
		"../../Assets/Model/Run_left.NFAnimationSet",								//|24
		"../../Assets/Model/Run_forward_left.NFAnimationSet",						//|25

		"../../Assets/Model/IdleGunTurnRight_45.NFAnimationSet",					//|26
		"../../Assets/Model/IdleGunTurnLeft_45.NFAnimationSet",						//|27
		"../../Assets/Model/Idle_rifle.NFAnimationSet",								//|28
	};

	const int UpperBodyBoneIndexLowerBound = 1;
	const int UpperBodyBoneIndexUpperBound = 42;

	const int LowerBodyBoneIndexLowerBound = 43;
	const int LowerBodyBoneIndexUpperBound = 51;

	const float yBotMaxSpeed = 100.0f;
	const float yBotArriveDistance = 500.0f;
	const Vector3 yBotCameraLookAt{ 0.0f,150.0f,0.0f };
	const Vector3 yBotCameraPosOffset{ 0.0f,200.0f,0.0f };
	const float yBotCameraBackOffset = 200.0f;

	const float SightRange = 300.0f;
	const float HitRange = 100.0f;
	const float TouchRange = 150.0f;
	
	const int RootBoneIndex = 52;
	const int HipBoneIndex = 0;
	const int Spine3BoneIndex = 1;
	const int Spine2BoneIndex = 2;
	const int Spine1BoneIndex = 3;
	const int NeckBoneIndex = 4;
	const int HeadBoneIndex = 5;

	const int RightShoulderBoneIndex = 25;
	const int RightArmBoneIndex = 26;
	const int RightForeArmBoneIndex = 27;
	const int RightHandIndex = 28;
	const int LeftShoulderBoneIndex = 6;
	const int LeftArmBoneIndex = 7;
	const int LeftForeArmBoneIndex = 8;
	const int LeftHandIndex = 9;
	const int RightUpLegBoneIndex = 44;
	const int RightLegBoneIndex = 45;
	const int RightFootBoneIndex = 46;
	const int LeftUpLegBoneIndex = 48;
	const int LeftLegBoneIndex = 49;
	const int LeftFootBoneIndex = 50;

	

	const int totalController = 2;

	void InitializeAniamtion(FILE* file, AnimationSet& animationSet)
	{
		for (auto& animationFile : animationFiles)
		{
			file = fopen(animationFile, "r");
			AnimationSetIO::Load(animationSet, file);
			fclose(file);
		}
	}


	float minX = 0.0f, maxX = 0.0f, minY = 0.0f, maxY = 0.0f, minZ = 0.0f, maxZ = 0.0f, adjustX = 0.0f, adjustY = 0.0f, adjustZ = 0.0f;
	bool flipX = false, flipY = false, flipZ = false;
}

void YBot::Load(const char * idleAnimationFile, const NFGE::Graphics::Color & material0, const NFGE::Graphics::Color & material1)
{
	mSteeringModule = std::make_unique<SteeringModule3D>(*this);
	auto Arrive = mSteeringModule->AddBehavior<ArriveBehavior3D>("Arrive");
	Arrive->SetActive(true);
	Arrive->arriveDistance = yBotArriveDistance;
	maxSpeed = yBotMaxSpeed;
	destination = position;
	heading = { 0.0f,0.0f,1.0f };
	mLookDir = heading;

	mController_AI = std::make_unique<MovementController>(*this);
	mController_Physics = std::make_unique<MovementController_AllDirection>(*this);
	mControllers.reserve(2);
	mControllers.push_back(mController_AI.get());
	mControllers.push_back(mController_Physics.get());
	for (auto& controller : mControllers)
		controller->Load();

	//mModel.Load(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), modelFileName_0, shaderFileName, shaderFileName, SkeletonFileName);
	//mModel.mMaterials[0].ambient = material0;
	//mModel.mMaterials[1].ambient = material1;
	mModel2.Load(modelFileName_0, SkeletonFileName, &NFGE::sApp.GetMainLight());
	mModel2.mContext.AdjustRotation = Vector3(0.0f, 180.0f, 0.0f);
	mModel2.mContext.materials[0].ambient = material0;
	mModel2.mContext.materials[1].ambient = material1;


	FILE* file = fopen(idleAnimationFile, "r");
	AnimationSetIO::Load(mAnimmationSet, file);
	fclose(file);

	InitializeAniamtion(file, mAnimmationSet);

	//mAnimator.Load(mModel.mSkeleton.mSkeleton);
	mAnimator.Load(mModel2.mSkeleton);
	mAnimator.AddAnimationSet(mAnimmationSet);
	mAnimator.SetClip(0, true, 1.0f);
	mAnimator.SetClip(1, true, 1.0f);
	mAnimator.SetClip(2, true, 1.0f);
	mAnimator.SetClip(3, true, 1.0f);
	mAnimator.SetClip(4, true, 1.0f);
	mAnimator.SetClip(5, true, 1.0f);
	mAnimator.SetClip(6, false, 1.0f);
	mAnimator.SetClip(7, false, 1.0f);
	mAnimator.SetClip(8, false, 1.0f);
	mAnimator.SetClip(9, true, 1.0f);
	mAnimator.SetClip(10, true, 1.0f);
	mAnimator.SetClip(11, true, 1.0f);
	mAnimator.SetClip(12, true, 1.0f);
	mAnimator.SetClip(13, true, 1.0f);
	mAnimator.SetClip(14, true, 1.0f);
	mAnimator.SetClip(15, true, 1.0f);
	mAnimator.SetClip(16, true, 1.0f);
	mAnimator.SetClip(17, true, 1.0f);
	mAnimator.SetClip(18, true, 1.0f);
	mAnimator.SetClip(19, true, 1.0f);
	mAnimator.SetClip(20, true, 1.0f);
	mAnimator.SetClip(21, true, 1.0f);
	mAnimator.SetClip(22, true, 1.0f);
	mAnimator.SetClip(23, true, 1.0f);
	mAnimator.SetClip(24, true, 1.0f);
	mAnimator.SetClip(25, true, 1.0f);
	mAnimator.SetClip(26, true, 1.0f);
	mAnimator.SetClip(27, true, 1.0f);
	mAnimator.SetClip(28, true, 1.0f);
	mAnimator.Play(0);

	std::vector<int> UpperBodyBoneIndices;
	UpperBodyBoneIndices.reserve(UpperBodyBoneIndexUpperBound - UpperBodyBoneIndexLowerBound + 1);
	for (int i = UpperBodyBoneIndexLowerBound; i <= UpperBodyBoneIndexUpperBound; ++i)
		UpperBodyBoneIndices.emplace_back(i);
	mAnimator.AddPartial("UpperBody", UpperBodyBoneIndices);

	std::vector<int> LowerBodyBoneIndices;
	LowerBodyBoneIndices.reserve(LowerBodyBoneIndexUpperBound - LowerBodyBoneIndexLowerBound + 1);
	for (int i = LowerBodyBoneIndexLowerBound; i <= LowerBodyBoneIndexUpperBound; ++i)
		LowerBodyBoneIndices.emplace_back(i);
	LowerBodyBoneIndices.emplace_back(0); // Add Hip
	mAnimator.AddPartial("LowerBody", LowerBodyBoneIndices);

	mHeadRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.mSkeleton.bones[HeadBoneIndex].get());
	mHeadRig->mXMinRad = DEG2RAD(60.0f);
	mHeadRig->mXMaxRad = DEG2RAD(60.0f);
	mHeadRig->mYMinRad = DEG2RAD(90.0f);
	mHeadRig->mYMaxRad = DEG2RAD(90.0f);
	mHeadRig->mZMinRad = DEG2RAD(10.0f);
	mHeadRig->mZMaxRad = DEG2RAD(10.0f);
	mHeadRig->mRotationSpeed = 10.0f;
	mHeadRig->isNeedDirectionFlipX = true;
	mHeadRig->isFlipInSmallAngle_X = true;
	//mHeadRig->isNeedFlipX = true;
	//mHeadRig->tPosfrontVector = Vector3{ 0.0f,0.0f,1.0f };

	// Left arm grabing system
	//mLeftShoulderRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.mSkeleton.bones[LeftShoulderBoneIndex].get());
	//mLeftShoulderRig->mXMinRad = DEG2RAD(10.0f);
	//mLeftShoulderRig->mXMaxRad = DEG2RAD(10.0f);
	//mLeftShoulderRig->mYMinRad = DEG2RAD(10.0f);
	//mLeftShoulderRig->mYMaxRad = DEG2RAD(10.0f);
	//mLeftShoulderRig->mZMinRad = DEG2RAD(10.0f);
	//mLeftShoulderRig->mZMaxRad = DEG2RAD(10.0f);
	//mLeftShoulderRig->mRotationSpeed = 10.0f;
	//mLeftShoulderRig->ModelAdjust_Y = DEG2RAD(90);
	////mLeftShoulderRig->ModelAdjust_Z_2 = DEG2RAD(180);
	////mLeftShoulderRig->ModelAdjust_Z = DEG2RAD(90);
	//mLeftShoulderRig->isNeedFlipY = true;
	////mLeftShoulderRig->tPosfrontVector = Vector3{ -1.0f,0.0f,0.0f };
	////mLeftShoulderRig->adjustToFacingFrontRotation_X = DEG2RAD(0);
	////mLeftShoulderRig->, = DEG2RAD(-90);
	////mLeftShoulderRig->adjustToFacingFrontRotation_Z = DEG2RAD(180);

	mLeftUpArmRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.mSkeleton.bones[LeftArmBoneIndex].get());
	mLeftUpArmRig->mXMinRad = DEG2RAD(300.0f);
	mLeftUpArmRig->mXMaxRad = DEG2RAD(300.0f);
	mLeftUpArmRig->mYMinRad = DEG2RAD(360.0f);
	mLeftUpArmRig->mYMaxRad = DEG2RAD(360.0f);
	mLeftUpArmRig->mZMinRad = DEG2RAD(360.0f);
	mLeftUpArmRig->mZMaxRad = DEG2RAD(360.0f);
	mLeftUpArmRig->mRotationSpeed = 10.0f;
	mLeftUpArmRig->adjustToFacingFrontRotation_Y = DEG2RAD(-90);
	mLeftUpArmRig->isNeedFlipZ = true;


	mLeftLowerArmRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.mSkeleton.bones[LeftForeArmBoneIndex].get());
	mLeftLowerArmRig->mXMinRad = DEG2RAD(120.0f);
	mLeftLowerArmRig->mXMaxRad = DEG2RAD(120.0f);
	mLeftLowerArmRig->mYMinRad = DEG2RAD(120.0f);
	mLeftLowerArmRig->mYMaxRad = DEG2RAD(120.0f);
	mLeftLowerArmRig->mZMinRad = DEG2RAD(120.0f);
	mLeftLowerArmRig->mZMaxRad = DEG2RAD(120.0f);
	mLeftLowerArmRig->mRotationSpeed = 10.0f;
	mLeftLowerArmRig->adjustToFacingFrontRotation_Y = DEG2RAD(-90);
	mLeftLowerArmRig->isNeedFlipZ = true;

	mLeftHandRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.mSkeleton.bones[LeftHandIndex].get());
	mLeftHandRig->mXMinRad = DEG2RAD(60.0f);
	mLeftHandRig->mXMaxRad = DEG2RAD(60.0f);
	mLeftHandRig->mYMinRad = DEG2RAD(30.0f);
	mLeftHandRig->mYMaxRad = DEG2RAD(60.0f);
	mLeftHandRig->mZMinRad = DEG2RAD(30.0f);
	mLeftHandRig->mZMaxRad = DEG2RAD(30.0f);
	mLeftHandRig->mRotationSpeed = 10.0f;
	mLeftHandRig->adjustToFacingFrontRotation_Y = DEG2RAD(-90);
	mLeftHandRig->isNeedFlipZ = true;

	mRightUpArmRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.mSkeleton.bones[RightArmBoneIndex].get());
	mRightUpArmRig->mXMinRad = DEG2RAD(300.0f);
	mRightUpArmRig->mXMaxRad = DEG2RAD(300.0f);
	mRightUpArmRig->mYMinRad = DEG2RAD(360.0f);
	mRightUpArmRig->mYMaxRad = DEG2RAD(360.0f);
	mRightUpArmRig->mZMinRad = DEG2RAD(360.0f);
	mRightUpArmRig->mZMaxRad = DEG2RAD(360.0f);
	mRightUpArmRig->mRotationSpeed = 10.0f;
	mRightUpArmRig->adjustToFacingFrontRotation_Y = DEG2RAD(90);
	mRightUpArmRig->isNeedFlipZ = true;


	mRightLowerArmRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.mSkeleton.bones[RightForeArmBoneIndex].get());
	mRightLowerArmRig->mXMinRad = DEG2RAD(120.0f);
	mRightLowerArmRig->mXMaxRad = DEG2RAD(120.0f);
	mRightLowerArmRig->mYMinRad = DEG2RAD(120.0f);
	mRightLowerArmRig->mYMaxRad = DEG2RAD(120.0f);
	mRightLowerArmRig->mZMinRad = DEG2RAD(120.0f);
	mRightLowerArmRig->mZMaxRad = DEG2RAD(120.0f);
	mRightLowerArmRig->mRotationSpeed = 10.0f;
	mRightLowerArmRig->adjustToFacingFrontRotation_Y = DEG2RAD(90);
	mRightLowerArmRig->isNeedFlipZ = true;

	mRightHandRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.mSkeleton.bones[RightHandIndex].get());
	mRightHandRig->mXMinRad = DEG2RAD(60.0f);
	mRightHandRig->mXMaxRad = DEG2RAD(60.0f);
	mRightHandRig->mYMinRad = DEG2RAD(30.0f);
	mRightHandRig->mYMaxRad = DEG2RAD(60.0f);
	mRightHandRig->mZMinRad = DEG2RAD(30.0f);
	mRightHandRig->mZMaxRad = DEG2RAD(30.0f);
	mRightHandRig->mRotationSpeed = 10.0f;
	mRightHandRig->adjustToFacingFrontRotation_Y = DEG2RAD(90);
	mRightHandRig->isNeedFlipZ = true;

	mSightRange = SightRange;
	mHitRange = HitRange;
	mTouchRange = TouchRange;
}

void YBot::Load(const NFGE::Graphics::Color& material0, const NFGE::Graphics::Color& material1)
{
	Load(idleAnimation, material0, material1);
}

void YBot::Update(float deltaTime)
{
	UpdateSimplePhysics(deltaTime);

	mAnimator.Update(deltaTime);
	if (mRagBody.mIsActive)
	{
		mRagBody.SetSkeleton(position);
		mModel2.Update(deltaTime);
	}
	else
	{
		mModel2.Update(mAnimator, deltaTime);
	}

	// bone rigging ------------------------------------------------------------------------------------------------
	auto toWorldMatrix = mModel2.mContext.finalAdjustMatrix * mModel2.mContext.finalToWorld;

	mHeadRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.finalAdjustMatrix, mModel2.mContext.finalToWorld);
	mHeadRig->Update(deltaTime, mIsLooking, mLookTar);
	NFGE::Math::Matrix4 rigRotation = mHeadRig->GetTransform();
	mHeadRig->mBone->toParentTransform = rigRotation * mModel2.mContext.tPosToParentMatrix[mHeadRig->mBone->index] * mHeadRig->GetModelAdjustTransform();
	mModel2.mContext.UpdateTransform(mHeadRig->mBone->index);

	// Arm rig testing
	//mLeftShoulderRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, toWorldMatrix);
	//mLeftShoulderRig->Update(deltaTime, mIsLooking, mLookTar);
	//rigRotation = mLeftShoulderRig->GetTransform();
	//mLeftShoulderRig->mBone->toParentTransform = /*mLeftShoulderRig->GetModelAdjustTransform() **/ rigRotation * mModel2.mContext.tPosToParentMatrix[mLeftShoulderRig->mBone->index];
	//mModel2.mContext.UpdateTransform(mLeftShoulderRig->mBone->index);

	mLeftUpArmRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.finalAdjustMatrix, mModel2.mContext.finalToWorld);
	mLeftUpArmRig->Update(deltaTime, mIsLeftArmTargeting, mLeftUpArmTar);
	if (armRigResetTimer > 0.0f)
	{
		rigRotation = mLeftUpArmRig->GetTransform();
		mLeftUpArmRig->mBone->toParentTransform = rigRotation * mModel2.mContext.tPosToParentMatrix[mLeftUpArmRig->mBone->index];
		mModel2.mContext.UpdateTransform(mLeftUpArmRig->mBone->index);
	}
	

	mLeftLowerArmRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.finalAdjustMatrix, mModel2.mContext.finalToWorld);
	mLeftLowerArmRig->Update(deltaTime, mIsLeftArmTargeting, mLeftLowerArmTar);
	if (armRigResetTimer > 0.0f)
	{
		rigRotation = mLeftLowerArmRig->GetTransform();
		mLeftLowerArmRig->mBone->toParentTransform = rigRotation * mModel2.mContext.tPosToParentMatrix[mLeftLowerArmRig->mBone->index];
		mModel2.mContext.UpdateTransform(mLeftLowerArmRig->mBone->index);
	}

	mLeftHandRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.finalAdjustMatrix, mModel2.mContext.finalToWorld);
	mLeftHandRig->Update(deltaTime, mIsLeftArmTargeting, mLeftHandTar);
	if (armRigResetTimer > 0.0f)
	{
		rigRotation = mLeftHandRig->GetTransform();
		mLeftHandRig->mBone->toParentTransform = rigRotation * mModel2.mContext.tPosToParentMatrix[mLeftHandRig->mBone->index];
		mModel2.mContext.UpdateTransform(mLeftHandRig->mBone->index);
	}

	mRightUpArmRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.finalAdjustMatrix, mModel2.mContext.finalToWorld);
	mRightUpArmRig->Update(deltaTime, mIsRightArmTargeting, mRightUpArmTar);
	if (armRigResetTimer > 0.0f)
	{
		rigRotation = mRightUpArmRig->GetTransform();
		mRightUpArmRig->mBone->toParentTransform = rigRotation * mModel2.mContext.tPosToParentMatrix[mRightUpArmRig->mBone->index];
		mModel2.mContext.UpdateTransform(mRightUpArmRig->mBone->index);
	}
	 

	mRightLowerArmRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.finalAdjustMatrix, mModel2.mContext.finalToWorld);
	mRightLowerArmRig->Update(deltaTime, mIsRightArmTargeting, mRightLowerArmTar);
	if (armRigResetTimer > 0.0f)
	{
		rigRotation = mRightLowerArmRig->GetTransform();
		mRightLowerArmRig->mBone->toParentTransform = rigRotation * mModel2.mContext.tPosToParentMatrix[mRightLowerArmRig->mBone->index];
		mModel2.mContext.UpdateTransform(mRightLowerArmRig->mBone->index);
	}

	mRightHandRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, mModel2.mContext.finalAdjustMatrix, mModel2.mContext.finalToWorld);
	mRightHandRig->Update(deltaTime, mIsRightArmTargeting, mRightHandTar);
	if (armRigResetTimer > 0.0f)
	{
		rigRotation = mRightHandRig->GetTransform();
		mRightHandRig->mBone->toParentTransform = rigRotation * mModel2.mContext.tPosToParentMatrix[mRightHandRig->mBone->index];
		mModel2.mContext.UpdateTransform(mRightHandRig->mBone->index);
	}
	// bone rigging ------------------------------------------------------------------------------------------------
	

	mModel2.mContext.position = position;
	mModel2.mContext.currentFoward = heading;

	if (NFGE::Input::InputSystem::Get()->IsKeyPressed(NFGE::Input::KeyCode::M))
	{
		mCurrentUseController = (mCurrentUseController + 1) % mControllers.size();
	}

	armRigResetTimer -= deltaTime;
	if (mIsLeftArmTargeting || mIsRightArmTargeting)
	{
		armRigResetTimer = armRigResetTime;
	}
}

void YBot::Render(const Camera & camera)
{
	//
	
	if(!isDebugDraw)
	{
		mModel2.Render(camera);
	}
	else
	{
		//mModel2.DebugUI(camera);
	}
	
	NFGE::sApp.DrawSphere(mLookTar, 2.0f, NFGE::Graphics::Colors::Red);
}

void YBot::UnLoad()
{
	mModel2.UnLoad();
}

void YBot::DebugUI()
{
	
	ImGui::Begin("Head Rotation Constrains");
	auto _minX = RAD2DEG(mHeadRig->mXMinRad); auto _maxX = RAD2DEG(mHeadRig->mXMaxRad);
	auto _minY = RAD2DEG(mHeadRig->mYMinRad); auto _maxY = RAD2DEG(mHeadRig->mYMaxRad);
	auto _minZ = RAD2DEG(mHeadRig->mZMinRad); auto _maxZ = RAD2DEG(mHeadRig->mZMaxRad);
	ImGui::DragFloat("Min X##look", &_minX, 10.0f, 0.0f, 360.0f);
	ImGui::DragFloat("Max X##look", &_maxX, 10.0f, 0.0f, 360.0f);
	ImGui::DragFloat("Min Y##look", &_minY, 10.0f, 0.0f, 360.0f);
	ImGui::DragFloat("Max Y##look", &_maxY, 10.0f, 0.0f, 360.0f);
	ImGui::DragFloat("Min Z##look", &_minZ, 10.0f, 0.0f, 360.0f);
	ImGui::DragFloat("Max Z##look", &_maxZ, 10.0f, 0.0f, 360.0f);

	mHeadRig->mXMinRad = DEG2RAD(_minX); mHeadRig->mXMaxRad = DEG2RAD(_maxX);
	mHeadRig->mYMinRad = DEG2RAD(_minY); mHeadRig->mYMaxRad = DEG2RAD(_maxY);
	mHeadRig->mZMinRad = DEG2RAD(_minZ); mHeadRig->mZMaxRad = DEG2RAD(_maxZ);
	ImGui::End();


	ImGui::Begin("YBot");

	ImGui::DragFloat("Material_0 alpha##material", &mModel2.mContext.materials[0].ambient.w, 0.1f, 0.0f, 1.0f);
	mModel2.mContext.materials[0].SetAlpha(mModel2.mContext.materials[0].ambient.w);
	mModel2.mContext.materials[1].SetAlpha(mModel2.mContext.materials[0].ambient.w);
	ImGui::Text("Movening speed: %f", Magnitude(velocity));
	ImGui::DragFloat("Direction X##look", &mLookTar.x, 10.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat("Direction Y##look", &mLookTar.y, 10.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat("Direction Z##look", &mLookTar.z, 10.0f, -1000.0f, 1000.0f);
	if (ImGui::Button("Toggle look"))
	{
		mIsLooking = !mIsLooking;
	}

	//ImGui::DragFloat("Testing Rig minX##minX", &minX, 10.0f, 0.0f, 360.0f);
	//ImGui::DragFloat("Testing Rig maxX##maxX", &maxX, 10.0f, 0.0f, 360.0f);
	//ImGui::DragFloat("Testing Rig minY##minY", &minY, 10.0f, 0.0f, 360.0f);
	//ImGui::DragFloat("Testing Rig maxY##maxY", &maxY, 10.0f, 0.0f, 360.0f);
	//ImGui::DragFloat("Testing Rig minZ##minZ", &minZ, 10.0f, 0.0f, 360.0f);
	//ImGui::DragFloat("Testing Rig maxZ##maxZ", &maxZ, 10.0f, 0.0f, 360.0f);
	//ImGui::DragFloat("Testing Rig adjustX##adjustX", &adjustX, 10.0f, -360.0f, 360.0f);
	//ImGui::DragFloat("Testing Rig adjustY##adjustY", &adjustY, 10.0f, -360.0f, 360.0f);
	//ImGui::DragFloat("Testing Rig adjustZ##adjustZ", &adjustZ, 10.0f, -360.0f, 360.0f);
	
	//mLeftShoulderRig->mXMinRad = DEG2RAD(minX);
	//mLeftShoulderRig->mXMaxRad = DEG2RAD(maxX);
	//mLeftShoulderRig->mYMinRad = DEG2RAD(minY);
	//mLeftShoulderRig->mYMaxRad = DEG2RAD(maxY);
	//mLeftShoulderRig->mZMinRad = DEG2RAD(minZ);
	//mLeftShoulderRig->mZMaxRad = DEG2RAD(maxZ);
	//mLeftUpArmRig->ModelAdjust_X = DEG2RAD(adjustX);
	//mLeftUpArmRig->ModelAdjust_Y = DEG2RAD(adjustY);
	//mLeftUpArmRig->ModelAdjust_Z = DEG2RAD(adjustZ);
	ImGui::Text("zRot : %f", mLeftUpArmRig->mCurrentZRotation);
	if (ImGui::Button("DebugDraw"))
	{
		isDebugDraw = !isDebugDraw;
	}
	ImGui::LabelText("", "Press [M] to switch control mode. Current mode : [%d]", mCurrentUseController);

	if (mIsReadyHitting)
	{
		ImGui::LabelText("", "Press [H] to Hit !!");
	}
	if (isDebugDraw)
	{
		mHeadRig->DebugDraw();
		//mLeftShoulderRig->DebugDraw();
		mLeftUpArmRig->DebugDraw();
		if (mRagBody.mIsActive)
		{
			mRagBody.DebugDraw();
		}
	}
	

	if (ImGui::Button("RagDoll"))
	{
		//mPhysicsWorld.ClearDynamic();
		SpwanRagDoll();
	}

	if (ImGui::Button("Freeze"))
	{
		mPhysicsWorld.ClearForce();
		
	}
	if (mRagBody.mIsActive)
	{
		if (ImGui::Button("Boom"))
		{
			mRagBody.mHip.pCenter->SetVelocity(Vector3{ 0.0f,100.0f,0.0f });
		}
	}



	ImGui::End();


}

void YBot::HookCamera(Camera & camera, float deltaTime)
{
	Vector3 lookPoint = position + yBotCameraLookAt;
	camera.SetDirection(lookPoint - camera.GetPosition());
	auto cameraSupposePos = (position - (mLookDir * yBotCameraBackOffset)) + yBotCameraPosOffset;

	if (NFGE::Math::MagnitudeSqr(cameraSupposePos - camera.GetPosition()) > 1.0f)
		camera.SetPosition(camera.GetPosition() + ((cameraSupposePos - camera.GetPosition()) * 0.3f)* 10.0f * deltaTime);
}

void YBot::Looking(const std::list<NFGE::Math::Vector3>& targets)
{
	float clostestTargetDist = FLT_MAX;
	mIsLooking = false;
	for (auto& target : targets)
	{
		float distanceSqr = DistanceSqr(target, GetHeadRigBone().GetPosition());
		if (distanceSqr > mSightRange * mSightRange)
			continue;
		if (distanceSqr < clostestTargetDist && distanceSqr > 100.0f)
		{
			mIsLooking = true;
			mLookTar = target;
			clostestTargetDist = distanceSqr;
		}
	}
}

void YBot::SenceHitingTarget(const std::list<NFGE::Math::Vector3>& targets)
{
	float clostestTargetDist = FLT_MAX;
	mIsReadyHitting = false;
	for (auto& target : targets)
	{
		float distanceSqr = DistanceSqr(target, GetHeadRigBone().GetPosition());
		if (distanceSqr > mHitRange * mHitRange)
			continue;
		if (distanceSqr < clostestTargetDist && distanceSqr > 50.0f)
		{
			mIsReadyHitting = true;
			mLeftUpArmTar = target;
			mLeftLowerArmTar = target;
			mLeftHandTar = target;
			mRightUpArmTar = target;
			mRightLowerArmTar = target;
			mRightHandTar = target;
			clostestTargetDist = distanceSqr;
		}
	}
}

void YBot::Touching(const std::vector<NFGE::Math::Vector3>& targets)
{
	float clostestTargetDist = FLT_MAX;
	mIsLeftArmTargeting = false;
	mIsRightArmTargeting = false;

	for (auto& target : targets)
	{
		float distanceSqr = DistanceSqr(target, GetHeadRigBone().GetPosition());
		if (distanceSqr > mTouchRange * mTouchRange)
			continue;
		auto targetDirection = Normalize(target - GetHeadRigBone().GetPosition());
		if (Dot(Vector3{ targetDirection.x,0.0f, targetDirection.z }, heading) < 0.0f)
			continue;
		
		if (distanceSqr < clostestTargetDist && distanceSqr > 50.0f)
		{
			clostestTargetDist = distanceSqr;
			if (Cross(Vector3{ targetDirection.x,0.0f, targetDirection.z }, heading).y > 0.0f) // detecting is using left or right arm
			{
				mIsLeftArmTargeting = true;
				mLeftUpArmTar = target;
				mLeftLowerArmTar = target;
				mLeftHandTar = target;
			}
			else
			{
				mIsRightArmTargeting = true;
				mRightUpArmTar = target;
				mRightLowerArmTar = target;
				mRightHandTar = target;
			}
			
		}
	}
}

const NFGE::Physics::RigBone& YBot::GetHeadRigBone() const
{
	return *mHeadRig.get();
}

void YBot::SpwanRagDoll()
{
	mRagBody.BuildDynamicRigDoll(mModel2.mContext.finalAdjustMatrix, mModel2.mContext.finalToWorld);
}

void YBot::UpdateSimplePhysics(float deltaTime)
{
	auto force = mSteeringModule->Calculate();
	auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	// TODO add truncate method here
	if (MagnitudeSqr(velocity) > maxSpeed * maxSpeed)
		velocity = NFGE::Math::Normalize(velocity) * maxSpeed;

	position += velocity * deltaTime;

	if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
		heading = NFGE::Math::Normalize(velocity);
}
