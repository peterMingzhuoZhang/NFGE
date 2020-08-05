#include "Scene.h"

using namespace NFGE::Input;

namespace
{
	const char* SkeletonFileName = "../../Assets/Model/yBot2.NFSkeleton";
	const char* modelFileName_0 = "../../Assets/Model/ybot.NFModelRV";
	const wchar_t* shaderFileName = L"../../Assets/Shaders/Standard_Model_ZMZ.fx";
}

void Scene::Load()
{
	mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,1.0f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mLightBuffer.Initialize();

	mModel.Load(Vector3(0.0f, 0.0f, -50.0f), Vector3(), Vector3(1.0f, 1.0f, 1.0f), modelFileName_0, shaderFileName, shaderFileName);
	mModel2.Load(modelFileName_0,SkeletonFileName, &NFGE::sApp.GetMainLight());
	mModel2.mContext.position = Vector3(0.0f, 0.0f, -150.0f);

	mPTSFSkeleton.Load(SkeletonFileName, &mLight);
}

void Scene::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::TAB))
	{
		
	}

	mPTSFSkeleton.Update(deltaTime);
	mModel2.Update(deltaTime);

}

void Scene::Render(const Graphics::Camera & camera)
{
	mLightBuffer.Load(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mPTSFSkeleton.Render(camera);
	mModel.Render(NFGE::sApp.GetMainCamera());
	mModel2.Render(camera);
	mModel2.DebugUI(camera);

	
}

void Scene::DebugUI()
{
	mPTSFSkeleton.DebugUI();
	mModel.DebugUI();
}

void Scene::Unload()
{
	mLightBuffer.Terminate();
	mModel.UnLoad();
}
