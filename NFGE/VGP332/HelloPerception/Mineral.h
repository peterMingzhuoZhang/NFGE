//====================================================================================================
// Filename:	Mineral.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <Mineral> inherient from <AI::Entity>
//====================================================================================================

#ifndef HELLOPERCEPTION_MINERAL
#define	 HELLOPERCEPTION_MINERAL

#include "LCTexture.h"
#include "EngineWrapper.h"

struct PolygonGridGraphMap;
struct PolygoneMapNode;

struct Mineral : public AI::Entity
{
	RTGCN(Mineral)

	int value = 10;
	LCTexture mTexture;

	void Load(const NFGE::Math::Vector2& size);
	void Load(const NFGE::Math::Vector2& size, PolygonGridGraphMap* thePolygonMap);
	void Unload();
	void Update(float deltaTime);
	void Render(const Camera2D& camera);

	void ResetPosition(const AI::Graph<PolygoneMapNode>& theGraphMap);

};

#endif // !HELLOPERCEPTION_MINERAL