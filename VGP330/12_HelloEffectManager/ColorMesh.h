#pragma once
#include <NFGE/Inc/NFGE.h>

class ColorMesh
{
public:
	void Load()
	{
		mMeshBuffer.Initialize(NFGE::Graphics::MeshBuilder::CreateCubePC());
	}
	void Update(float deltaTime)
	{
		
	}
	void Render(NFGE::Graphics::Camera* camera)
	{
		auto effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::ColorMesh);
		mMeshBuffer.Render(effect, &mEffectContext, *camera);
	}
	void Unload()
	{
		mMeshBuffer.Terminate();
	}
private:
	NFGE::Graphics::Effects::ColorMeshEffect::ColorMeshEffectContext mEffectContext;
	NFGE::Graphics::MeshBuffer mMeshBuffer;
};