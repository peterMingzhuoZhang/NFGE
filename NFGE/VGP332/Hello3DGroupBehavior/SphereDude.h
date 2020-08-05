#ifndef HELLOGROUPBEHAVIOR_TRIANGLEDUDE_H
#define HELLOGROUPBEHAVIOR_TRIANGLEDUDE_H

#include "PTSphere.h"
using namespace NFGE;
// Using selfDefine type --------------------------------------------------------------------------------------------------
struct BehaviorControl
{
	float mSeparationWeight = 1.0f;
	float mAlignmentWeight = 1.0f;
	float mCohesionWeight = 1.0f;

	float mWanderWeight = 1.0f;
	float mObsAvoidWeight = 1.0f;
	float mNeighborRadius = 100.0f;
};
//--------------------------------------------------------------------------------------------------------------------------
struct SphereDude : public AI::Agent
{
	//- static ------------------------------------------------------
	static PTSphere mSphere;
	//---------------------------------------------------------------

	SphereDude(AI::AIWorld& world);

	void Initialize(float width, float height);
	void Terminate();
	
	void Update(const BehaviorControl& behaviorControl, float deltaTime);
	
	void Render();

	void DebugDraw(const NFGE::Math::Vector2& leftPoint, const NFGE::Math::Vector2& rightPoint);

	std::unique_ptr<AI::SteeringModule> steeringModule;
	NFGE::Graphics::Color mColor = NFGE::Graphics::Colors::Green;

	float worldWidth;
	float worldHeight;
	float mDebugLineHeight = 10.0f;
	//float mRadiue;					//AI::Agent Has this member
	bool mIsDebugDraw = false;

private:
	void Render_2D();
};

#endif // !HELLOGROUPBEHAVIOR_TRIANGLEDUDE
