#include "Scene.h"

void Scene::Load()
{
	mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,0.5f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mLightBuffer.Initialize();
}

void Scene::Update(float deltaTime)
{
}

void Scene::Render(const Graphics::Camera & camera)
{
	mLightBuffer.Load(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
}

void Scene::Unload()
{
	mLightBuffer.Terminate();
}
