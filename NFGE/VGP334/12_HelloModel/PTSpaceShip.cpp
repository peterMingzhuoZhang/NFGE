#include "PTSpaceShip.h"

namespace 
{
	const char* modelFileName_0 = "../../Assets/Model/Spaceship.NFModelRV";
	const wchar_t* shaderFileName = L"../../Assets/Shaders/Standard_Model_ZMZ.fx";
	const float ForwardForce = 100.0f;
	const float UpForce = 30.0f;
	const float TurnSpeed = 30.0f;

	void Yaw(float deg, Vector3& direction)
	{
		Matrix4 rotation{};
		rotation.Identity();
		direction = Math::Normalize(direction * rotation.RotationY(deg));
	}
	void Pitch(float deg, Vector3& direction)
	{
		Matrix4 rotationToAngle{};
		rotationToAngle.Identity();
		Matrix4 rotationToAxis{};
		rotationToAxis.Identity();

		Vector3 axis = Cross(Vector3::YAxis, direction);
		Math::Normalize(axis);
		direction = Math::Normalize(direction * rotationToAxis.Rotation(axis, deg));
	}

	const uint8_t ControlState_Idle = 0X1 << 0;
	const uint8_t ControlState_PushFoward = 0X1 << 1;
	const uint8_t ControlState_SlowDown = 0X1 << 2;
	const uint8_t ControlState_PushUp = 0X1 << 3;
	const uint8_t ControlState_PushDown = 0X1 << 4;
	const uint8_t ControlState_XRotation = 0X1 << 5;
	const uint8_t ControlState_YRotation = 0X1 << 6;
	const uint8_t ControlState_ZRotation = 0X1 << 7;
}


void PTSpaceShip::Load(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	PTModel::Load(position, rotation, scale, modelFileName_0, shaderFileName, shaderFileName);
	Agent3D::position = PTModel::mContext.position;
	mSteeringModule = std::make_unique<AI::SteeringModule3D>(*this);
	auto Arrive = mSteeringModule->AddBehavior<AI::ArriveBehavior3D>("Arrive");
	Arrive->SetActive(true);
	Arrive->arriveDistance = 100.0f;;
	maxSpeed = 20.0f;

	// - Test ------------------
	mAnimationBuilder.AddScaleKey(Vector3{ 1.0f,1.0f,1.0f }, 0.0f)
		.AddScaleKey(Vector3{ 1.0f,1.0f,5.0f }, 1.0f)
		.AddScaleKey(Vector3{ 1.0f,1.0f,1.0f }, 2.0f);
	mJumpAnimationPtr = mAnimationBuilder.Build();
}

void PTSpaceShip::Update(float deltaTime)
{
	auto Arrive = mSteeringModule->GetBehavior<AI::ArriveBehavior3D>("Arrive");
	auto inputSystem = NFGE::Input::InputSystem::Get();

	Control(deltaTime);
	ShipAction(deltaTime);

	UpdateSimplePhysics(deltaTime);	
	HookCamera(NFGE::sApp.GetMainCamera(), deltaTime);

	if (mAnimationActive)
		mAnimationTimer += deltaTime;
	if (mAnimationTimer > mJumpAnimationPtr->GetLastFrameTime())
		mAnimationActive = false;
	
}

void PTSpaceShip::Render(Camera& camera)
{
	PTModel::mContext.position = position;
	PTModel::mContext.currentFoward = heading;
	PTModel::Render(camera);
}

void PTSpaceShip::DebugUI()
{
	auto currentFoward = heading;
	auto currentRight = NFGE::Math::Normalize (NFGE::Math::Cross(Vector3::YAxis, heading));
	auto currentUp = NFGE::Math::Normalize(NFGE::Math::Cross(heading, currentRight));

	NFGE::sApp.DrawLine(position, position + heading * 20.0f, NFGE::Graphics::Colors::Red);
	NFGE::sApp.DrawSphere(position + heading * 20.0f, 2.0f, NFGE::Graphics::Colors::Red);
	NFGE::sApp.DrawLine(position, position + currentUp * 20.0f, NFGE::Graphics::Colors::Green);
	NFGE::sApp.DrawSphere(position + currentUp * 20.0f, 2.0f, NFGE::Graphics::Colors::Green);
	NFGE::sApp.DrawLine(position, position + currentRight * 20.0f, NFGE::Graphics::Colors::Cyan);
	NFGE::sApp.DrawSphere(position + currentRight * 20.0f, 2.0f, NFGE::Graphics::Colors::Cyan);
}

void PTSpaceShip::UnLoad()
{
	// - Test ------------------
	mJumpAnimationPtr.release();
	mJumpAnimationPtr.reset();
	PTModel::Unload();
	// - Test ------------------
}

