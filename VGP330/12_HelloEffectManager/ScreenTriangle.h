#pragma once
#include <NFGE/Inc/NFGE.h>

class ScreenTriangle
{
public:
	void Load()
	{
		const NFGE::Graphics::VertexPC vertices[] =
		{
			{ { 0.0f, 0.5f,0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.5f, 0.0f,0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.5f, 0.0f,0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
		};
		mMeshBuffer.Initialize(vertices, static_cast<uint32_t>(std::size(vertices)), sizeof(NFGE::Graphics::VertexPC));
	}
	void Update(float deltaTime)
	{
	}

	void Render(NFGE::Graphics::Camera* camera)
	{
		auto effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::OnScreen);
		mMeshBuffer.Render(effect, &mEffectContext, *camera);
	}
	void Unload()
	{
		mMeshBuffer.Terminate();
	}
private:
	NFGE::Graphics::Effects::OnScreenEffect::OnScreenEffectContext mEffectContext;
	NFGE::Graphics::MeshBuffer mMeshBuffer;
};