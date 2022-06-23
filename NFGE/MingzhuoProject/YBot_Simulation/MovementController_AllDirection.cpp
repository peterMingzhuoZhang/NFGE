#include "MovementController_AllDirection.h"
#include "YBot.h"

namespace
{
	const float COS_5 = 0.996;
	const float COS_40 = 0.766f;
	const float COS_45 = 0.707;
	const float sightTurnSpeed = 0.03f;
	const float walkTurnSpeed = 5.0f;
	const float runTurnSpeed = 10.0f;
	const float headingTurnSpeedModifyer = 1.0f;
	const float idleTurnSpeed = 1.00f;

	const float walkMaxSpeed = 150.0f;
	const float runMaxSpeed = 400.0f;

	const float walkAccelation = 100.0f;
	const float walkDecelation = 100.0f;
	const float runAccelation = 300.0f;
	const float runDecelation = 300.0f;

	const int IdleGun_Index				= 28;
	const int WalkForward_Index			= 10;
	const int WalkForwardRight_Index	= 11;
	const int WalkRight_Index			= 12;
	const int WalkBackwardRight_Index	= 13;
	const int WalkBackward_Index		= 14;
	const int WalkBackwardLeft_Index	= 15;
	const int WalkLeft_Index			= 16;
	const int WalkForwardLeft_Index		= 17;
	
	const int RunForward_Index			= 18;
	const int RunForwardRight_Index		= 19;
	const int RunRight_Index			= 20;
	const int RunBackwardRight_Index	= 21;
	const int RunBackward_Index			= 22;
	const int RunBackwardLeft_Index		= 23;
	const int RunLeft_Index				= 24;
	const int RunForwardLeft_Index		= 25;

	const int IdleTurnRight_Index		= 26;
	const int IdleTurnLeft_Index		= 27;

	ClipNode* idleClipNode;

	ClipNode* WalkForwardClipNode;
	ClipNode* WalkForwardRightClipNode;
	ClipNode* WalkRightClipNode;
	ClipNode* WalkBackwardRightClipNode;
	ClipNode* WalkBackwardClipNode;
	ClipNode* WalkBackwardLeftClipNode;
	ClipNode* WalkLeftClipNode;
	ClipNode* WalkForwardLeftClipNode;

	ClipNode* RunForwardClipNode;
	ClipNode* RunForwardRightClipNode;
	ClipNode* RunRightClipNode;
	ClipNode* RunBackwardRightClipNode;
	ClipNode* RunBackwardClipNode;
	ClipNode* RunBackwardLeftClipNode;
	ClipNode* RunLeftClipNode;
	ClipNode* RunForwardLeftClipNode;

	ClipNode* IdleTurnRightClipNode;
	ClipNode* IdleTurnLeftClipNode;


	struct Idle_to_Movment_BlendPolicy
	{
		void operator()(ClipModifyer& clipModifyer, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, const MovementController_AllDirection::YBOtBlendContext& context, float time)
		{
			ASSERT(false, "[Non_DefaultPolicy] should not calling this overload");
			
		}
		void operator()(const std::vector<ClipNode*>& mClipNodes, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, const MovementController_AllDirection::YBOtBlendContext& context, float time)
		{
			ASSERT(!mClipNodes.empty(), "[Non_DefaultPolicy] should not have no children");

			float moveWeight = NFGE::Math::Clamp(context.mCurrentSpeed / walkMaxSpeed, 0.0f, 1.0f);
			float idleWeight = (1.0f - moveWeight);
			moveWeight *= weight;
			idleWeight *= weight;
			mClipNodes[0]->Blending(activeClipNodePtr, idleWeight, time);
			mClipNodes[1]->Blending(activeClipNodePtr, moveWeight, time);
		}
	};

