#include "Precompiled.h"
#include "App.h"

#include "AppState.h"
#include "MetaRegistration.h"

#include "World.h"
#include "CameraService.h"

#include "Graphics/Src/D3DUtil.h"
#pragma comment(lib, "FW1FontWrapper.lib")


using namespace NFGE;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

void NFGE::App::ChangeState(std::string name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}
void NFGE::App::Run(AppConfig appConfig)
{
	LOG("App starting ...");

	LOG("Registering meta types ...");
	Core::StaticMetaRegister();
	Math::StaticMetaRegister();
	Graphics::StaticMetaRegister();
	NFGE::StaticMetaRegister();

	mAppConfig = std::move(appConfig);

	LOG("Creating window ...");
	mWindow.Initialize(GetModuleHandle(NULL), mAppConfig.appName.c_str(), mAppConfig.windowWidth, mAppConfig.windowHeight, mAppConfig.maximize);

	// Initialize the input system
	auto handle = mWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);

	// Initialize the graphic system
	GraphicsSystem::StaticInitialize(handle, false);
	SamplerManager::StaticInitialize();
	TextureManager::StaticInitialize(mAppConfig.assetsDirectory / "Images");
	ModelManager::StaticInitialize(mAppConfig.assetsDirectory / "Model");
	SpriteRenderer::StaticInitialize();
	SimpleDraw::StaticInitialize(mAppConfig.assetsDirectory / "Shaders");
	DebugUI::StaticInitialize(handle, mAppConfig.imGuiDocking, true);
	BlendStateManager::StaticInitialize();
	RasterizerStateManager::StaticInitialize();
	DepthStencilStateManager::StaticInitialize();
	PostProcessManager::StaticInitialize(mAppConfig.assetsDirectory / "Shaders", GraphicsSystem::Get()->GetBackBufferWidth(), GraphicsSystem::Get()->GetBackBufferHeight(), &GetMainCamera(), &GetMainLight(),RenderTarget::Format::RGBA_U8);
	EffectManager::StaticInitialize(mAppConfig.assetsDirectory / "Shaders");
	GeometryMeshManager::StaticInitialize(&mLight);
	myFont.Initialize();

	myCamera.SetFOV(60.0f * Math::Constants::DegToRad);
	myCamera.SetNearPlane(0.1f);
	myCamera.SetFarPlane(1000.0f);

	mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,-1.0f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	initialized = true;
	// Initialize the starting state
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();

	myTimer.Initialize();

	bool done = false;
	while (!done)
	{
		done = mWindow.ProcessMessage();
		if (done)
			break;

		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = mNextState;		//|---> profession way mCurrentState = std:exchange(mNextState, nullptr);
			mNextState = nullptr;			//|
			mCurrentState->Initialize();
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
		lastTime = currentTime;

		myTimer.Update();

		mCurrentState->Update(deltaTime);

		GraphicsSystem::Get()->BeginRender();

		// GameState render
		BlendStateManager::Get()->GetBlendState("Opaque")->Set();
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
		DepthStencilStateManager::Get()->GetDepthStencilState("Default")->Set();

		DebugUI::BeginRender(); // Some app use imgui in the main render // example Lacalle craft
		{//Experimenting
			auto effectManager = EffectManager::Get();
			auto postProcessManager = PostProcessManager::Get();
			postProcessManager->SetCurrentUsingLight(&GetMainLight());
			postProcessManager->SetCurrentUsingCamera(&GetMainCamera());

			auto& masterRenderTarget = postProcessManager->GetMasterRenderTarget();
			masterRenderTarget.BeginRender();	// incase some object not using effect system to render
			mCurrentState->Render();			// Register the render object in to the effect & post-process pipeline
			masterRenderTarget.EndRender();
			
			effectManager->PreparePostProcessEffect();
			// TextureRender
			masterRenderTarget.ResumeRender();
			{
				effectManager->MainRender();
				SpriteRenderer::Get()->BeginRender();
				ExecuteSpriteCommand();
				SpriteRenderer::Get()->EndRender();

				DepthStencilStateManager::Get()->GetDepthStencilState("Default")->Set();;
				// Text render
				ExecuteTextCommand();
				SimpleDraw::Render(GetMainCamera());
				
			}
			masterRenderTarget.EndRender();

			postProcessManager->SetLastRenderTargetPtr(&masterRenderTarget);

			effectManager->RunPostProcessEffect();

			if (!appConfig.isEditor)
			{
				postProcessManager->FinalToScreenRender();
			}

			effectManager->ClearRenderObject();

			// Imgui render
			mCurrentState->DebugUI();
			//postProcessManager->OptionUI();
			DebugUI::EndRender();
			//post
		}
		// Origin VV
		//mCurrentState->Render();
		//
		//// TextureRender
		//SpriteRenderer::Get()->BeginRender();
		//ExecuteSpriteCommand();
		//SpriteRenderer::Get()->EndRender();
		//
		//DepthStencilStateManager::Get()->GetDepthStencilState("Default")->Set();
		//SimpleDraw::Render(myCamera);
		//// Text render
		//ExecuteTextCommand();
		//
		//// Imgui render
		//DebugUI::BeginRender();
		//mCurrentState->DebugUI();
		//DebugUI::EndRender();

		GraphicsSystem::Get()->EndRender();
	}

	// Clean up current state
	mCurrentState->Terminate();

	myFont.Terminate();
	GeometryMeshManager::StaticTerminate();
	EffectManager::StaticTerminate();
	PostProcessManager::StaticTerminate();
	DepthStencilStateManager::StaticTerminate();
	RasterizerStateManager::StaticTerminate();
	BlendStateManager::StaticTerminate();
	DebugUI::StaticTerminate();
	SimpleDraw::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	ModelManager::StaticTerminate();
	TextureManager::StaticTerminate();
	SamplerManager::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();

	mWindow.Terminate();
}

