#include "GameState.h"

namespace 
{
	const wchar_t* shaderFileName5 = L"../../Assets/Shaders/PostProcess.fx";

	const wchar_t* floorShaderFileName = L"../../Assets/Shaders/Standard.fx";
	const char* floorSpecular = "Wood_012_Tiles_ambientOcclusion.jpg";
	const char* floorDiffuse = "Cobblestone_001_COLOR.jpg";
	const char* floorNormal = "Wood_012_Tiles_normal.jpg";
	const char* floorDisplacement = "Wood_012_Tiles_height.png";

	const wchar_t* depthShaderFileName = L"../../Assets/Shaders/DepthMap.fx";

	const int row = 5;
	const int col = 4;
	const int TotalTanks = row * col;
}

void CameraControl(Camera& camera);

void GameState::Initialize()
{
	auto graphicSystem = GraphicsSystem::Get();
	GraphicsSystem::Get()->SetClearColor(Colors::DimGray);

	NFGE::sApp.GetMainCamera().SetPosition({ 0.0f, 0.0f, -10.0f });
	NFGE::sApp.GetMainCamera().SetDirection({ 0.0f, 0.0f, 1.0f });
	NFGE::sApp.GetMainCamera().SetFarPlane(500.0f);

	mTankInitPosition = { 0.0f,3.5f,0.0f };
	
	mDebugCamera.SetNearPlane(0.001f);
	mDebugCamera.SetPosition({-500.0f,100.0f,-500.0f});
	auto debugCameraDirection = Normalize(mTankInitPosition - mDebugCamera.GetPosition());
	mDebugCamera.SetDirection(debugCameraDirection);
	mDebugCamera.SetFarPlane(10000.0f);

	mLightCamera.SetDirection(NFGE::sApp.GetMainLight().direction);
	mLightCamera.SetNearPlane(-1000.0f);
	mLightCamera.SetFarPlane(1000.0f);

	mActiveCamera = &NFGE::sApp.GetMainCamera();

	tank.Load(mTankInitPosition);
	mTanks.reserve(TotalTanks);
	for (size_t i = 0; i < TotalTanks; i++)
	{
		mTanks.emplace_back(world);
		mTanks[i].Load(mTankInitPosition);
	}


	spaceShip.Load("../../Assets/Model/Spaceship.NFModelRV", &NFGE::sApp.GetMainLight());
	spaceShip.mContext.position = Vector3{ 10.0f, 5.0f, 0.0f };
	spaceShip.mContext.AdjustRotation.x = 90.0f;

	mPlane.Load(floorShaderFileName, floorShaderFileName, 10.0f, 10.0f, floorSpecular, floorDiffuse, floorDisplacement, floorNormal);

	// PostProcess
	mScreenMesh = MeshBuilder::CreateNDCQuad();
	mScreenMeshBuffer.Initialize(mScreenMesh);

	mPostProcessPixelShader.Initialize(shaderFileName5, "NoPostProcess");
	mPostProcessVertexShader.Initialize(shaderFileName5, VertexPX::Format);
	mRenderTarget.Initialize(graphicSystem->GetBackBufferWidth(), graphicSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);

	//Shadow
	constexpr uint32_t depthMapSize = 1024 * 4;
	mDepthMapRanderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	mDepthVertexShaderTarget.Initialize(depthShaderFileName, Vertex::Format);
	mDepthPixelShaderTarget.Initialize(depthShaderFileName);
	mDepthConstantBuffer.Initialize();
	
}