	struct Idle_BlendPolicy
	{
		const float idleToTurnBlendWeight = 0.01f;
		void operator()(ClipModifyer& clipModifyer, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, MovementController_AllDirection::YBOtBlendContext& context, float time)
		{
			ASSERT(false, "[Non_DefaultPolicy] should not calling this overload");

		}
		void operator()(const std::vector<ClipNode*>& mClipNodes, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, MovementController_AllDirection::YBOtBlendContext& context, float time)
		{
			ASSERT(!mClipNodes.empty(), "[Non_DefaultPolicy] should not have no children");

			//if (context.mCurrentSpeed > 1.0f)
			//{
			//	mClipNodes[0]->Blending(activeClipNodePtr, weight, time);
			//	return;
			//}

			if (context.IsNeedToTurn)
				context.IsTurning = true;

			float dot = NFGE::Math::Dot(NFGE::Math::Normalize(context.mPlayerHeading), NFGE::Math::Normalize(context.mTempTurnToDirection));
			float turnDot = context.mTurnDot;
			dot = Clamp(dot, -1.0f, 1.0f);		// This lane kills me
			Vector3 cross = NFGE::Math::Cross(context.mPlayerHeading, context.mTempTurnToDirection);

			float turnTimeWeight = NFGE::Math::Clamp((dot - turnDot)/ (1.0f - turnDot), 0.0f, 1.0f);
			float turnWeight;
			if (dot - turnDot < (1.0f - turnDot) * 0.5f)
			{
				turnWeight = NFGE::Math::Clamp((dot - turnDot) / (idleToTurnBlendWeight), 0.0f, 1.0f);
			}
			else
			{
				turnWeight = NFGE::Math::Clamp((1.0f - dot) / (idleToTurnBlendWeight), 0.0f, 1.0f);
			}

			float idleWeight = (1.0f - turnWeight);
			turnWeight *= weight;
			idleWeight *= weight;
			if (cross.y > 0.0f)
			{
				mClipNodes[0]->Blending(activeClipNodePtr, idleWeight, time);
				mClipNodes[1]->Blending(activeClipNodePtr, turnWeight, turnTimeWeight);
			}
			else
			{
				mClipNodes[0]->Blending(activeClipNodePtr, idleWeight, time);
				mClipNodes[2]->Blending(activeClipNodePtr, turnWeight, turnTimeWeight);
			}

			if (turnTimeWeight == 1.0f)
			{
				context.IsTurning = false;
				context.IsNeedToTurn = false;
			}
		}
	};

	struct Walk_to_Run_BlendPolicy
	{
		void operator()(ClipModifyer& clipModifyer, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, const MovementController_AllDirection::YBOtBlendContext& context, float time)
		{
			ASSERT(false, "[Non_DefaultPolicy] should not calling this overload");

		}
		void operator()(const std::vector<ClipNode*>& mClipNodes, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, const MovementController_AllDirection::YBOtBlendContext& context, float time)
		{
			ASSERT(!mClipNodes.empty(), "[Non_DefaultPolicy] should not have no children");

			float runWeight = NFGE::Math::Clamp((context.mCurrentSpeed - walkMaxSpeed) / (runMaxSpeed - walkMaxSpeed), 0.0f, 1.0f);
			float walkWeight = (1.0f - runWeight);
			runWeight *= weight;
			walkWeight *= weight;
			mClipNodes[0]->Blending(activeClipNodePtr, walkWeight, time);
			mClipNodes[1]->Blending(activeClipNodePtr, runWeight, time);
		}
	};

	struct MoveDirection_BlendPolicy
	{
		const float DotForward = 1.0f;
		const float DotForwardSide = 0.7071f;
		const float DotSide = 0.0f;
		const float DotBackwardSide = -0.7071f;
		const float DotBackward = -1.0f;


