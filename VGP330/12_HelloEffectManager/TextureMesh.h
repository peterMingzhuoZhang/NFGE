#pragma once
#pragma once
#include <NFGE/Inc/NFGE.h>

class TextureMesh
{
public:
	void Load()
	{
		mMeshBuffer.Initialize(NFGE::Graphics::MeshBuilder::CreatePlanePX());
		mEffectContext.textureId = NFGE::Graphics::TextureManager::Get()->LoadTexture("brickwork.jpg");
		mEffectContext.position.x += 3.0f;
	}
	void Update(float deltaTime)
	{

	}
	void Render(NFGE::Graphics::Camera* camera)
	{ 
		auto effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::TextureMesh);
		mMeshBuffer.Render(effect, &mEffectContext, *camera);
	}
	void Unload()
	{
		mMeshBuffer.Terminate();
	}
private:
	NFGE::Graphics::Effects::TextureMeshEffect::TextureMeshEffectContext mEffectContext;
	NFGE::Graphics::MeshBuffer mMeshBuffer;
};