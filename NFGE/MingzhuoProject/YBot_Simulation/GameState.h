#pragma once

#include "YBot.h"
#include "PTPlane.h"

using namespace NFGE;
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
	void CameraControl(Camera& camera);
	//void CameraFollow(Camera& camera);
	void ShowUI();
private:
	float mdeltaTime;

	Graphics::DirectionalLight* mLight;
	Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight> mLightBuffer;

	AI::AIWorld3D mWorld3D;
	Physics::PhysicsWorld mPhysicsWorld;
	//PTPlane mPlane;
	Graphics::Geometry::PlaneMesh mPlane;
	//YBot mYBot{ mWorld3D };
	std::vector<std::unique_ptr<YBot>> mYBots;

	int mCurrentControlIndex = -1;
};