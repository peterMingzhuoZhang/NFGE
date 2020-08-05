#pragma once
#include <NFGE/Inc/NFGE.h>
#include "PTTentacle.h"

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
	
	NFGE::GameObject groundPlane;
	NFGE::GameObject skyDom;
	std::vector<PTTentacle> grasses;
	
	NFGE::Physics::PhysicsWorld mPhysicsWorld;
	NFGE::Math::OBB* mCollisionOBB;

private:
	void CameraControl(Camera& camera,float deltaTime);
	void OBBControl();

};