//----------------------------------- Mingzhuo zhang Editing --------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------|||||||||||||||||||||||||||----------------------------------------------------------------------
//------------------------------------vvvvvvvvvvvvvvvvvvvvvvvvvvv----------------------------------------------------------------------
inline uint32_t ToColor(const Math::Vector4& color)
{
	uint8_t r = (uint8_t)(color.x * 255);
	uint8_t g = (uint8_t)(color.y * 255);
	uint8_t b = (uint8_t)(color.z * 255);
	uint8_t a = (uint8_t)(color.w * 255);
	return (a << 24) | (b << 16) | (g << 8) | r;
}

void NFGE::App::ExecuteSpriteCommand()
{
	for (const auto& command : mySpriteCommands)
	{
		Texture* texture = TextureManager::Get()->GetTexture(command.textureId);
		if (texture)
		{
			if (command.alpha == 1.0f)
			{
				if (Math::IsEmpty(command.sourceRect))
				{
					//SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.pivot);
					SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.alpha, command.anchorX, command.anchorY, command.xScale, command.yScale);
				}
				else if (command.anchorX != -0.1f || command.anchorY != -0.1f)
				{
					SpriteRenderer::Get()->Draw(*texture, command.sourceRect, command.position, command.rotation, command.alpha, command.anchorX, command.anchorY, command.xScale, command.yScale);
				}
				else
				{
					SpriteRenderer::Get()->Draw(*texture, command.sourceRect, command.position, command.rotation, command.pivot);
				}
			}
			else
			{
				if (command.anchorX != -0.1f || command.anchorY != -0.1f)
				{
					SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.alpha, command.anchorX, command.anchorY, command.xScale, command.yScale);
				}
				else
				{
					SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.alpha);
				}
			}
		}
	}
	mySpriteCommands.clear();
}

void NFGE::App::ExecuteTextCommand()
{
	for (const auto& command : myTextCommands)
	{
		myFont.Draw(command.str.c_str(), command.size, command.x, command.y, command.color);
	}
	myTextCommands.clear();
}

//------------------------------------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^--------------------------------------------------------------
//------------------------------------|||||||||||||||||||||||||||||||||||--------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------- End Edite -------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
float NFGE::App::GetTime()
{
	return myTimer.GetTotalTime();
}

//----------------------------------------------------------------------------------------------------
float NFGE::App::GetDeltaTime()
{
	return myTimer.GetElapsedTime();
}

//----------------------------------------------------------------------------------------------------
TextureId NFGE::App::LoadTexture(const char* fileName)
{
	ASSERT(initialized, "[NFGE] Engine not started.");
	return TextureManager::Get()->LoadTexture(fileName);
}

//----------------------------------------------------------------------------------------------------
void NFGE::App::ClearAllTextures()
{
	ASSERT(initialized, "[NFGE] Engine not started.");
	return TextureManager::Get()->Clear();
}

//----------------------------------------------------------------------------------------------------

bool NFGE::App::OpenFileDialog(char fileName[MAX_PATH], const char* title, const char* filter)
{
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = mWindow.GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title;
	return GetOpenFileNameA(&ofn);
}

//----------------------------------------------------------------------------------------------------

bool NFGE::App::SaveFileDialog(char fileName[MAX_PATH], const char* title, const char* filter)
{
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = mWindow.GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title;
	return GetSaveFileNameA(&ofn);
}
//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawLine(float x0, float y0, float z0, float x1, float y1, float z1, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddLine(x0, y0, z0, x1, y1, z1, color);
}

