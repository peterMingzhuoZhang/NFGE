#ifndef HELLOLIGHTING_GAMESTATE_H
#define HELLOLIGHTING_GAMESTATE_H

#include <NFGE/Inc/NFGE.h>

using namespace NFGE::Graphics;

class GameState : public NFGE::AppState 
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	Camera mCamera;
	Mesh mMesh;
	MeshBuffer mMeshBuffer;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	struct TransformData
	{
		NFGE::Math::Matrix4 world;
		NFGE::Math::Matrix4 wvp;
		NFGE::Math::Vector3 viewPosition;
		float padding;
	};

	using TransformBuffer = TypedConstantBuffer<TransformData>;	
	using LightBuffer = TypedConstantBuffer<DirectionalLight>;
	using MaterialBuffer = TypedConstantBuffer<Material>;
	
	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;

	float mTurnSpeed = 0.1f;
	float mMoveSpeed = 10.0f;

	DirectionalLight mLight;
	Material mMaterial;
	NFGE::Math::Vector3 mPosition;
	TextureId mDiffuseMap;
	TextureId mSpecularMap;
};


#endif // !HELLOLIGHTING_GAMESTATE_H