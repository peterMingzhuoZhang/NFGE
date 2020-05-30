#pragma once

#include "Sphere_PostProcess.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

class EditState : public NFGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void RenderScene();

private:
	//Camera camera;
	Camera * mCamera = nullptr;

	MySpherePC spherePC{};
	MySpherePN spherePN{};
	MySpherePX spherePX{};
	SphereStandard sphereStandard{};
	MySpherePX sphereSkydome{};

	NFGE::Graphics::DirectionalLight mLight;
	NFGE::Graphics::Material mMaterial;

	NFGE::World mWorld;
	NFGE::Editor mEditor{ mWorld };

};
