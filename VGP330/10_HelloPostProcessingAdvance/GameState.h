#ifndef HELLOPOSTPROCESSINGADVANCE_GAMESTATE_HEADER
#define HELLOPOSTPROCESSINGADVANCE_GAMESTATE_HEADER

#include "Sphere_PostProcess.h"
#include "PostProcesser.h"

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
	void RenderSceneWithoutShader();
private:
	SphereStandard sphereStandard{};
	SphereStandard sphereStandard1{};
	SphereStandard sphereStandard2{};
	SphereStandard sphereStandard3{};
	SphereStandard sphereStandard4{};

	NFGE::Graphics::DirectionalLight mLight;
	NFGE::Graphics::Material mMaterial;

	bool mIsPostProcessOn = true;
	PostProcesser_DepthBlur mPostProcesser;

};

#endif // !HELLOPOSTPROCESSINGADVANCE_GAMESTATE_HEADER
