#ifndef HELLOPERCEPTION3D_PTGEOMETRY_SHELL_HEADER
#define HELLOPERCEPTION3D_PTGEOMETRY_SHELL_HEADER

#include <NFGE/Inc/NFGE.h>

// Using selfDefine type --------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------------

struct PTGemotry_Shell
{
	NFGE::Graphics::MeshPN mMesh;
	NFGE::Graphics::MeshBuffer mMeshBuffer;

	NFGE::Math::Matrix4 mMatrices[3];
	NFGE::Graphics::Material mMaterial;
	TransformBuffer mTransformBuffer;
	MaterialBuffer mMaterialBuffer;
	NFGE::Graphics::ConstantBuffer mConstantBuffer;

	NFGE::Graphics::VertexShader mVertexShader;
	NFGE::Graphics::PixelShader mPixelShader;

	NFGE::Math::Vector3 mPosition;
	NFGE::Math::Vector3 mRotation;

	void Update(float deltaTime);
	void Render(const NFGE::Math::Vector3 & pos, const NFGE::Math::Vector3& forward, const NFGE::Math::Vector3 & scale, const NFGE::Graphics::Camera & camera, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::DarkGray);
	void Terminate();
private:
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnTerminate();
protected:
	void BaseInitliaze(const wchar_t* vertexShader, const wchar_t* pixelShader);
};

#endif // !HELLOPERCEPTION3D_PTGEOMETRY_SHELL_HEADER
