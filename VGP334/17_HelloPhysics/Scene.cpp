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

}

void Scene::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::TAB))
	{
		
	}



}

void Scene::Render(const Graphics::Camera & camera)
{
	mLightBuffer.Load(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
	
}

void Scene::DebugUI()
{

}

void Scene::Unload()
{
	mLightBuffer.Terminate();
}