void PTSpaceShip::HookCamera(Camera & camera, float deltaTime)
{
	camera.SetDirection(position - camera.GetPosition());
	auto cameraSupposePos = (position - (heading * 30.0f)) + Vector3(0.0f,10.0f,0.0f);
	
	if(NFGE::Math::Magnitude(cameraSupposePos - camera.GetPosition()) > 0.001f)
		camera.SetPosition(camera.GetPosition() + ((cameraSupposePos - camera.GetPosition()) * 0.3f)* 10.0f * deltaTime);
}

void PTSpaceShip::Control(float deltaTime)
{
	mControlState &= ControlState_Idle;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mControlState |= ControlState_XRotation;
		mXRotationMagitude = inputSystem->GetMouseMoveY() * TurnSpeed * 0.01f * deltaTime;
		mControlState |= ControlState_YRotation;
		mYRotationMagitude = inputSystem->GetMouseMoveX() * TurnSpeed * 0.01f * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mControlState |= ControlState_PushFoward;
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mControlState |= ControlState_SlowDown;
	}
	if (inputSystem->IsKeyDown(KeyCode::SPACE))
	{
		mControlState |= ControlState_PushUp;
	}
	if (inputSystem->IsKeyDown(KeyCode::LCONTROL))
	{
		mControlState |= ControlState_PushDown;
	}
	if (inputSystem->IsKeyDown(KeyCode::E))
	{
		mControlState |= ControlState_ZRotation;
		mDestinationZRotation += TurnSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::Q))
	{
		mControlState |= ControlState_ZRotation;
		mDestinationZRotation -= TurnSpeed * deltaTime;
	}

	// - Test ------------------
	if (inputSystem->IsKeyDown(KeyCode::J))
	{
		mAnimationActive = true;
		mAnimationTimer = 0.0f;
	}
	// - Test ------------------
}

void PTSpaceShip::ShipAction(float deltaTime)
{
	auto currentFoward = heading;
	auto currentRight = NFGE::Math::Cross(Vector3::YAxis, heading);
	auto currentUp = NFGE::Math::Cross(heading, currentRight);

	if (mControlState & ControlState_Idle)
	{
		if (NFGE::Math::Distance(destination, position) > 0.01f)
			destination += ((position - destination) * 0.3f) * deltaTime;
	}
	if (mControlState & ControlState_PushFoward)
	{
		Vector3 destinationDir;
		if (NFGE::Math::Distance(destination, position) < 1.0f)
			destinationDir = heading;
		else
			destinationDir = NFGE::Math::Normalize(destination - position);
		destination = position + destinationDir * ForwardForce;
	}
	if (mControlState & ControlState_SlowDown)
	{
		if (NFGE::Math::Distance(destination, position) > 0.01f)
			destination += ((position - destination) * 1.8f) * deltaTime;
	}
	if (mControlState & ControlState_PushUp)
	{
		Vector3 UpVec = currentUp * UpForce * deltaTime;
		position += UpVec;
		destination += UpVec;
	}
	if (mControlState & ControlState_PushDown)
	{
		Vector3 DownVec = NFGE::Math::Cross(heading, NFGE::Math::Cross(Vector3::YAxis, heading)) * -UpForce * deltaTime;
		position += DownVec;
		destination += DownVec;
	}
	if (mControlState & ControlState_XRotation)
	{
		if (NFGE::Math::IsZero(velocity))
			destination = position + heading * 10.0f;
		Vector3 currentToDestinationVector = destination - position;
		Vector3 newToDestinationVec = currentToDestinationVector * NFGE::Math::MatrixRotationAxis(currentRight, mXRotationMagitude);
		destination = position + newToDestinationVec;
	}
	if (mControlState & ControlState_YRotation)
	{
		if (NFGE::Math::IsZero(velocity))
			destination = position + heading * 10.0f;
		Vector3 currentToDestinationVector = destination - position;
		Vector3 newToDestinationVec = currentToDestinationVector * NFGE::Math::MatrixRotationAxis(currentUp, mYRotationMagitude);
		destination = position + newToDestinationVec;
	}
	if (mControlState & ControlState_ZRotation)
	{
		//Do the left or right Emmision base one the z rotation
	}

	// terpolation
	if (NFGE::Math::Abs(mDestinationZRotation - mContext.AdjustRotation.z) > 0.01f)
	{
		mContext.AdjustRotation.z += (mDestinationZRotation - mContext.AdjustRotation.z) * 0.3f * deltaTime;
	}
}

void PTSpaceShip::UpdateSimplePhysics(float deltaTime)
{
	auto force = mSteeringModule->Calculate();
	auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	// TODO add truncate method here
	position += velocity * deltaTime;

	if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
		heading = NFGE::Math::Normalize(velocity);
}

void PTSpaceShip::OnRender(const Camera & camera)
{
	 //mJumpAnimationPtr->GetAllTransform(mAnimationTimer);
	 mContext.scale = mJumpAnimationPtr->GetValue_OnKeyframes(mJumpAnimationPtr->mScaleChannel.mKeyFrames, mAnimationTimer);
}
