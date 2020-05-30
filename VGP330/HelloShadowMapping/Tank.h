#pragma once
#pragma once
#include <NFGE/Inc/NFGE.h>

using namespace NFGE;
using namespace NFGE::Input;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

class Tank : public AI::Agent3D
{
	using ShadowConstantBuffer = TypedConstantBuffer<Matrix4>;
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
	


public:
	Tank(AI::AIWorld3D & world)
		:Agent3D(world)
	{

	}

	void Load(Vector3 position)
	{
		const char* textureDiffuseName = "../../Assets/Model/Original/Tank/tank_diffuse.jpg";
		const char* textureSpecularName = "../../Assets/Model/Original/Tank/tank_specular.jpg";
		const char* textureNormalName = "../../Assets/Model/Original/Tank/tank_normal.jpg";
		const char* textureAOName = "../../Assets/Model/Original/Tank/tank_ao.jpg";

		const wchar_t* vertexShader = L"../../Assets/Shaders/Standard.fx";
		const wchar_t* pixelShader = L"../../Assets/Shaders/Standard.fx";

		ObjLoader::Load("../../Assets/Model/Original/Tank/tank.obj", 0.001f, mesh);
		mMeshBuffer.Initialize(mesh);

		mMatrices[0].Identity();
		mMatrices[1].Identity();
		mMatrices[2].Identity();
		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();
		mVertexShader.Initialize(vertexShader, Vertex::Format);
		mPixelShader.Initialize(pixelShader);

		mTransformBuffer.Initialize();
		mLightBuffer.Initialize();
		mMaterialBuffer.Initialize();
		mOptionDateBuffer.Initialize();
		mShadowConstantBuffer.Initialize();

		mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,1.0f });
		mLight.ambient = Colors::Gray;
		mLight.diffuse = Colors::White;
		mLight.specular = Colors::White;

		mMaterial.ambient = Colors::White;
		mMaterial.diffuse = Colors::White;
		mMaterial.specular = Colors::White;
		mMaterial.power = 10.0f;

		mDiffuseTexture = TextureManager::Get()->LoadTexture(textureDiffuseName);
		mSpecularTexture = TextureManager::Get()->LoadTexture(textureSpecularName);
		mNormalTexture = TextureManager::Get()->LoadTexture(textureNormalName);
		mAOTexture = TextureManager::Get()->LoadTexture(textureAOName);

		// Movement
		Agent3D::position = position;
		mSteeringModule = std::make_unique<AI::SteeringModule3D>(*this);
		auto Arrive = mSteeringModule->AddBehavior<AI::ArriveBehavior3D>("Arrive");
		Arrive->SetActive(true);
		Arrive->arriveDistance = 100.0f;;
		maxSpeed = 20.0f;

		destination = position + Vector3(0.0f, 0.0f, 1.0f);

		mLight = NFGE::sApp.GetMainLight();
	}
	void Update(float deltaTime, Camera* currentCamera)
	{
		Control(deltaTime);
		ShipAction(deltaTime);

		UpdateSimplePhysics(deltaTime);
		HookCamera(*currentCamera, deltaTime);
	}

	void SubUpdate(float deltaTime, Camera* currentCamera)
	{
		Control(deltaTime);
		ShipAction(deltaTime);

		UpdateSimplePhysics(deltaTime);
		//HookCamera(*currentCamera, deltaTime);
	}
	void Render(NFGE::Graphics::Camera& camera, NFGE::Graphics::Camera& lightCamera,  NFGE::Graphics::RenderTarget& depthMap)
	{
		mVertexShader.Bind();
		mPixelShader.Bind();

		TextureManager::Get()->GetTexture(mDiffuseTexture)->BindPS();
		TextureManager::Get()->GetTexture(mSpecularTexture)->BindPS(1);
		//TextureManager::Get()->GetTexture(mDisplacementTexture)->BindVS(2);
		TextureManager::Get()->GetTexture(mNormalTexture)->BindPS(3);
		TextureManager::Get()->GetTexture(mAOTexture)->BindPS(4);
		SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		mMatrices[0].Identity();
		Matrix4 rotX, rotY, rotZ;
		rotX.Identity();
		rotY.Identity();
		rotZ.Identity();

		(rotX.RotationX(mRotation.x));
		(rotY.RotationY(mRotation.y));
		(rotZ.RotationZ(mRotation.z));
		auto headRotation = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(heading, Vector3::YAxis));
		mMatrices[0] = ( rotX *rotY * rotZ * headRotation * mMatrices[0].Translation(Agent3D::position.x, Agent3D::position.y, Agent3D::position.z));

		mMatrices[1] = (camera.GetViewMatrix());
		mMatrices[2] = (camera.GetPerspectiveMatrix());

		TransformData transformData;
		transformData.world = Transpose(mMatrices[0]);
		transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
		transformData.viewPositon = camera.GetPosition();
		mTransformBuffer.Load(transformData);
		mTransformBuffer.BindVS(0);
		mTransformBuffer.BindPS(0);

		mLightBuffer.Load(mLight);
		mLightBuffer.BindVS(1);
		mLightBuffer.BindPS(1);

		mMaterialBuffer.Load(mMaterial);
		mMaterialBuffer.BindVS(2);
		mMaterialBuffer.BindPS(2);

		OptionsData optData;
		optData.displacementWeight = mBumpWeight;
		optData.brightness = mBrightness;
		optData.useShadow = 1;
		mOptionDateBuffer.Load(optData);
		mOptionDateBuffer.BindVS(3);
		mOptionDateBuffer.BindPS(3);


		auto wvpLight = Transpose(mMatrices[0] * lightCamera.GetViewMatrix() * lightCamera.GetPerspectiveMatrix());
		mShadowConstantBuffer.Load(wvpLight);
		mShadowConstantBuffer.BindVS(4);

		depthMap.BindPS(5);

		mMeshBuffer.Render();
	}

	void Render(NFGE::Graphics::Camera& camera, NFGE::Math::Matrix4& viewMat, NFGE::Math::Matrix4& projMat, NFGE::Graphics::RenderTarget& depthMap)
	{
		mVertexShader.Bind();
		mPixelShader.Bind();

		TextureManager::Get()->GetTexture(mDiffuseTexture)->BindPS();
		TextureManager::Get()->GetTexture(mSpecularTexture)->BindPS(1);
		//TextureManager::Get()->GetTexture(mDisplacementTexture)->BindVS(2);
		TextureManager::Get()->GetTexture(mNormalTexture)->BindPS(3);
		TextureManager::Get()->GetTexture(mAOTexture)->BindPS(4);
		SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		mMatrices[0].Identity();
		Matrix4 rotX, rotY, rotZ;
		rotX.Identity();
		rotY.Identity();
		rotZ.Identity();

		(rotX.RotationX(mRotation.x));
		(rotY.RotationY(mRotation.y));
		(rotZ.RotationZ(mRotation.z));
		auto headRotation = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(heading, Vector3::YAxis));
		mMatrices[0] = (rotX *rotY * rotZ * headRotation * mMatrices[0].Translation(Agent3D::position.x, Agent3D::position.y, Agent3D::position.z));

		mMatrices[1] = (camera.GetViewMatrix());
		mMatrices[2] = (camera.GetPerspectiveMatrix());

		TransformData transformData;
		transformData.world = Transpose(mMatrices[0]);
		transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
		transformData.viewPositon = camera.GetPosition();
		mTransformBuffer.Load(transformData);
		mTransformBuffer.BindVS(0);
		mTransformBuffer.BindPS(0);

		mLightBuffer.Load(mLight);
		mLightBuffer.BindVS(1);
		mLightBuffer.BindPS(1);

		mMaterialBuffer.Load(mMaterial);
		mMaterialBuffer.BindVS(2);
		mMaterialBuffer.BindPS(2);

		OptionsData optData;
		optData.displacementWeight = mBumpWeight;
		optData.brightness = mBrightness;
		optData.useShadow = 1;
		mOptionDateBuffer.Load(optData);
		mOptionDateBuffer.BindVS(3);
		mOptionDateBuffer.BindPS(3);


		auto wvpLight = Transpose(mMatrices[0] * viewMat * projMat);
		mShadowConstantBuffer.Load(wvpLight);
		mShadowConstantBuffer.BindVS(4);

		depthMap.BindPS(5);

		mMeshBuffer.Render();
	}

	void Unload()
	{
		mTransformBuffer.Terminate();
		mLightBuffer.Terminate();
		mMaterialBuffer.Terminate();
		mMeshBuffer.Terminate();
		mVertexShader.Terminate();
		mPixelShader.Terminate();
		mConstantBuffer.Terminate();
		mShadowConstantBuffer.Initialize();
	}

	void DebugUI()
	{
		ImGui::Begin("Standard Setting");
		if (ImGui::CollapsingHeader("Light"))
		{
			bool directionChanged = false;
			directionChanged |= ImGui::DragFloat("Direction X##Light", &mLight.direction.x, 0.05f, -1.0f, 1.0f);
			directionChanged |= ImGui::DragFloat("Direction Y##Light", &mLight.direction.y, 0.05f, -1.0f, 1.0f);
			directionChanged |= ImGui::DragFloat("Direction Z##Light", &mLight.direction.z, 0.05f, -1.0f, 1.0f);
			if (directionChanged)
			{
				mLight.direction = NFGE::Math::Normalize(mLight.direction);
			}
			ImGui::ColorEdit4("Ambient##Light", &mLight.ambient.r);
			ImGui::ColorEdit4("Diffuse##Light", &mLight.diffuse.r);
			ImGui::ColorEdit4("Specular##Light", &mLight.specular.r);
		}

		if (ImGui::CollapsingHeader("Material"))
		{

			ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
			ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
			ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
			ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 0.0f, 100.0f);
		}

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::DragFloat("rotationX##Transform", &mRotation.x, 1.0f, 0.0f, 360.0f);
			ImGui::DragFloat("rotationY##Transform", &mRotation.y, 1.0f, 0.0f, 360.0f);
			ImGui::DragFloat("rotationZ##Transform", &mRotation.z, 1.0f, 0.0f, 360.0f);
			ImGui::DragFloat("positionX##Transform", &mPosition.x, 1.0f, 0.0f, 360.0f);
			ImGui::DragFloat("positionY##Transform", &mPosition.y, 1.0f, 0.0f, 360.0f);
			ImGui::DragFloat("positionZ##Transform", &mPosition.z, 1.0f, 0.0f, 360.0f);
		}

		ImGui::DragFloat("bumpWeight", &mBumpWeight, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("brightness", &mBrightness, 0.1f, 0.0f, 100.0f);

		ImGui::End();
	}
	MeshBuffer mMeshBuffer;
