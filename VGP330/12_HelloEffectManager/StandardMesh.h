#pragma once
#include <NFGE/Inc/NFGE.h>

using namespace NFGE::Graphics;

class StandardMesh
{
public:
	void Load(DirectionalLight* light)
	{
		mMeshBuffer.Initialize(NFGE::Graphics::MeshBuilder::CreateSphere(10,10,5.0f));
		mEffectContext.AddTexture(MeshTextureMaterial::DIFFUSE, TextureManager::Get()->LoadTexture("earth.jpg"));
		mEffectContext.AddTexture(MeshTextureMaterial::SPECULAR, TextureManager::Get()->LoadTexture("earth_spec.jpg"));
		mEffectContext.AddTexture(MeshTextureMaterial::DISPLACEMENT, TextureManager::Get()->LoadTexture("earth_bump.jpg"));
		mEffectContext.AddTexture(MeshTextureMaterial::NORMALS, TextureManager::Get()->LoadTexture("earth_normal.jpg"));
		
		mEffectContext.position.x += 15.0f;
		mEffectContext.light = light;
	}
	void Update(float deltaTime)
	{

	}
	void Render(const NFGE::Graphics::Camera& camera)
	{
		auto effectManager = EffectManager::Get();
		auto effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::StandardMesh);
		//mMeshBuffer.Render(effect, &mEffectContext, camera);
		effectManager->RegisterRenderObject(effect, &mEffectContext, &mMeshBuffer, camera);
	}
	void Unload()
	{
		mMeshBuffer.Terminate();
	}
private:
	NFGE::Graphics::Effects::StandardMeshEffect::StandardMeshEffectContext mEffectContext;
	NFGE::Graphics::MeshBuffer mMeshBuffer;
};