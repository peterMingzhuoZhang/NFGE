#ifndef INCLUDED_SCENE_HELLOROBOTICARM_ZORY_SCENE_H
#define INCLUDED_SCENE_HELLOROBOTICARM_ZORY_SCENE_H

#include <NFGE/Inc/NFGE.h>

using namespace NFGE;

class Zory_Scene
{
public:
	void Load()
	{
		mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,0.5f });
		mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
		mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
		mLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
		mLightBuffer.Initialize();
	}
	void Update(float deltaTime)
	{

	}
	void Render(const Graphics::Camera& camera)
	{
		mLightBuffer.Load(mLight);
		mLightBuffer.BindVS(1);
		mLightBuffer.BindPS(1);
	}
	void Unload() 
	{
		mLightBuffer.Terminate();
	}
private:
	Graphics::DirectionalLight mLight;
	Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight> mLightBuffer;
};

#endif // !INCLUDED_SCENE_HELLOROBOTICARM_ZORY_SCENE_H
