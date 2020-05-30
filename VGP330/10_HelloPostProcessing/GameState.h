#ifndef HELLOLIGHTING_GAMESTATE_HEADER
#define HELLOLIGHTING_GAMESTATE_HEADER

#include "MySphere.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

class GameState : public NFGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void RenderScene();
	void PostProcess();
private:
	Camera camera;

	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;
	VertexShader mPostProcessVertexShader;
	PixelShader mPostProcessPixelShader;


	MySpherePC spherePC{};
	MySpherePN spherePN{};
	MySpherePX spherePX{};
	SphereStandard sphereStandard{};
	MySpherePX sphereSkydome{};

	NFGE::Graphics::DirectionalLight mLight;
	NFGE::Graphics::Material mMaterial;

	NFGE::Graphics::RenderTarget mRenderTarget;
};

#endif // !HELLOLIGHTING_GAMESTATE_HEADER
