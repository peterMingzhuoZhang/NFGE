#include "PTPartical.h"

void PTParticleEmitter::Load()
{
	mConstantBuffer.Initialize();

	mVertexShader.Initialize(L"../../Assets/Shaders/Particle.fx", Graphics::VertexPNC::Format);	// [NEW STUFF ADDED]: Add a <VertexPNC> struct in to VertexType.h
	mGeometryShader.Initialize(L"../../Assets/Shaders/Particle.fx");
	mPixelShader.Initialize(L"../../Assets/Shaders/Particle.fx");

	mTextureId = Graphics::TextureManager::Get()->LoadTexture("sword.png");
	mParticles = std::vector<Particle>(mParticlesCount);
	mMeshBuffer.Initialize(nullptr, static_cast<uint32_t>(mParticles.size()), sizeof(Graphics::VertexPNC), true);	    // [CAREFUL] the paramaters order may be different on different engine
	mCurrentBlendState = "Opaque";
}

void PTParticleEmitter::Unload()
{
	mMeshBuffer.Terminate();
	mPixelShader.Terminate();
	mGeometryShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();

}

void PTParticleEmitter::Update(float deltaTime)
{
	for (auto& p : mParticles)
	{
		if (p.lifeTime > 0.0f)
		{
			p.lifeTime -= deltaTime;
			p.velocity.y += deltaTime;
			p.position += p.velocity * deltaTime;
		}
	}
}

void PTParticleEmitter::Render()
{
	auto& camera = NFGE::sApp.GetMainCamera();	// [NEW STUFF ADDED]: Update your App.h with Mingzhuo's App.h. (Make sure have this function)
	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();
	ConstantData data;
	data.wvp = Math::Transpose(view * projection);
	data.viewPosition = { camera.GetPosition(), 1.0f }; // [NEW STUFF ADDED]: Add a Constructor For Vector4, which take a vector3 and a float
														//					  you will have to include Vector3.h inside Vector4.h
	data.viewUp = Math::Vector3{ view._12, view._22, view._32 };

	mConstantBuffer.Load(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindGS();					//[NEW STUFF ADDED]: Make sure you have BindGS() in your ConstantBuffer.
												//					 It is similar with BindPS() & BindVS()
	mConstantBuffer.BindPS();
	mVertexShader.Bind();
	mGeometryShader.Bind();
	mPixelShader.Bind();

	NFGE::Graphics::BlendStateManager::Get()->GetBlendState(mCurrentBlendState)->Set();
	NFGE::Graphics::SamplerManager::Get()->GetSampler("AnisotropicWrap")->BindPS(0);
	NFGE::Graphics::TextureManager::Get()->GetTexture(mTextureId)->BindPS(0);

	std::vector<Graphics::VertexPNC> vertices;
	vertices.reserve(mParticles.size());
	for (auto& p : mParticles)
	{
		if (p.lifeTime > 0.0f)
		{
			float size = 1.0f - p.lifeTime / mMaxLifeTime;

			Graphics::VertexPNC vertex;
			vertex.position = p.position;
			vertex.normal.x = size;
			vertex.normal.y = size;
			vertex.color = Math::Vector4{
				Math::Lerp(endColor.x, startColor.x, size),					// [NEW STUFF ADDED]: Add Math::lerp for float into Math lib
				Math::Lerp(endColor.y, startColor.y, size),
				Math::Lerp(endColor.z, startColor.z, size),
				1.0f
			};
			vertices.push_back(vertex);
		}
	}
	mCurrentParticleCount = static_cast<int>(vertices.size());
	std::sort(vertices.begin(), vertices.end(), [&](auto& a, auto& b)
	{
		auto s = Math::Dot(a.position, camera.GetDirection());
		auto t = Math::Dot(b.position, camera.GetDirection());
		return s > t;
	});

	mMeshBuffer.Update(vertices.data(), static_cast<uint32_t>(vertices.size()));
	mMeshBuffer.SetTopology(Graphics::MeshBuffer::Topology::Points);
	mMeshBuffer.Render();

	mGeometryShader.UnBind();

}

void PTParticleEmitter::SpawnPartice()
{
	auto& p = mParticles[mNextParticle];
	p.position = Math::Vector3{ 0.0f };
	p.velocity = Math::Vector3{
		Math::RandomFloat(-1.0f, 1.0f),
		Math::RandomFloat(-1.0f, 1.0f),
		Math::RandomFloat(-1.0f, 1.0f)
	};
	p.lifeTime = mMaxLifeTime;
	mNextParticle = (mNextParticle + 1) % mParticles.size();
}


