#include "MovementController.h"
#include "YBot.h"

namespace
{
	const float sightTurnSpeed = 0.03f;
	const float walkTurnSpeed = 1.5f;
	const float runTurnSpeed = 1.0f;
	const float destinationDistance = 4000.0f;
	const float walkMaxSpeed = 150.0f;
	const float runMaxSpeed = 300.0f;
	const float slowDownSpeedPercentage = 5.5f;
	const float toIdelSpeed = 3.0f;

	enum State
	{
		Idle = 0,
		Walk,
		Run,
	};
	State state = Idle;
}

void MovementController::Load()
{
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	mMouseLockPos.x = static_cast<float>(screenWidth >> 1);
	mMouseLockPos.y = static_cast<float>(screenHeight >> 1);

	
}

void MovementController::Update(float deltaTime)
{
	auto inputSystem = NFGE::Input::InputSystem::Get();

	auto currentFoward = mYBot.heading;
	auto currentRight = NFGE::Math::Cross(Vector3::YAxis, currentFoward);
	auto currentUp = NFGE::Math::Cross(currentFoward, currentRight);

	auto& animator = mYBot.GetAnimator();
	if (inputSystem->IsKeyPressed(NFGE::Input::KeyCode::SPACE))
	{
		mPlayedAnimationIndex = (mPlayedAnimationIndex + 1) % animator.GetClipCount();
		animator.ChangeTo(mPlayedAnimationIndex, 0.5f, 0, NFGE::Math::Ease::EaseOutCirc);
	}

	float currentMoveMaxSpeed = walkMaxSpeed;
	float currentTurnSpeed = walkTurnSpeed;// = NFGE::Math::Magnitude(mYBot.velocity);
	
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::LSHIFT))
	{
		currentMoveMaxSpeed = 2000.0f;//runMaxSpeed;

		currentTurnSpeed = runTurnSpeed;
	}
	mYBot.maxSpeed += (currentMoveMaxSpeed - mYBot.maxSpeed) * slowDownSpeedPercentage * deltaTime;	// lerp slow down the max speed
	// Center the mouse

	// No key pressed
	if (NFGE::Math::Distance(mYBot.destination, mYBot.position) > 1.01f)
		mYBot.destination += ((mYBot.position - mYBot.destination) * slowDownSpeedPercentage) * deltaTime;

	// Movement
	auto currentSpeed = Magnitude(mYBot.velocity);
	Vector3 destinationDir;
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::W))
	{
		if (NFGE::Math::DistanceSqr(mYBot.destination, mYBot.position) < 1.0f)
			destinationDir = mYBot.heading;		// Get direction from heading when not move, because the destination maybe same as position
		else
			destinationDir = NFGE::Math::Normalize(mYBot.destination - mYBot.position); //TO check the difference for current heading to create tilt
		mYBot.destination = mYBot.position + destinationDir * destinationDistance;
		
		//if (currentSpeed <= walkMaxSpeed)
		//{
		//	float walkWeight = currentSpeed / walkMaxSpeed;
		//	mWalkClipsModifyer[0].mBlendWeight = 1.0f - walkWeight;
		//	mWalkClipsModifyer[1].mBlendWeight = walkWeight;
		//	mYBot.GetAnimator().MixClips(mWalkClipsModifyer);
		//}
		//else
		//{
		//	float runWeight = (currentSpeed - walkMaxSpeed ) / (runMaxSpeed - walkMaxSpeed);
		//	mRunClipsModifyer[0].mBlendWeight = 1.0f - runMaxSpeed;
		//	mRunClipsModifyer[1].mBlendWeight = runMaxSpeed;
		//	mYBot.GetAnimator().MixClips(mRunClipsModifyer);
		//}
		
	}
	if ((inputSystem->IsKeyDown(NFGE::Input::KeyCode::A))||(inputSystem->IsKeyDown(NFGE::Input::KeyCode::D)))
	{
		mIsPressTurn= true;
		float dir = (inputSystem->IsKeyDown(NFGE::Input::KeyCode::A)) ? -1.0f : 1.0f;
		dir = (inputSystem->IsKeyDown(NFGE::Input::KeyCode::D)) ? 1.0f : -1.0f;

		if (NFGE::Math::DistanceSqr(mYBot.destination, mYBot.position) < 1.0f)
			destinationDir = mYBot.heading;		// Get direction from heading when not move, because the destination maybe same as position
		else
			destinationDir =/* mYBot.heading * NFGE::Math::MatrixRotationAxis(Vector3{ 0.0f,1.0f,0.0f }, currentTurnSpeed * dir * deltaTime);*/NFGE::Math::Normalize(mYBot.destination - mYBot.position); //TO check the difference for current heading to create tilt
		mYBot.destination = mYBot.position + destinationDir * destinationDistance;
		Vector3 currentToDestinationVector = mYBot.destination - mYBot.position;
		Vector3 newToDestinationVec = currentToDestinationVector * NFGE::Math::MatrixRotationAxis(Vector3{0.0f,1.0f,0.0f}, currentTurnSpeed * dir * deltaTime);
		mYBot.destination = mYBot.position + newToDestinationVec;
		mYBot.velocity = NFGE::Math::Normalize(newToDestinationVec) * NFGE::Math::Magnitude( mYBot.velocity );		// Set velocity so that there is no slow down when turnning
	}
	else if(mIsPressTurn)
	{
		destinationDir = mYBot.heading;
		mYBot.destination = mYBot.position + destinationDir * destinationDistance;;
		mIsPressTurn = false;
	}

	if (inputSystem->IsMousePressed(NFGE::Input::MouseButton::LBUTTON))
	{
		mYBot.GetAnimator().ChangeTo(6, 0.5f, 1);
	}
	if (inputSystem->IsMousePressed(NFGE::Input::MouseButton::RBUTTON))
	{
		mYBot.GetAnimator().ChangeTo(7, 0.2f, 1);
	}

	if (inputSystem->IsMousePressed(NFGE::Input::MouseButton::MBUTTON))
	{
		mYBot.GetAnimator().ChangeTo(8, 0.2f, 2);
	}

	{// Look control
		tagPOINT p;
		GetCursorPos(&p);
		float currentMouseX = static_cast<float>(p.x);
		float currentMouseY = static_cast<float>(p.y);

		float mXRotationMagitude = (currentMouseY - mMouseLockPos.y) * sightTurnSpeed * deltaTime;
		float mYRotationMagitude = (currentMouseX - mMouseLockPos.x) * sightTurnSpeed * deltaTime;

		mYBot.mLookDir = mYBot.mLookDir * MatrixRotationQuaternion(QuaternionRotationAxis(Vector3::YAxis, mYRotationMagitude) * QuaternionRotationAxis(Cross(Vector3::YAxis, mYBot.heading), mXRotationMagitude));
		
		SetCursorPos(static_cast<int>(mMouseLockPos.x), static_cast<int>(mMouseLockPos.y));
	}
	
	auto currentSpeedSqr = MagnitudeSqr(mYBot.velocity);
	auto walkSpeedSqr = walkMaxSpeed * walkMaxSpeed;
	auto runSpeedSqr = runMaxSpeed * runMaxSpeed;
	auto toIdleSpeedSqr = toIdelSpeed * toIdelSpeed;
	

	if ((currentSpeedSqr > walkSpeedSqr + 100.0f) &&(currentSpeedSqr < runSpeedSqr) && (state != State::Run))
	{
		mYBot.GetAnimator().ChangeTo(State::Run, 0.5f);
		state = Run;
	}
	else if ((currentSpeedSqr <= walkSpeedSqr + 100.0f) && (currentSpeedSqr > toIdleSpeedSqr) && (state != State::Walk))
	{
		mYBot.GetAnimator().ChangeTo(State::Walk, 0.5f);
		state = Walk;
	}
	else if ((currentSpeedSqr <= toIdleSpeedSqr) && (state != State::Idle))
	{
		mYBot.GetAnimator().ChangeTo(0, 0.2f);
		state = Idle;
	}

	float walkSpeedAnimationMutiplyer = currentSpeedSqr / walkSpeedSqr;										//|
	walkSpeedAnimationMutiplyer = walkSpeedAnimationMutiplyer < 0.8f ? 0.8f : walkSpeedAnimationMutiplyer;	//|--- Truncting
	walkSpeedAnimationMutiplyer = walkSpeedAnimationMutiplyer > 1.0f ? 1.0f : walkSpeedAnimationMutiplyer;	//|

	float runSpeedAnimationMutiplyer = (currentSpeedSqr / runSpeedSqr) /10.0f ;								//|
	runSpeedAnimationMutiplyer = runSpeedAnimationMutiplyer < 0.8f ? 0.8f : runSpeedAnimationMutiplyer;		//|--- Truncting
	runSpeedAnimationMutiplyer = runSpeedAnimationMutiplyer > 5.0f ? 5.0f : runSpeedAnimationMutiplyer;		//|

	mYBot.GetAnimator().SetClip(State::Walk, true, walkSpeedAnimationMutiplyer);
	mYBot.GetAnimator().SetClip(State::Run, true, runSpeedAnimationMutiplyer);
	
}

void MovementController::Set()
{
	auto Arrive = mYBot.mSteeringModule->GetBehavior<NFGE::AI::ArriveBehavior3D>("Arrive");
	Arrive->SetActive(true);
	mYBot.destination = mYBot.position + mYBot.heading * 0.01f;

}

void MovementController::DebugUI()
{
	
}
