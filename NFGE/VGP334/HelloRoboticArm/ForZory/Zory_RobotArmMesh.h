#ifndef HELLOROBOTARM_ZORY_ROBOTARMMESH_H
#define HELLOROBOTARM_ZORY_ROBOTARMMESH_H

#include "NFGE/Inc/NFGE.h"



class Zory_RobotArmMesh
{
public:
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

	void Load();
	void Update(float deltaTime);
	void Render(const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera); 
	void UnLoad();

	NFGE::Math::Vector3	mChildSpwanPosition;					// Step 2
private:
	NFGE::Graphics::Mesh mMesh;
	NFGE::Graphics::MeshBuffer mMeshBuffer;
	
	NFGE::Graphics::Material mMaterial;
	MaterialBuffer mMaterialBuffer;

	NFGE::Math::Matrix4 mMatrices[3];
	TransformBuffer mTransformBuffer;

	NFGE::Graphics::VertexShader mVertexShader;
	NFGE::Graphics::PixelShader mPixelShader;

	NFGE::Math::Vector3 mPosition;
	NFGE::Math::Quaternion mRotation;
	NFGE::Math::Vector3 mScale;
};

#endif // !HELLOROBOTARM_ZORY_ROBOTARMMESH_H
