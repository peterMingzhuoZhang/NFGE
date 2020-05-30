#ifndef HELLOPLANTES_PLANTS_HEADER
#define HELLOPLANTES_PLANTS_HEADER

#include <Core\Inc\Core.h>
#include <Graphics\Inc\Graphics.h>
#include <NFGEMath/Inc/NFGEMath.h>

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Math;



struct PlanetMovment
{
	Vector3 SelfRotate;
	Vector3 SroundTarget;
	Vector3 SroundRotate;
	Vector3 SroundDistance;
};

class Planet
{
public:
	static Matrix4 GetTransformMatrix(const Transform3D& theTransform);
	static void TransfromControl(Transform3D& transfrom);

public:
	Planet() {}
	~Planet() {}

	void Load(const MeshPX& mesh, const wchar_t* textName);
	void Load(const MeshPC& mesh);
	void Unload();

	void Update(PlanetMovment movment);
	

	void Control()
	{
		Planet::TransfromControl(mTransfromInfo);
	}

	void Render(const Camera& camera, int windowWidth, int windowHeight);

//private:
	Transform3D mInitTransfromInfo;
	Transform3D mTransfromInfo;
	Matrix4 mMatrices[3];
	MeshBuffer mMeshBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
	ConstantBuffer mConstantBuffer;
	Texture mTexture;
	Sampler mSampler;

	Vector3 mPosition;
	float mRadius;
};


#endif // !HELLOPLANTES_PLANTS_HEADER