		void operator()(ClipModifyer& clipModifyer, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, const MovementController_AllDirection::YBOtBlendContext& context, float time)
		{
			ASSERT(false, "[Non_DefaultPolicy] should not calling this overload");

		}
		void operator()(const std::vector<ClipNode*>& mClipNodes, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, const MovementController_AllDirection::YBOtBlendContext& context, float time)
		{
			ASSERT(!mClipNodes.empty(), "[Non_DefaultPolicy] should not have no children");

			float dot = NFGE::Math::Dot(NFGE::Math::Normalize(context.mMovementDirection), NFGE::Math::Normalize(context.mPlayerHeading));
			dot = Clamp(dot, -1.0f, 1.0f);		// This lane kills me
			Vector3 cross = NFGE::Math::Cross(context.mMovementDirection, context.mPlayerHeading);

			if (cross.y < 0.0f) // MoveDirection is on the Left side
			{
				if (dot <= DotForward && dot >= DotForwardSide)
				{
					float direction2weight = ((dot - DotForward) / (DotForwardSide - DotForward));
					float direction1weight = (1.0f - direction2weight);
					direction1weight *= weight;
					direction2weight *= weight;
					mClipNodes[0]->Blending(activeClipNodePtr, direction1weight, time);
					mClipNodes[1]->Blending(activeClipNodePtr, direction2weight, time);
				}
				else if (dot <= DotForwardSide && dot > DotSide)
				{
					float direction2weight = ((dot - DotForwardSide) / (DotSide - DotForwardSide));
					float direction1weight = (1.0f - direction2weight);
					direction1weight *= weight;
					direction2weight *= weight;
					mClipNodes[1]->Blending(activeClipNodePtr, direction1weight, time);
					mClipNodes[2]->Blending(activeClipNodePtr, direction2weight, time);
				}
				else if (dot <= DotSide && dot > DotBackwardSide)
				{
					float direction2weight = ((dot - DotSide) / (DotBackwardSide - DotSide));
					float direction1weight = (1.0f - direction2weight);
					direction1weight *= weight;
					direction2weight *= weight;
					mClipNodes[2]->Blending(activeClipNodePtr, direction1weight, time);
					mClipNodes[3]->Blending(activeClipNodePtr, direction2weight, time);
				}
				else if (dot <= DotBackwardSide && dot >= DotBackward)
				{
					float direction2weight = ((dot - DotBackwardSide) / (DotBackward - DotBackwardSide));
					float direction1weight = (1.0f - direction2weight);
					direction1weight *= weight;
					direction2weight *= weight;
					mClipNodes[3]->Blending(activeClipNodePtr, direction1weight, time);
					mClipNodes[4]->Blending(activeClipNodePtr, direction2weight, time);
				}
			}
			else				// MoveDirection is on the Right side
			{
				if (dot <= DotForward && dot >= DotForwardSide)
				{
					float direction2weight = ((dot - DotForward) / (DotForwardSide - DotForward));
					float direction1weight = (1.0f - direction2weight);
					direction1weight *= weight;
					direction2weight *= weight;
					mClipNodes[0]->Blending(activeClipNodePtr, direction1weight, time);
					mClipNodes[7]->Blending(activeClipNodePtr, direction2weight, time);
				}
				else if (dot <= DotForwardSide && dot > DotSide)
				{
					float direction2weight = ((dot - DotForwardSide) / (DotSide - DotForwardSide));
					float direction1weight = (1.0f - direction2weight);
					direction1weight *= weight;
					direction2weight *= weight;
					mClipNodes[7]->Blending(activeClipNodePtr, direction1weight, time);
					mClipNodes[6]->Blending(activeClipNodePtr, direction2weight, time);
				}
				else if (dot <= DotSide && dot > DotBackwardSide)
				{
					float direction2weight = ((dot - DotSide) / (DotBackwardSide - DotSide));
					float direction1weight = (1.0f - direction2weight);
					direction1weight *= weight;
					direction2weight *= weight;
					mClipNodes[6]->Blending(activeClipNodePtr, direction1weight, time);
					mClipNodes[5]->Blending(activeClipNodePtr, direction2weight, time);
				}
				else if (dot <= DotBackwardSide && dot >= DotBackward)
				{
					float direction2weight = ((dot - DotBackwardSide) / (DotBackward - DotBackwardSide));
					float direction1weight = (1.0f - direction2weight);
					direction1weight *= weight;
					direction2weight *= weight;
					mClipNodes[5]->Blending(activeClipNodePtr, direction1weight, time);
					mClipNodes[4]->Blending(activeClipNodePtr, direction2weight, time);
				}
			}
		}
	};
}

MovementController_AllDirection::MovementController_AllDirection(YBot & ybot)
	: mYbot(ybot)
	, mBlendTree({})
{
	mBlendContext = &mBlendTree.GetBlendContext();
}