void GameState::Terminate()
{
	spaceShip.UnLoad();
	mPlane.UnLoad();
	tank.Unload();
	for (size_t i = 0; i < TotalTanks; i++)
	{
		mTanks[i].Unload();
	}
	// PostProcess
	mScreenMeshBuffer.Terminate();
	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mRenderTarget.Terminate();

	//Shadow                                                                                                                                                                                                                                                                                                                          
	mDepthMapRanderTarget.Terminate();
	mDepthConstantBuffer.Terminate();
	mDepthVertexShaderTarget.Terminate();
	mDepthPixelShaderTarget.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	//mLightCamera.SetPosition(tank.position - NFGE::sApp.GetMainLight().direction * 50.0f);
	mLightCamera.SetDirection(NFGE::sApp.GetMainLight().direction);
	//CameraControl(NFGE::sApp.GetMainCamera());
	CameraControl(mDebugCamera);

	
	for (size_t i = 0; i < TotalTanks; i++)
	{
		mTanks[i].position.x = tank.position.x + i / col * spacing;
		mTanks[i].position.z = tank.position.z + i % col * spacing;
		mTanks[i].position = mTanks[i].position - Vector3(row * 0.5f * spacing, 0.0f, col * 0.5f * spacing);
		mTanks[i].destination.x = tank.destination.x + i / col * spacing;
		mTanks[i].destination.z = tank.destination.z + i % col * spacing;
		mTanks[i].destination = mTanks[i].destination - Vector3(row * 0.5f * spacing, 0.0f, col * 0.5f * spacing);
		//mTanks[i].destination = mTanks[i].position + Vector3(0.1f, 0.0f, 0.0f);
		mTanks[i].SubUpdate(deltaTime, &NFGE::sApp.GetMainCamera());

	}
	tank.Update(deltaTime, &NFGE::sApp.GetMainCamera());
}

void GameState::Render()
{
	mViewFrustumVertices =
	{
		// NearPlane
		{ -1.0f, -1.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f },

	// FarPlane
	{ -1.0f, -1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f },
	};

	auto mainCamera = NFGE::sApp.GetMainCamera();

	auto defaultMatView = mainCamera.GetViewMatrix();
	auto defaultMatProj = mainCamera.GetPerspectiveMatrix();
	auto invViewProj = Inverse(defaultMatView * defaultMatProj);
	for (auto& vertex : mViewFrustumVertices)
	{
		vertex = TransformCoord(vertex, invViewProj);
	}

	SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[1], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[2], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[3], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[0], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[4], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[5], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[6], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[7], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[4], mViewFrustumVertices[5], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[5], mViewFrustumVertices[6], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[6], mViewFrustumVertices[7], Colors::OrangeRed);
	SimpleDraw::AddLine(mViewFrustumVertices[7], mViewFrustumVertices[4], Colors::OrangeRed);

	auto lightLook = mLightCamera.GetDirection();
	auto lightSide = Normalize(Cross(Vector3::YAxis, lightLook));
	auto lightUp = Normalize(Cross(lightLook, lightSide));
	float minX = FLT_MAX, maxX = -FLT_MAX;
	float minY = FLT_MAX, maxY = -FLT_MAX;
	float minZ = FLT_MAX, maxZ = -FLT_MAX;

	for (auto& vertex : mViewFrustumVertices)
	{
		float projectX = Dot(lightSide, vertex);
		minX = Min(minX, projectX);
		maxX = Max(maxX, projectX);
		float projectY = Dot(lightUp, vertex);
		minY = Min(minY, projectY);
		maxY = Max(maxY, projectY);
		float projectZ = Dot(lightLook, vertex);
		minZ = Min(minZ, projectZ);
		maxZ = Max(maxZ, projectZ);
	}

	mLightCamera.SetPosition(
		lightSide * (minX + maxX) * 0.5f +
		lightUp * (minY + maxY) * 0.5f +
		lightLook * (minZ + maxZ) * 0.5f
	);
	//mLightCamera.SetPosition(tank.position);
	
	mLightMatView = mLightCamera.GetViewMatrix();
	mLightMatProj = mLightCamera.GetOrthographicMatrix(maxX - minX, maxY - minY);

	mViewFrustumVertices =
	{
		// NearPlane
		{ -1.0f, -1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f },

		// FarPlane
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
	};
	auto lightViewProj = Inverse( mLightMatView * mLightMatProj);
	for (auto& vertex : mViewFrustumVertices)
	{
		vertex = TransformCoord(vertex, lightViewProj);
	}
	SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[1], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[2], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[3], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[0], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[4], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[5], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[6], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[7], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[4], mViewFrustumVertices[5], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[5], mViewFrustumVertices[6], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[6], mViewFrustumVertices[7], Colors::LightGreen);
	SimpleDraw::AddLine(mViewFrustumVertices[7], mViewFrustumVertices[4], Colors::LightGreen);

	SimpleDraw::AddSphere(mLightCamera.GetPosition(), 10.0f, Colors::Blue);
	mDepthMapRanderTarget.BeginRender();  
	DrawDepthMap();
	mDepthMapRanderTarget.EndRender();

	mRenderTarget.BeginRender();
	RenderScene();
	mRenderTarget.EndRender();

	PostProcess();
}

