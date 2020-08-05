//====================================================================================================
// Filename:	SCV.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <SCV> inherient from <AI::Agent>
//====================================================================================================

#ifndef HELLOPERCEPTION_SCV_H
#define HELLOPERCEPTION_SCV_H

#include "Mineral.h"

struct PolygonGridGraphMap;

class SCV : public AI::Agent
{
public:
	SCV(AI::AIWorld& world);

	void Initialize( PolygonGridGraphMap* polyMap);
	void Update(float deltaTime);
	void Render();

	void ResetPosition(const AI::Graph<PolygoneMapNode>& theGraphMap);
	bool TryCollectMinaral(Mineral& theMineral);
public:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	PolygonGridGraphMap* mPolyMap;

	int mValue = 0;

private:
	std::unique_ptr<AI::StateMachine<SCV>> mStateMachine;
	NFGE::Graphics::TextureId mTextureIds[16];
};

#endif // !HELLOPERCEPTION_SCV_H