void MovementController_AllDirection::Load()
{
	ASSERT(walkMaxSpeed != 0.0f, "[MovementController_AllDirection] can not have walkMaxSpeed equals 0.0f.");
	ASSERT(runMaxSpeed != 0.0f, "[MovementController_AllDirection] can not have runMaxSpeed equals 0.0f.");

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	mMouseLockPos.x = static_cast<float>(screenWidth >> 1);
	mMouseLockPos.y = static_cast<float>(screenHeight >> 1);

	mBlendContext->mFacingDirection = mYbot.heading;
	mBlendContext->mAccelation = walkAccelation;
	mBlendContext->mDecelation = walkDecelation;

	// Create Default(leaf) clip node
	idleClipNode =  mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("Idle_clip", *mBlendContext, 0, true, IdleGun_Index));
	
	WalkForwardClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("WalkForward_clip", *mBlendContext, 0, true, WalkForward_Index));
	WalkForwardRightClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("WalkForwardRight_clip", *mBlendContext, 0, true, WalkForwardRight_Index));
	WalkRightClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("WalkRight_clip", *mBlendContext, 0, true, WalkRight_Index));
	WalkBackwardRightClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("WalkBackwardRight_clip", *mBlendContext, 0, true, WalkBackwardRight_Index));
	WalkBackwardClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("WalkBackward_clip", *mBlendContext, 0, true, WalkBackward_Index));
	WalkBackwardLeftClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("WalkBackwardLeft_clip", *mBlendContext, 0, true, WalkBackwardLeft_Index));
	WalkLeftClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("WalkLeft_clip", *mBlendContext, 0, true, WalkLeft_Index));
	WalkForwardLeftClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("WalkForwardLeft_clip", *mBlendContext, 0, true, WalkForwardLeft_Index));

	RunForwardClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("RunForward_clip", *mBlendContext, 0, true, RunForward_Index));
	RunForwardRightClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("RunForwardRight_clip", *mBlendContext, 0, true, RunForwardRight_Index));
	RunRightClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("RunRight_clip", *mBlendContext, 0, true, RunRight_Index));
	RunBackwardRightClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("RunBackwardRight_clip", *mBlendContext, 0, true, RunBackwardRight_Index));
	RunBackwardClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("RunBackward_clip", *mBlendContext, 0, true, RunBackward_Index));
	RunBackwardLeftClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("RunBackwardLeft_clip", *mBlendContext, 0, true, RunBackwardLeft_Index));
	RunLeftClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("RunLeft_clip", *mBlendContext, 0, true, RunLeft_Index));
	RunForwardLeftClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("RunForwardLeft_clip", *mBlendContext, 0, true, RunForwardLeft_Index));

	IdleTurnRightClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("IdleTurnRight_clip", *mBlendContext, 0, true, IdleTurnRight_Index, true));
	IdleTurnLeftClipNode = mBlendTree.AddBlendNode(new BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>("IdleTurnLeft_clip", *mBlendContext, 0, true, IdleTurnLeft_Index, true));

	// Create composed clip node
	ClipNode* walk_BlendNode = mBlendTree.AddBlendNode(new BlendNode<MoveDirection_BlendPolicy, YBOtBlendContext>("walk_blend", *mBlendContext, 8));
	ClipNode* run_BlendNode = mBlendTree.AddBlendNode(new BlendNode<MoveDirection_BlendPolicy, YBOtBlendContext>("run_blend", *mBlendContext, 8));
	ClipNode* walk_to_run_BlendNode = mBlendTree.AddBlendNode(new BlendNode<Walk_to_Run_BlendPolicy, YBOtBlendContext>("walk_to_run", *mBlendContext, 2));
	ClipNode* idle_BlendNode = mBlendTree.AddBlendNode(new BlendNode<Idle_BlendPolicy, YBOtBlendContext>("Idle_Blend", *mBlendContext, 3));
	ClipNode* idle_to_movement = mBlendTree.AddBlendNode(new BlendNode<Idle_to_Movment_BlendPolicy, YBOtBlendContext>("Idle_to_movement", *mBlendContext, 2));
	
	// Connect the nodes

	walk_BlendNode->SetChildNode(WalkForwardClipNode,		0);
	walk_BlendNode->SetChildNode(WalkForwardRightClipNode,	1);
	walk_BlendNode->SetChildNode(WalkRightClipNode,			2);
	walk_BlendNode->SetChildNode(WalkBackwardRightClipNode,	3);
	walk_BlendNode->SetChildNode(WalkBackwardClipNode,		4);
	walk_BlendNode->SetChildNode(WalkBackwardLeftClipNode,	5);
	walk_BlendNode->SetChildNode(WalkLeftClipNode,			6);
	walk_BlendNode->SetChildNode(WalkForwardLeftClipNode,	7);

	run_BlendNode->SetChildNode(RunForwardClipNode,			0);
	run_BlendNode->SetChildNode(RunForwardRightClipNode,	1);
	run_BlendNode->SetChildNode(RunRightClipNode,			2);
	run_BlendNode->SetChildNode(RunBackwardRightClipNode,	3);
	run_BlendNode->SetChildNode(RunBackwardClipNode,		4);
	run_BlendNode->SetChildNode(RunBackwardLeftClipNode,	5);
	run_BlendNode->SetChildNode(RunLeftClipNode,			6);
	run_BlendNode->SetChildNode(RunForwardLeftClipNode,		7);

	walk_to_run_BlendNode->SetChildNode(walk_BlendNode, 0);
	walk_to_run_BlendNode->SetChildNode(run_BlendNode, 1);

	idle_BlendNode->SetChildNode(idleClipNode, 0);
	idle_BlendNode->SetChildNode(IdleTurnRightClipNode, 1);
	idle_BlendNode->SetChildNode(IdleTurnLeftClipNode, 2);

	idle_to_movement->SetChildNode(idle_BlendNode, 0);
	idle_to_movement->SetChildNode(walk_to_run_BlendNode, 1);
	
	mBlendTree.SetRootNode(idle_to_movement);
}

