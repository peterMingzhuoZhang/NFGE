#ifndef HELLOLIGHTING_MYSPHERE_HEADER
#define HELLOLIGHTING_MYSPHERE_HEADER

#include "NFGE/Inc/NFGE.h"
using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

struct MySphere
{
	Matrix4 mMatrices[3];
	MeshBuffer mMeshBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
	ConstantBuffer mConstantBuffer;

	Vector3 mPosition;
	Vector3 mRotation;
	float mRadius;
	virtual void Load(Vector3 position, int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader) = 0;
	virtual void Unload()
	{
		mMeshBuffer.Terminate();
		mVertexShader.Terminate();
		mPixelShader.Terminate();
		mConstantBuffer.Terminate();

	}
	virtual void Render(const Camera& camera)
	{
		mVertexShader.Bind();
		mMatrices[0].Identity();
		mMatrices[0] = Transpose(mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));
		mMatrices[1] = Transpose(camera.GetViewMatrix());
		mMatrices[2] = Transpose(camera.GetPerspectiveMatrix());
		mConstantBuffer.Load(mMatrices);
		mConstantBuffer.BindVS();
		mPixelShader.Bind();
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
		mMeshBuffer.Render();
	}
};

struct MySpherePC : MySphere
{
	MeshPC mesh;
	void Load(Vector3 position, int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader) override
	{
		mPosition = position;
		mesh = MeshBuilder::CreateSpherePC(row, col, mRadius);

		mMatrices[0].Identity();
		mMatrices[1].Identity();
		mMatrices[2].Identity();
		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();
		mVertexShader.Initialize(vertexShader, VertexPC::Format);
		mPixelShader.Initialize(pixelShader);
		mConstantBuffer.Initialize(3 * sizeof(Matrix4));
	}
	void Unload()
	{
		MySphere::Unload();
	}
	void Render(const Camera& camera)
	{
		MySphere::Render(camera);
	}
};
struct MySpherePN : MySphere
{
	MeshPN mesh;
	DirectionalLight mLight;
	Material mMaterial;
	
	struct TransformData
	{
		NFGE::Math::Matrix4 world;
		NFGE::Math::Matrix4 wvp;
		NFGE::Math::Vector3 viewPositon;
		float padding;
	};
	using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight>;
	using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;

	void Load(Vector3 position, int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader) override
	{
		mPosition = position;
		mesh = MeshBuilder::CreateSpherePN(row, col, mRadius);

		mMatrices[0].Identity();
		mMatrices[1].Identity();
		mMatrices[2].Identity();
		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();
		mVertexShader.Initialize(vertexShader, VertexPN::Format);
		mPixelShader.Initialize(pixelShader);
		
		mTransformBuffer.Initialize();
		mLightBuffer.Initialize();
		mMaterialBuffer.Initialize();

		mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,1.0f });
		mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
		mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
		mLight.specular = { 0.1f, 0.1f, 0.1f, 1.0f };

		mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
		mMaterial.diffuse = { 0.8f, 0.7f, 0.1f, 1.0f };
		mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
		mMaterial.power = 10.0f;

	}
	void Unload()
	{
		mMaterialBuffer.Terminate();
		mLightBuffer.Terminate();
		mTransformBuffer.Terminate();
		MySphere::Unload();

	}
	void Render(const Camera& camera)
	{
		//MySphere::Render(camera);
		mVertexShader.Bind();
		mPixelShader.Bind();

		mMatrices[0].Identity();
		mMatrices[0] = (mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));
		mMatrices[1] = (camera.GetViewMatrix());
		mMatrices[2] = (camera.GetPerspectiveMatrix());

		TransformData transformData;
		transformData.world = Transpose(mMatrices[0]);
		transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
		transformData.viewPositon = camera.GetPosition();
		mTransformBuffer.Load(transformData);
		mTransformBuffer.BindVS(0);

		mLightBuffer.Load(mLight);
		mLightBuffer.BindVS(1);

		mMaterialBuffer.Load(mMaterial);
		mMaterialBuffer.BindVS(2);

		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		mMeshBuffer.Render();
	}
};
struct MySpherePX : MySphere
{
	MeshPX mesh;
	TextureId diffuseTexture;

	bool IsSkyDome = false;

	void Load(Vector3 position, int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader, std::filesystem::path textureName)
	{
		mPosition = position;
		Load(position, row, col, mRadius, vertexShader, pixelShader);
		diffuseTexture = TextureManager::Get()->LoadTexture(textureName);
	}
	void Load(Vector3 position, int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader) override
	{
		mPosition = position;
		mesh = MeshBuilder::CreateSpherePX(row, col, mRadius);

		mMatrices[0].Identity();
		mMatrices[1].Identity();
		mMatrices[2].Identity();
		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();
		mVertexShader.Initialize(vertexShader, VertexPX::Format);
		mPixelShader.Initialize(pixelShader);
		mConstantBuffer.Initialize(3 * sizeof(Matrix4));
		mConstantBuffer.Initialize(3 * sizeof(Matrix4));
	}
	void Unload()
	{
		MySphere::Unload();
	}
	void Render(const Camera& camera)
	{
		if (IsSkyDome)
			mPosition = camera.GetPosition();

		mVertexShader.Bind();
		mMatrices[0].Identity();
		mMatrices[0] = Transpose(mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));
		mMatrices[1] = Transpose(camera.GetViewMatrix());
		mMatrices[2] = Transpose(camera.GetPerspectiveMatrix());
		mConstantBuffer.Load(mMatrices);
		mConstantBuffer.BindVS();
		mPixelShader.Bind();

