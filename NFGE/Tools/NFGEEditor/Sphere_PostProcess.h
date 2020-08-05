#ifndef NFGEEDITOR_POSTPROCESSSPHERER_HEADER
#define NFGEEDITOR_POSTPROCESSSPHERER_HEADER

#include "NFGE/Inc/NFGE.h"
using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

struct MySphere
{
	MeshBuffer mMeshBuffer;
	float mRadius;
	virtual void Load(Vector3 position, int row, int col, float mRadius) = 0;
	virtual void Unload()
	{
		mMeshBuffer.Terminate();
	}
	virtual void Render(const Camera& camera)
	{
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
		mMeshBuffer.Render();
	}
};

struct MySpherePC : MySphere
{
	MeshPC mesh;
	NFGE::Graphics::Effects::ColorMeshEffect::ColorMeshEffectContext mEffectContext;
	void Load(Vector3 position, int row, int col, float mRadius) override
	{
		mEffectContext.position = position;
		mesh = MeshBuilder::CreateSpherePC(row, col, mRadius);

		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();
	}
	void Unload()
	{
		MySphere::Unload();
	}
	void Render(const Camera& camera)
	{
		auto effectManager = EffectManager::Get();
		Effect* effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::ColorMesh);

		effectManager->RegisterRenderObject(effect, &mEffectContext, &mMeshBuffer, camera);
	}
};
struct MySpherePN : MySphere
{
	Mesh mesh;
	Material mMaterial;

	NFGE::Graphics::Effects::StandardMeshEffect::StandardMeshEffectContext mEffectContext;

	void Load(Vector3 position, int row, int col, float mRadius) override
	{
		mEffectContext.light = &NFGE::sApp.GetMainLight();
		mEffectContext.position = position;
		mesh = MeshBuilder::CreateSphere(row, col, mRadius);

		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();

		mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
		mMaterial.diffuse = { 0.8f, 0.7f, 0.1f, 1.0f };
		mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
		mMaterial.power = 10.0f;

		mEffectContext.material = mMaterial;


	}
	void Unload()
	{
		MySphere::Unload();

	}
	void Render(const Camera& camera)
	{
		auto effectManager = EffectManager::Get();
		Effect* effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::StandardMesh);

		effectManager->RegisterRenderObject(effect, &mEffectContext, &mMeshBuffer, camera);
	}
};
struct MySpherePX : MySphere
{
	MeshPX mesh;
	TextureId diffuseTexture;
	NFGE::Graphics::Effects::TextureMeshEffect::TextureMeshEffectContext mEffectContext;

	bool IsSkyDome = false;

	void Load(Vector3 position, int row, int col, float mRadius, std::filesystem::path textureName)
	{
		Load(position, row, col, mRadius);
		mEffectContext.textureId = TextureManager::Get()->LoadTexture(textureName);
		mEffectContext.rasterizerState = "CullFrontSolid";
	}
	void Load(Vector3 position, int row, int col, float mRadius) override
	{
		mEffectContext.position = position;
		mesh = MeshBuilder::CreateSpherePX(row, col, mRadius);

		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();

		mEffectContext.rasterizerState = (IsSkyDome ? "CullFrontSolid" : "Solid");
	}
	void Unload()
	{
		MySphere::Unload();
	}
	void Render(const Camera& camera)
	{
		if (IsSkyDome)
			mEffectContext.position = camera.GetPosition();
		auto effectManager = EffectManager::Get();
		Effect* effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::TextureMesh);

		effectManager->RegisterRenderObject(effect, &mEffectContext, &mMeshBuffer, camera);
	}
};

struct SphereStandard : MySphere
{
	Mesh mesh;
	NFGE::Graphics::Effects::StandardMeshEffect::StandardMeshEffectContext mEffectContext;

	Material mMaterial;
	TextureId mDiffuseTexture;
	TextureId mSpecularTexture;
	TextureId mDisplacementTexture;
	float mBumpWeight;
	TextureId mNormalTexture;

	void Load(Vector3 position, int row, int col, float mRadius, std::filesystem::path textureName, std::filesystem::path textureName2, std::filesystem::path textureName3, std::filesystem::path textureName4)
	{
		mEffectContext.position = position;
		mesh = MeshBuilder::CreateSphere(row, col, mRadius);
		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();

		mEffectContext.AddTexture(MeshTextureMaterial::DIFFUSE, TextureManager::Get()->LoadTexture(textureName));
		mEffectContext.AddTexture(MeshTextureMaterial::SPECULAR, TextureManager::Get()->LoadTexture(textureName2));
		mEffectContext.AddTexture(MeshTextureMaterial::DISPLACEMENT, TextureManager::Get()->LoadTexture(textureName3));
		mEffectContext.bumpWeight = 10.0f;
		mEffectContext.AddTexture(MeshTextureMaterial::NORMALS, TextureManager::Get()->LoadTexture(textureName4));

		mEffectContext.light = &NFGE::sApp.GetMainLight();

		mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
		mMaterial.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
		mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
		mMaterial.power = 10.0f;

		mEffectContext.material = mMaterial;

	}
	void Load(Vector3 position, int row, int col, float mRadius) override
	{

	}
	void Render(const Camera& camera)
	{
		auto effectManager = EffectManager::Get();
		Effect* effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::StandardMesh);

		effectManager->RegisterRenderObject(effect, &mEffectContext, &mMeshBuffer, camera);
	}

	void Unload()
	{
		MySphere::Unload();
	}
};

#endif // !HELLOPOSTPROCESSINGADVANCE_MYSPHERE_HEADER



