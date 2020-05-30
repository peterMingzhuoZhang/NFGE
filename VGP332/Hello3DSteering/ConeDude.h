#ifndef HELLOGROUPBEHAVIOR_CONEDUDE_H
#define HELLOGROUPBEHAVIOR_CONEDUDE_H

#include "PTCone.h"

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
struct ConeDude : public AI::Agent3D
{
	//- static ------------------------------------------------------
	static PTCone mCone;
	//---------------------------------------------------------------

	ConeDude(AI::AIWorld3D& world);

	void Initialize(float width, float height, float depth);
	void Terminate();

	void Update(const BehaviorControl& behaviorControl, float deltaTime);

	void Render();

	void DebugDraw(const NFGE::Math::Vector2& leftPoint, const NFGE::Math::Vector2& rightPoint);

	std::unique_ptr<AI::SteeringModule3D> steeringModule;
	NFGE::Graphics::Color mColor = NFGE::Graphics::Colors::Green;

	float worldWidth;
	float worldHeight;
	float worldDepth;
	//float mRadiue;					//AI::Agent Has this member
	bool mIsDebugDraw = false;

private:
};

#endif // !HELLOGROUPBEHAVIOR_TRIANGLEDUDE
