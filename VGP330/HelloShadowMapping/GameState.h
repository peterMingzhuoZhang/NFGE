#pragma once
#include <NFGE/Inc/NFGE.h>
#include "Tank.h"
#include "PTPlane.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

class GameState : public NFGE::AppState
{
	
	using DepthConstantBuffer = TypedConstantBuffer<Matrix4>;
	using ShadowConstantBuffer = TypedConstantBuffer<Matrix4>;
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void RenderScene();
	void PostProcess();
	void DrawDepthMap();
private:
	Camera* mActiveCamera;
	Camera mLightCamera;
	Camera mDebugCamera;
	Vector3 mTankInitPosition;
	AI::AIWorld3D world;

	Tank tank{world};

	std::vector<Tank> mTanks;
	float spacing = 0.0f;

	Model spaceShip;
	PTPlane mPlane;

	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;
	VertexShader mPostProcessVertexShader;
	PixelShader mPostProcessPixelShader;
	NFGE::Graphics::RenderTarget mRenderTarget;

	//Shadow
	NFGE::Graphics::RenderTarget mDepthMapRanderTarget;
	NFGE::Graphics::VertexShader mDepthVertexShaderTarget;
	NFGE::Graphics::PixelShader mDepthPixelShaderTarget;
	DepthConstantBuffer mDepthConstantBuffer;
	

	NFGE::Graphics::DirectionalLight mLight;
	NFGE::Graphics::Material mMaterial;


	NFGE::Math::Matrix4 mLightMatView;
	NFGE::Math::Matrix4 mLightMatProj;
	std::vector<Vector3> mViewFrustumVertices;
};