void MovementController_AllDirection::Update(float deltaTime)
{
	auto inputSystem = NFGE::Input::InputSystem::Get();
	

	bool isMoving = false;

	float currentTurnSpeed = walkTurnSpeed;
	float currentMaxSpeed = walkMaxSpeed;
	mBlendContext->mAccelation = walkAccelation;

	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::LSHIFT))
	{
		currentTurnSpeed = runTurnSpeed;
		currentMaxSpeed = runMaxSpeed;
		mBlendContext->mAccelation = runAccelation;
	}
	//---------------------------------------------------------------------------------------------------------------//|
	auto currentForward = mBlendContext->mFacingDirection;															 //|
	currentForward.y = 0.0f;																						 //|
	currentForward = NFGE::Math::Normalize(currentForward) * currentTurnSpeed;										 //|
	auto currentRight = Vector3{ currentForward.z, 0.0f, -currentForward.x };										 //|
	auto currentBackward = Vector3{ -currentForward.x, 0.0f, -currentForward.z };									 //|
	auto currentLeft = Vector3{ -currentForward.z, 0.0f, currentForward.x };										 //|
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::W))															 //|
	{																												 //|
		mBlendContext->mMovementDirection += currentForward * deltaTime;											 //|
		isMoving = true;																							 //|
	}																												 //|
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::D))															 //|
	{																												 //|--- Set AccelationDirection with key pad
		mBlendContext->mMovementDirection += currentRight * deltaTime;												 //|
		isMoving = true;																							 //|
	}																												 //|
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::S))															 //|
	{																												 //|
		mBlendContext->mMovementDirection += currentBackward * deltaTime;											 //|
		isMoving = true;																							 //|
	}																												 //|
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::A))															 //|
	{																												 //|
		mBlendContext->mMovementDirection += currentLeft * deltaTime;												 //|
		isMoving = true;																							 //|
	}																												 //|
	mBlendContext->mMovementDirection = NFGE::Math::Normalize(mBlendContext->mMovementDirection);					 //|
	//---------------------------------------------------------------------------------------------------------------//|
	
	if (isMoving)
		mBlendContext->mCurrentSpeed = mBlendContext->mCurrentSpeed < currentMaxSpeed ? mBlendContext->mCurrentSpeed + mBlendContext->mAccelation * deltaTime : mBlendContext->mCurrentSpeed + (currentMaxSpeed - mBlendContext->mCurrentSpeed) * deltaTime;
	else
		mBlendContext->mCurrentSpeed = mBlendContext->mCurrentSpeed > 0.0f ? mBlendContext->mCurrentSpeed - mBlendContext->mDecelation * deltaTime : 0.0f;

	float turnSpeed;
	if (mBlendContext->mCurrentSpeed > 0.0f)
	{
		turnSpeed = Clamp((mBlendContext->mCurrentSpeed / runMaxSpeed) * headingTurnSpeedModifyer * deltaTime, 0.0f, 1.0f);
		mYbot.heading += (mBlendContext->mFacingDirection - mYbot.heading) * turnSpeed;
		mYbot.heading = NFGE::Math::Normalize(mYbot.heading);
		mBlendContext->mTempTurnToDirection += (mBlendContext->mFacingDirection - mBlendContext->mTempTurnToDirection) * turnSpeed;
		//mBlendContext->IsTurning = false;
		//mBlendContext->IsNeedToTurn = false;
	}
	else if(mBlendContext->IsTurning)
	{
		turnSpeed = Clamp(idleTurnSpeed * headingTurnSpeedModifyer * deltaTime, 0.0f, 1.0f);
		float turnDegree = cosf( NFGE::Math::Dot(mYbot.heading, mBlendContext->mTempTurnToDirection));
		mYbot.heading += NFGE::Math::Normalize(mBlendContext->mTempTurnToDirection - mYbot.heading) * (turnDegree) * turnSpeed;
		mYbot.heading = NFGE::Math::Normalize(mYbot.heading);
	}
	
	float currentHeadToFacingDot = NFGE::Math::Dot(mYbot.heading, mBlendContext->mFacingDirection);
	if (!mBlendContext->IsNeedToTurn && mIsMouseNotMovingDirection && currentHeadToFacingDot < COS_45)
	{
		mBlendContext->IsNeedToTurn = true;
		mBlendContext->mTempTurnToDirection = mBlendContext->mFacingDirection;
		mBlendContext->mTurnDot = currentHeadToFacingDot;
	}

	if (mBlendContext->mCurrentSpeed <= 0.0f)
	{
		mBlendContext->mMovementDirection = mYbot.heading;
	}

	mBlendContext->mPlayerHeading = mYbot.heading;

	mYbot.position += mBlendContext->mMovementDirection * (mBlendContext->mCurrentSpeed * deltaTime);
	
	if (inputSystem->IsMouseDown(NFGE::Input::MouseButton::RBUTTON)) 
	{// Look control
		tagPOINT p;
		GetCursorPos(&p);
		float currentMouseX = static_cast<float>(p.x);
		float currentMouseY = static_cast<float>(p.y);
	
		float mXRotationMagitude = (currentMouseY - mMouseLockPos.y) * sightTurnSpeed * deltaTime;
		float mYRotationMagitude = (currentMouseX - mMouseLockPos.x) * sightTurnSpeed * deltaTime;
	
		mIsMouseNotMovingDirection = (mXRotationMagitude == 0.0f && mYRotationMagitude == 0.0f);

		mYbot.mLookDir = mYbot.mLookDir * MatrixRotationQuaternion(QuaternionRotationAxis(Vector3::YAxis, mYRotationMagitude) * QuaternionRotationAxis(Cross(Vector3::YAxis, mYbot.heading), mXRotationMagitude));
	
		SetCursorPos(static_cast<int>(mMouseLockPos.x), static_cast<int>(mMouseLockPos.y));
		
		mBlendContext->mFacingDirection = NFGE::Math::Normalize(NFGE::Math::Vector3(mYbot.mLookDir.x, 0.0f, mYbot.mLookDir.z));
	}
	else
	{
		mIsMouseNotMovingDirection = true;
	}
}

