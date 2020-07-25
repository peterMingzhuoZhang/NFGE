#include "Scene.h"

using namespace NFGE::Input;

void Scene::Load()
{
	mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,0.5f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mLightBuffer.Initialize();

	mTentaclePC.Load(2, 10.0f, "earth.jpg", "earth_normal.jpg", "earth_spec.jpg", "earth_bump.jpg");
	mTentaclePA.Load(2, 10.0f, "earth.jpg", "earth_normal.jpg", "earth_spec.jpg", "earth_bump.jpg");
}

void Scene::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::TAB))
	{
		isSelectPA = !isSelectPA;
	}

	if (isSelectPA)
		mTentaclePA.Update(deltaTime);
	else
		mTentaclePC.Update(deltaTime);
}

void Scene::Render(const Graphics::Camera & camera)
{
	mLightBuffer.Load(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	if (isSelectPA)
		mTentaclePA.Render(camera);
	else
		mTentaclePC.Render(camera);
}

void Scene::DebugUI()
{
	if (isSelectPA)
		mTentaclePA.DebugUI();
	else
		mTentaclePC.DebugUI();
}

void Scene::Unload()
{
	mLightBuffer.Terminate();
	mTentaclePC.UnLoad();
	mTentaclePA.UnLoad();
}
