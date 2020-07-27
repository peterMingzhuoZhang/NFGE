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
	const int HeadBoneIndex = 5;

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

	mHeadRig = std::make_unique<NFGE::Physics::RigBone>(mPhysicsWorld, mModel2.mContext.boneMatrix, mModel2.mContext.mSkeleton.bones[HeadBoneIndex].get());
	mHeadRig->mXMinRad = DEG2RAD(60.0f);
	mHeadRig->mXMaxRad = DEG2RAD(60.0f);
	mHeadRig->mYMinRad = DEG2RAD(60.0f);
	mHeadRig->mYMaxRad = DEG2RAD(60.0f);
	mHeadRig->mZMinRad = DEG2RAD(30.0f);
	mHeadRig->mZMaxRad = DEG2RAD(30.0f);
	mHeadRig->mRotationSpeed = 10.0f;

	mSightRange = SightRange;

}

void YBot::Load(const NFGE::Graphics::Color& material0, const NFGE::Graphics::Color& material1)
{
	Load(idleAnimation, material0, material1);
}

void YBot::Update(float deltaTime)
{
	UpdateSimplePhysics(deltaTime);

	mAnimator.Update(deltaTime);
	mModel2.Update(mAnimator, deltaTime);

	//Rig testing--------------
	//auto toWorldMatrix =	mModel.mAdjustRotMat * 
	//						NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(mModel.currentFoward, Vector3::YAxis)) *
	//						NFGE::Math::Matrix4::sScaling(mModel.mScale) *
	//						NFGE::Math::Matrix4::sTranslation(mModel.mPosition);
	auto toWorldMatrix = mModel2.mContext.finalAdjustMatrix * mModel2.mContext.finalToWorld;

	mHeadRig->Binding(mModel2.mContext.boneMatrix, mModel2.mContext.tPosToParentMatrix, toWorldMatrix);
	mHeadRig->Update(deltaTime, mIsLooking, mLookTar);
	//mHeadRig->LookTo(mLookTar);

	NFGE::Math::Matrix4 rigRotation = mHeadRig->GetRotationTransform();

	mHeadRig->mBone->toParentTransform = rigRotation * mModel2.mContext.tPosToParentMatrix[mHeadRig->mBone->index];
	mModel2.mContext.UpdateTransform(mHeadRig->mBone->index);

	mHeadRig->DebugDraw();
	//Rig testing--------------

	mModel2.mContext.position = position;
	mModel2.mContext.currentFoward = heading;

	if (NFGE::Input::InputSystem::Get()->IsKeyPressed(NFGE::Input::KeyCode::M))
	{
		mCurrentUseController = (mCurrentUseController + 1) % mControllers.size();
	}
}

void YBot::Render(const Camera & camera)
{
	//mModel2.DebugUI(NFGE::sApp.GetMainCamera());
	mModel2.Render(camera);
	NFGE::sApp.DrawSphere(mLookTar, 2.0f, NFGE::Graphics::Colors::Red);
}

void YBot::UnLoad()
{
	mModel2.UnLoad();
}

void YBot::DebugUI()
{

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
	ImGui::LabelText("", "Press [M] to switch control mode. Current mode : [%d]", mCurrentUseController);
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
		float distanceSqr = DistanceSqr(target, mHeadRig->GetPosition());
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
