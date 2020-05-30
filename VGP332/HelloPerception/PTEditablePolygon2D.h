//====================================================================================================
// Filename:	PTEditablePolygon2D.h
// Created by:	Mingzhuo Zhang
// Description:	It contain a structure <PTEditablePolygon2D> which can build and store a polygon of any shape
//				It contains :	---	Info of all vertices of polygon
//								---	Counters of Clockwise & CounterClockwise edge connection, it is helpful in
//									cutting poly in triangles.
//								--- A <Graph> of <PolygonMapNode>, it store any other set of all node in screen
//									for calculation about node that out side of the polygon
//								--- A <Graph> of <PolygonMapNode>, it store the set of node in the polygon
//								---	A <PTEditablePolygon2D>, that can build a any shape polygon and store
//									Info of its vertices.
//								--- A std::vector of <Triangle> for Shai Xuan out the node in polygon 
//								--- A std::vector of <LineSegment> for store the wall info for AI::World
//								--- A std::vector of <Circle> for store the Obstacle info for AI::World
//====================================================================================================

#ifndef HELLOPERCEPTION_PTEDITABLEPOLYGON2D_H
#define HELLOPERCEPTION_PTEDITABLEPOLYGON2D_H

#include "EngineWrapper.h"
using namespace EngineWrapper;

struct PTEditablePolygon2D
{
	bool mIsBuilt = false;
	size_t mSides{};
	std::vector<Vector2> mVertices{};
	int mClockwiseCount = 0;
	int mCounterClockwiseCount = 0;

	void DebugUIUpdate(const Camera2D& camera, float deltaTime);

	bool IsLastLineSegmentInterceting();
	bool IsIntercet(const LineSegment& line);
	bool BuildingPolygon(const Vector2& newVertice);
	Vector2 GetVertices(int index) const { return mVertices[index % mSides]; }

private:
	void FinishBuild();
	void ResetBack();	// reset current adding index back for 1  position
	void UpdateOrientationCount(const Vector2& v0, const Vector2& v1, const Vector2& v2, int howMany);

};

#endif // !HELLOPERCEPTION_PTEDITABLEPOLYGON2D_H