private:
	// -Tank---------------------------
	Mesh mesh;
	Matrix4 mMatrices[3];
	VertexShader mVertexShader;
	PixelShader mPixelShader;
	ConstantBuffer mConstantBuffer;

	Vector3 mPosition;
	Vector3 mRotation;

	DirectionalLight mLight;
	Material mMaterial;
	TextureId mDiffuseTexture;
	TextureId mSpecularTexture;
	TextureId mDisplacementTexture;
	float mBumpWeight;
	TextureId mNormalTexture;
	TextureId mAOTexture;

	struct TransformData
	{
		NFGE::Math::Matrix4 world;
		NFGE::Math::Matrix4 wvp;
		NFGE::Math::Vector3 viewPositon;
		float padding;
	};
	struct OptionsData
	{
		float displacementWeight;
		int notImportant;
		float brightness = 20.0f;
		int useShadow;
	};
	using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight>;
	using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;
	using OptionBuffer = NFGE::Graphics::TypedConstantBuffer<OptionsData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionBuffer mOptionDateBuffer;
	MeshBuffer mTankMeshBuffer;
	ShadowConstantBuffer mShadowConstantBuffer;
	// -Tank---------------------------

	std::unique_ptr<AI::SteeringModule3D> mSteeringModule;
	uint8_t mControlState;
	float mXRotationMagitude;
	float mYRotationMagitude;
	float mDestinationZRotation;
	Vector3 mUp;

	float mBrightness = 20.0f;
	void HookCamera(Camera& camera, float deltaTime)
	{
		camera.SetDirection(position - camera.GetPosition());
		auto cameraSupposePos = (position - (heading * 30.0f)) + Vector3(0.0f, 10.0f, 0.0f);

		if (NFGE::Math::Magnitude(cameraSupposePos - camera.GetPosition()) > 0.001f)
			camera.SetPosition(camera.GetPosition() + ((cameraSupposePos - camera.GetPosition()) * 0.3f)* 10.0f * deltaTime);
	}
	void Control(float deltaTime)
	{
		mControlState &= ControlState_Idle;
		auto inputSystem = InputSystem::Get();
		if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		{
			//mControlState |= ControlState_XRotation;
			//mXRotationMagitude = inputSystem->GetMouseMoveY() * TurnSpeed * 0.01f * deltaTime;
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

		
	}
	void ShipAction(float deltaTime)
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
		//if (NFGE::Math::Abs(mDestinationZRotation - mRotation.z) > 0.01f)
		//{
		//	//mRotation.z += (mDestinationZRotation - mRotation.z) * 0.3f * deltaTime;
		//}
	}
	void UpdateSimplePhysics(float deltaTime)
	{
		auto force = mSteeringModule->Calculate();
		auto acceleration = force / mass;
		velocity += acceleration * deltaTime;
		// TODO add truncate method here
		position += velocity * deltaTime;

		if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
			heading = NFGE::Math::Normalize(velocity);
	}
};