void NFGE::App::DrawLine(const Math::Vector3 & v0, const Math::Vector3 & v1, const Graphics::Color & color)
{
	DrawLine(v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawAABB(const Math::AABB& aabb, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddAABB(aabb, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddAABB(minX, minY, minZ, maxX, maxY, maxZ, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawOBB(const Math::OBB& obb, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddOBB(obb, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices, uint32_t rings)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddSphere(center, radius, color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawSphere(float x, float y, float z, float radius, const Color& color, uint32_t slices, uint32_t rings)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddSphere(Math::Vector3(x, y, z), radius, color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenLine(v0, v1, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenLine(float x0, float y0, float x1, float y1, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenLine(x0, y0, x1, y1, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenRect(const Math::Rect& rect, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenRect(rect.left, rect.top, rect.right, rect.bottom, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenRect(float l, float t, float r, float b, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenRect(l, t, r, b, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenCircle(const Math::Circle& circle, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(circle, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenCircle(const Math::Vector2& center, float r, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(center, r, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenCircle(float x, float y, float r, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(x, y, r, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenDiamond(const Math::Vector2& center, float size, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenDiamond(center, size, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenDiamond(float x, float y, float size, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenDiamond(x, y, size, color);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawScreenText(const char* str, float x, float y, float size, const Color& color)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	size_t strLength = strlen(str);
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, strLength, NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, strLength, &wstrTo[0], size_needed);
	myTextCommands.emplace_back(wstrTo, size, x, y, ToColor(color));
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawSprite(TextureId textureId, const Math::Vector2& position)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	mySpriteCommands.emplace_back(textureId, position, 0.0f);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, Pivot pivot)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	mySpriteCommands.emplace_back(textureId, position, rotation, pivot);
}

//----------------------------------------------------------------------------------------------------

void NFGE::App::DrawSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	mySpriteCommands.emplace_back(textureId, sourceRect, position, 0.0f);
}

//----------------------------------------------------------------------------------------------------

//----------------------------------- Mingzhuo zhang Editing --------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------|||||||||||||||||||||||||||----------------------------------------------------------------------
//------------------------------------vvvvvvvvvvvvvvvvvvvvvvvvvvv----------------------------------------------------------------------
//------------------- Mingzhuo zhang : Draw sprite with alpha --------------------------------------

void NFGE::App::DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, float alpha)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	mySpriteCommands.emplace_back(textureId, position, rotation, alpha);
}

//------------------- Mingzhuo zhang : Draw sprite with alpha & anchor point ---------------------

void NFGE::App::DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale, Rect rect)
{
	ASSERT(initialized, "[XEngine] Engine not started.");
	mySpriteCommands.emplace_back(textureId, position, rotation, alpha, anchorX, anchorY, xScale, yScale, rect);
}
//----------------------------------------------------------------------------------------------------

//------------------------------------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^--------------------------------------------------------------
//------------------------------------|||||||||||||||||||||||||||||||||||--------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------- End Edite -------------------------------------------------------------------------------------

uint32_t NFGE::App::GetSpriteWidth(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetWidth() : 0u;
}

//----------------------------------------------------------------------------------------------------

uint32_t NFGE::App::GetSpriteHeight(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetHeight() : 0u;
}

void* NFGE::App::GetSprite(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? GetShaderResourceView(*texture) : nullptr;
}
//----------------------------------------------------------------------------------------------------

uint32_t NFGE::App::GetScreenWidth()
{
	return GraphicsSystem::Get()->GetBackBufferWidth();
}

//----------------------------------------------------------------------------------------------------

uint32_t NFGE::App::GetScreenHeight()
{
	return GraphicsSystem::Get()->GetBackBufferHeight();
}

Camera& NFGE::App::GetMainCamera()
{
	if (mAppConfig.isEditor && mWorld != nullptr)
		return mWorld->GetService<NFGE::CameraService>()->GetActiveCamera();
	else
		return myCamera;
}

NFGE::Graphics::DirectionalLight & NFGE::App::GetMainLight()
{
	return mLight;
}

void NFGE::App::SoSoCameraControl(float turnSpeed, float moveSpeed, NFGE::Graphics::Camera & camera, float deltaTime)
{

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		camera.Pitch(-inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}
void NFGE::App::SoSoCameraControl(float turnSpeed, float moveSpeed, CameraEntry& cameraEntry, float deltaTime)
{

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		cameraEntry.camera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		cameraEntry.camera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		cameraEntry.camera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		cameraEntry.camera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		cameraEntry.camera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		cameraEntry.camera.Pitch(-inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	cameraEntry.mPosition = cameraEntry.camera.GetPosition();
	cameraEntry.mDirection = cameraEntry.camera.GetDirection();
}

//----------------------------------------------------------------------------------------------------