void MovementController_AllDirection::Set()
{
	mBlendContext->mFacingDirection = mYbot.heading;
	mYbot.GetAnimator().ChangeTo(IdleGun_Index, 1);
	mYbot.GetAnimator().BindBlendTree(&mBlendTree,0);

	auto Arrive = mYbot.mSteeringModule->GetBehavior<NFGE::AI::ArriveBehavior3D>("Arrive");
	Arrive->SetActive(false);
}

void MovementController_AllDirection::DebugUI()
{
	//ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
	//ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	//ImGui::Text("Progress Bar");
	ImGui::Text("Idle Clip");
	ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)idleClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Turn Right Clip"); ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)IdleTurnRightClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Turn Left Clip"); ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)IdleTurnLeftClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));

	ImGui::Text("Walk Forward Clip");		ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)WalkForwardClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Walk Forward Right Clip");	ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)WalkForwardRightClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Walk Right Clip");			ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)WalkRightClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Walk Backward Right Clip");ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)WalkBackwardRightClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Walk Backward Clip");		ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)WalkBackwardClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Walk Backward Left Clip");	ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)WalkBackwardLeftClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Walk Left Clip");			ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)WalkLeftClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Walk Forward Left Clip");	ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)WalkForwardLeftClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	
	ImGui::Text("Run Forward Clip");		ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)RunForwardClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Run Forward Right Clip");	ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)RunForwardRightClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Run Right Clip");			ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)RunRightClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Run Backward Right Clip"); ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)RunBackwardRightClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Run Backward Clip");		ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)RunBackwardClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Run Backward Left Clip");	ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)RunBackwardLeftClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Run Left Clip");			ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)RunLeftClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));
	ImGui::Text("Run Forward Left Clip");	ImGui::ProgressBar(((BlendNode<DefaultBlendPolicy<YBOtBlendContext>, YBOtBlendContext>*)RunForwardLeftClipNode)->mClipModifyer.mWeight, ImVec2(0.0f, 0.0f));

	//ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
	//ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	//ImGui::Text("Progress Bar");
}
