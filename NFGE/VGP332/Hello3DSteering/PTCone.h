#ifndef HELLO3DGROUPBEHAVIOR_CONE_HEADER
#define HELLO3DGROUPBEHAVIOR_CONE_HEADER

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

struct PTCone
{
	void Initialize(int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader);
	void Terminate();
	void Update();
	void Render(const NFGE::Math::Vector3& pos, const NFGE::Math::Matrix4 & rot, const NFGE::Math::Vector3 & scale, const NFGE::Graphics::Camera& cmaera);

	bool mIsInitialize = false;

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
	NFGE::Math::Matrix4 mRotation;
	float mRadius;

};

#endif // !HELLO3DGROUPBEHAVIOR_SPHERE_HEADER
