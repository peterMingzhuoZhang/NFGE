//====================================================================================================
// Filename:	Carrier.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <Carrier> inherient from <AI::Agent>
//====================================================================================================

#ifndef HELLOPERCEPTION_CARRIER_H
#define HELLOPERCEPTION_CARRIER_H

#include "Mineral.h"

struct PolygonGridGraphMap;

class Carrier : public AI::Agent
{
public:
	RTGCN(Carrier)

public:
	Carrier(AI::AIWorld& world);

	void Initialize(PolygonGridGraphMap* thePolygonMap);
	void Update(float deltaTime);
	void Render();

	void ResetPosition(const AI::Graph<PolygoneMapNode>& theGraphMap);

public:
	//std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
private:
	//std::unique_ptr<AI::StateMachine<SCV>> mStateMachine;
	NFGE::Graphics::TextureId mTextureIds[32];
};

#endif // !HELLOPERCEPTION_CARRIER_H