void GameState::DebugUI()
{
	if (ImGui::Button("Switch Camera"))
	{
		if (mActiveCamera == &NFGE::sApp.GetMainCamera())
			mActiveCamera = &mLightCamera;
		else
			mActiveCamera = &NFGE::sApp.GetMainCamera();
	}
	
	if (ImGui::Button("Debug Camera"))
	{
		mActiveCamera = &mDebugCamera;
	}

	ImGui::Image(
		mDepthMapRanderTarget.GetShaderResourceView(),
		{150.0f,150.0f},
		{0.0f,0.0f},
		{ 1.0f,1.0f },
		{1.0f,1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f,1.0f }
	);

	auto& cameraDir = NFGE::sApp.GetMainCamera().GetDirection();
	auto& cameraPos = NFGE::sApp.GetMainCamera().GetPosition();
	ImGui::Text("Camera Pos, %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Dir, %f, %f, %f", cameraDir.x, cameraDir.y, cameraDir.z);

	ImGui::DragFloat("Spacing", &spacing, 1.0f, 0.0f, 100.0f);

	tank.DebugUI();
}


void GameState::RenderScene()
{
	//for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	//{
	//	SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::White);
	//	SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::White);e
	//}
	//SimpleDraw::Render(NFGE::sApp.GetMainCamera());
	
	
	

	spaceShip.Render(*mActiveCamera);


	tank.Render(*mActiveCamera, mLightMatView, mLightMatProj, mDepthMapRanderTarget);
	for (size_t i = 0; i < TotalTanks; i++)
	{
		mTanks[i].Render(*mActiveCamera, mLightMatView, mLightMatProj, mDepthMapRanderTarget);
	}

	mPlane.Render(NFGE::Math::Matrix4::sIdentity(), *mActiveCamera, mLightMatView, mLightMatProj, mDepthMapRanderTarget);
	//NFGE::sApp.DrawAABB(-10.0f, -10.0f, -10.0f, 10.0f, 10.0f, 10.0f, Colors::Red);
	
	//SimpleDraw::AddAABB(-10.0f, -10.0f, -10.0f, +10.0f, +10.0f, +10.0f, Colors::Red, true);
	//SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[1], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[2], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[3], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[0], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[4], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[5], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[6], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[7], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[4], mViewFrustumVertices[5], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[5], mViewFrustumVertices[6], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[6], mViewFrustumVertices[7], Colors::OrangeRed);
	//SimpleDraw::AddLine(mViewFrustumVertices[7], mViewFrustumVertices[4], Colors::OrangeRed);
	SimpleDraw::Render(*mActiveCamera);
}

void GameState::PostProcess()
{
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();

	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);

	PostProcessManager::Get()->GetMasterRenderTarget().ResumeRender();
	mRenderTarget.BindPS(0);
	mScreenMeshBuffer.Render();
	mRenderTarget.UnbindPS(0);
	PostProcessManager::Get()->GetMasterRenderTarget().EndRender();
}

void GameState::DrawDepthMap()
{
	

	mDepthVertexShaderTarget.Bind();
	mDepthPixelShaderTarget.Bind();

	auto matViewLight = mLightMatView;
	auto matProjectLight = mLightMatProj;

	auto matTrans = Matrix4::sTranslation(tank.position);
	auto matRot = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(tank.heading, Vector3::YAxis));
	auto matWorld = matRot * matTrans;
	auto wvp = Transpose(matWorld * matViewLight * matProjectLight);
	mDepthConstantBuffer.Load(wvp);
	mDepthConstantBuffer.BindVS(0);

	tank.mMeshBuffer.Render();
	for (size_t i = 0; i < TotalTanks; i++)
	{
		auto matTrans = Matrix4::sTranslation(mTanks[i].position);
		auto matRot = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(mTanks[i].heading, Vector3::YAxis));
		auto matWorld = matRot * matTrans;
		auto wvp = Transpose(matWorld * matViewLight * matProjectLight);
		mDepthConstantBuffer.Load(wvp);
		mTanks[i].mMeshBuffer.Render();
	}

}

void CameraControl(Camera& camera)
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
	lastTime = currentTime;

	const float moveSpeed = 10.0f;
	const float turnSpeed = 5.0f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		camera.Pitch(-inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
	
}
