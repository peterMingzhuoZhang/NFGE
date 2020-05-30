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
	Camera camera;

	MySpherePC spherePC{};
	MySpherePN spherePN{};
	MySpherePX spherePX{};
	SphereStandard sphereStandard{};
	MySpherePX sphereSkydome{};

	NFGE::Graphics::DirectionalLight mLight;
	NFGE::Graphics::Material mMaterial;
};

#endif // !HELLOLIGHTING_GAMESTATE_HEADER
