//====================================================================================================
// Filename:	PolygonGridGraphMap.h
// Created by:	Mingzhuo Zhang
// Description:	It contain a structure <PolygonGridGraphMap> which can allow a grid of nodes in a Polygon
//				do pathfinding on eachother, also the polygon could be any shape in the screen. And also 
//				can build wall<Linsegment> & Obstacle<Circle> on the grid for later AI::World usage.
//				It contains :	---	Infomation of screen & screen size of node 
//								---	A <TextureGridMap> to store the over all Grid, and texture info for Future
//									update
//								--- A <Graph> of <PolygonMapNode>, it store any other set of all node in screen
//									for calculation about node that out side of the polygon
//								--- A <Graph> of <PolygonMapNode>, it store the set of node in the polygon
//								---	A <PTEditablePolygon2D>, that can build a any shape polygon and store
//									Info of its vertices.
//								--- A std::vector of <Triangle> for Shai Xuan out the node in polygon 
//								--- A std::vector of <LineSegment> for store the wall info for AI::World
//								--- A std::vector of <Circle> for store the Obstacle info for AI::World
//====================================================================================================

#ifndef HELLOOERCEOTION_POLYGONGRIDGRAPHMAP_HEADER
#define HELLOOERCEOTION_POLYGONGRIDGRAPHMAP_HEADER

#include "TextureGridMap.h"
#include "PTEditablePolygon2D.h"

struct PolygonGridGraphMap
{
	PolygonGridGraphMap() = default;
	~PolygonGridGraphMap() = default;

	//Members
	float mScreenWidth = 0.0f;
	float mScreenHeight = 0.0f;
	float mTileSize = 0.0f;

	TextureGridMap mOverAllGridMap;
	bool mIsBuilt = false;
	bool mIsInUse = false;
	bool mIsRenderGraph = true;
	bool mIsRenderRegionTriangle = true;
	size_t mIsAddingWall = 0;		// 0001 -> adding
									// 0010 -> editing first point of wall
									// 0100 -> editing second point of wall

	size_t mIsAddingObbs = 0;		// 0001 -> adding
									// 0010 -> editing obstacle position

	AI::Graph<PolygoneMapNode> mOverAllGraph;
	AI::Graph<PolygoneMapNode> mPolygonGridGraph;
	PTEditablePolygon2D mPolygon;
	std::vector<Triangle> mTrianglesInPoly;
	std::vector<LineSegment> mWalls;
	std::vector<Circle> mObstacles;

	//Functions
	void Load(float screenWidth, float screenHeight, float tileSize, const Vector2& initPos = Vector2{0.0f,0.0f});
	void Load(std::string fileName); //TODO: Load the polygonGraph directly from file
	void Update(float deltaTime);
	void Render(const Camera2D& camera);
	void DebugUI(const Camera2D& camera);
	void Unload();

	void AddingWall();
	void AddingObs();

	void BindTheAIWorld(AI::AIWorld& world);

	void FindPathInPoly(const Vector2& from, const Vector2& to);
	std::list<Vector2> GetPosPath();
private:

	void BuildingMap();
	void FinishBuildMap();
	void BuildWall(const LineSegment& wall);
	void RenderPolygon(const Camera2D& camera);
	void RenderWall(const Camera2D& camera);
	void RenderObs(const Camera2D& camera);
	void RenderTriangles(const Camera2D& camera);
	void RenderPolygonGraph(const Camera2D& camera);
	void OptmaisPolyGraphRender(const Camera2D& camera);
	struct CutPolyContext
	{
		std::list<Vector2> mVertices;
		std::list<Vector2>::iterator mCurrentVertice;
	};
	void MingzhuoPolyCut();
	void Cut(CutPolyContext& context);
	bool IsTriangleValid(const Triangle& tri, CutPolyContext& context, std::list<Vector2>::iterator cutPoint);
	bool IsWallValid(const LineSegment& wall);
	bool IsIntercetTriangles(const LineSegment& line);
	bool IsPointInTriangle(const Vector2& point, const Triangle& tri);
	bool IsAnyPointsInTriangle(const std::list<Vector2>& points, const Triangle& tri);

	void BuildPolygonGraph();

	int ConvertToPolygonGraphIndex(const Vector2& position) const;
	int ClosestPolygonGraphIndex(const Vector2& position);

	void IsolateNode(size_t index);
};

#endif // !HELLOOERCEOTION_POLYGONGRIDGRAPHMAP_HEADER
