#include "Mineral.h"
#include "PolygonGridGraphMap.h"
#include "GraphUtil.h"

void Mineral::Load(const NFGE::Math::Vector2& size)
{
	mTexture.Load(size, { 0.5f,0.5f }, "minerals.png");
}

void Mineral::Load(const NFGE::Math::Vector2& size, PolygonGridGraphMap* thePolygonMap)
{
	Load(size);
	ResetPosition(thePolygonMap->mPolygonGridGraph);
}

void Mineral::Unload()
{
}

void Mineral::Update(float deltaTime)
{
}

void Mineral::Render(const Camera2D & camera)
{
	mTexture.Render(position, camera, true);
}

void Mineral::ResetPosition(const AI::Graph<PolygoneMapNode>& theGraphMap)
{
	int nextNodeIndex = NFGE::Math::Random() % theGraphMap.GetNodeCount();
	position = theGraphMap.GetNode(nextNodeIndex).position;
}
