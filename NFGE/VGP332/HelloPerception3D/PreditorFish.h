#ifndef HELLOPERCEPTION3D_PREDITORFISH_HEADER
#define HELLOPERCEPTION3D_PREDITORFISH_HEADER

#include "PTEntity_Shell.h"

struct PreditorFish : PTEntity_Shell
{
	RTGCN(PreditorFish);

	//- static ------------------------------------------------------
	static PTDynamicCone mGeometry;
	//---------------------------------------------------------------
	PreditorFish(AI::AIWorld3D& world);

	void Initialize(float width, float height, float depth);
	void Terminate();

	void OnUpdate(float deltaTime) override;

	void Render();

	void DebugDraw();

	void ResetPosition(const NFGE::Math::Vector3& pos);
	std::unique_ptr<AI::SteeringModule3D> steeringModule;
	
	float mDetectedRange = 200.0f;
	float mKillRange = 180.0f;

	std::unique_ptr<AI::StateMachine<PreditorFish>> mStateMachine;
};

#endif // !HELLOPERCEPTION3D_PREDITORFISH_HEADER