		if (IsSkyDome)
			RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid")->Set();
		else
			RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		//mTexture.BindPS();
		//mSampler.BindPS();
		TextureManager::Get()->GetTexture(diffuseTexture)->BindPS();
		SamplerManager::Get()->GetSampler("PointClamp")->BindPS();
		mMeshBuffer.Render();
	}
};

struct SphereStandard : MySphere
{
	Mesh mesh;
	DirectionalLight mLight;
	Material mMaterial;
	TextureId mDiffuseTexture;
	TextureId mSpecularTexture;
	TextureId mDisplacementTexture;
	float mBumpWeight;
	TextureId mNormalTexture;

	struct TransformData
	{
		NFGE::Math::Matrix4 world;
		NFGE::Math::Matrix4 wvp;
		NFGE::Math::Vector3 viewPositon;
		float padding;
	};
	struct OptionsData
	{
		float displacementWeight;
		float padding[3];
	};
	using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight>;
	using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;
	using OptionBuffer = NFGE::Graphics::TypedConstantBuffer<OptionsData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionBuffer mOptionDateBuffer;

	void Load(Vector3 position, int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader, std::filesystem::path textureName, std::filesystem::path textureName2,std::filesystem::path textureName3, std::filesystem::path textureName4)
	{
		mPosition = position;
		mDiffuseTexture = TextureManager::Get()->LoadTexture(textureName);

		mesh = MeshBuilder::CreateSphere(row, col, mRadius);

		mMatrices[0].Identity();
		mMatrices[1].Identity();
		mMatrices[2].Identity();
		mMeshBuffer.Initialize(mesh);
		mMeshBuffer.SetTopology();
		mVertexShader.Initialize(vertexShader, Vertex::Format);
		mPixelShader.Initialize(pixelShader);

		mTransformBuffer.Initialize();
		mLightBuffer.Initialize();
		mMaterialBuffer.Initialize();
		mOptionDateBuffer.Initialize();

		mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,1.0f });
		mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
		mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
		mLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };

		mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
		mMaterial.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
		mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
		mMaterial.power = 10.0f;

		mDiffuseTexture = TextureManager::Get()->LoadTexture(textureName);
		mSpecularTexture = TextureManager::Get()->LoadTexture(textureName2);
		mDisplacementTexture = TextureManager::Get()->LoadTexture(textureName3);
		mBumpWeight = 10.0f;
		mNormalTexture = TextureManager::Get()->LoadTexture(textureName4);
	}
	void Load(Vector3 position, int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader) override
	{

	}
	void Render(const Camera& camera)
	{
		//MySphere::Render(camera);
		mVertexShader.Bind();
		mPixelShader.Bind();
		
		TextureManager::Get()->GetTexture(mDiffuseTexture)->BindPS();
		TextureManager::Get()->GetTexture(mSpecularTexture)->BindPS(1);
		TextureManager::Get()->GetTexture(mDisplacementTexture)->BindVS(2);
		TextureManager::Get()->GetTexture(mNormalTexture)->BindPS(3);
		SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		mMatrices[0].Identity();
		Matrix4 rotX, rotY,rotZ;
		rotX.Identity();
		rotY.Identity();
		rotZ.Identity();
		
		(rotX.RotationX(mRotation.x));
		(rotY.RotationY(mRotation.y));
		(rotZ.RotationZ(mRotation.z));
		mMatrices[0] = (rotX *rotY * rotZ * mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));

		mMatrices[1] = (camera.GetViewMatrix());
		mMatrices[2] = (camera.GetPerspectiveMatrix());

		TransformData transformData;
		transformData.world = Transpose(mMatrices[0]);
		transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
		transformData.viewPositon = camera.GetPosition();
		mTransformBuffer.Load(transformData);
		mTransformBuffer.BindVS(0);
		mTransformBuffer.BindPS(0);

		mLightBuffer.Load(mLight);
		mLightBuffer.BindVS(1);
		mLightBuffer.BindPS(1);

		mMaterialBuffer.Load(mMaterial);
		mMaterialBuffer.BindVS(2);
		mMaterialBuffer.BindPS(2);

		OptionsData optData;
		optData.displacementWeight = mBumpWeight;
		mOptionDateBuffer.Load(optData);
		mOptionDateBuffer.BindVS(3);

		mMeshBuffer.Render();
	}
	void Unload()
	{
		mTransformBuffer.Terminate();
		mLightBuffer.Terminate();
		mMaterialBuffer.Terminate();
		MySphere::Unload();
	}
};

#endif // !HELLOLIGHTING_MYSPHERE_HEADER



