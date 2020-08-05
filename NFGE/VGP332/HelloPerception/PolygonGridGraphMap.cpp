#include "PolygonGridGraphMap.h"

size_t isAddingWallMask = 1 << 0;	//|
size_t isEditPointAMask = 1 << 1;	//|---- wall adding state
size_t isEditPointBMask = 1 << 2;	//|

size_t isAddingObsMask = 1 << 0;		//|
size_t isEditObsPosMask = 1 << 1;		//|---- obstacle adding state

float ObsRadiusChangeSpeed = 2.0f;

//-------------------------------------------------------------------------------------------------------------------------------------//|
PolygonGridGraphMap polygonMap;																										   //|
																																	   //|
struct GridMap_CanOpenNode_Djika																									   //|
{																																	   //|
	bool operator()(AI::GraphSearch::Context& context, size_t n)																	   //|
	{																																   //|
		if (context.closed[n])																										   //|
			return false;																											   //|
		if (polygonMap.mOverAllGridMap.IsWall(n))																					   //|
			return false;																											   //|
		return true;																												   //|
	}																																   //|
};																																	   //|
struct GridMap_CanOpenNode_DFSnBFS																									   //|
{																																	   //|
	bool operator()(AI::GraphSearch::Context& context, size_t n)																	   //|
	{																																   //|
		if (context.open[n])																										   //|
			return false;																											   //|
		if (polygonMap.mOverAllGridMap.IsWall(n))																					   //|--- polygonMap and its policies
			return false;																											   //|
		return true;																												   //|
	}																																   //|
};																																	   //|
struct GridMap_GetGCost																												   //|
{																																	   //|
	float operator()(AI::GraphSearch::Context& context, size_t parentIndex, size_t nodeIndex)										   //|
	{																																   //|
		if (parentIndex == AI::InvalidNode)																							   //|
			return 0.0f;																											   //|
																																	   //|
		size_t fromColumn = parentIndex % polygonMap.mOverAllGridMap.col;															   //|
		size_t fromRow = parentIndex / polygonMap.mOverAllGridMap.col;																   //|
		size_t toColume = nodeIndex % polygonMap.mOverAllGridMap.col;																   //|
		size_t toRow = nodeIndex / polygonMap.mOverAllGridMap.col;																	   //|
		float currentGCost = polygonMap.mOverAllGridMap.allTiles[polygonMap.mOverAllGridMap.gridGraph.GetNode(nodeIndex).tileType].walkThroughCost;
		return context.g[parentIndex] + ((fromColumn != toColume && fromRow != toRow) ? 1.414f*currentGCost : 1.0f*currentGCost);	   //|
																																	   //|
	}																																   //|
};																																	   //|
struct GridMap_GetHCost																												   //|
{																																	   //|
	float operator()(AI::GraphSearch::Context& context, size_t nodeIndex)															   //|
	{																																   //|
		NFGE::Math::Vector2 fromPoint = polygonMap.mPolygonGridGraph.GetNode(nodeIndex).position;										   //|
		NFGE::Math::Vector2 toPoint = polygonMap.mPolygonGridGraph.GetNode(context.end).position;										   //|
																																	   //|
		return  NFGE::Math::Distance(fromPoint, toPoint);																				   //|
	}																																   //|
};																																	   //|
																																	   //|
//-------------------------------------------------------------------------------------------------------------------------------------//|

template <typename GraphType, typename OpenPolicy, typename NextPolicy, typename ValidationPolicy>
struct CustomSearch_fluidFill
{
	CustomSearch_fluidFill(const GraphType& graph)
		:graph(graph)
	{}

	void Execute(AI::GraphSearch::Context& context)
	{
		if (!context.open[context.start])
		{
			Open(context, context.start, AI::InvalidNode);
		}
		while (!context.found && !context.openList.empty())
		{
			size_t nodeIndex = Next(context);

			if (graph.GetNode(nodeIndex).inPolygonIndex != AI::InvalidNode)
			{
				context.found = true;
			}
			else
			{
				auto& node = graph.GetNode(nodeIndex);
				for (auto& n : node.neighbors)
				{
					if (IsValid(context, n))
					{
						Open(context, n, nodeIndex);
					}
				}
			}

			context.closedList.push_back(nodeIndex);
			context.closed[nodeIndex] = true;
		}
	}

	const GraphType& graph;
	OpenPolicy Open;
	NextPolicy Next;
	ValidationPolicy IsValid;
};

void PolygonGridGraphMap::Load(float screenWidth, float screenHeight, float tileSize, const Vector2& initPos)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mTileSize = tileSize;

	mOverAllGridMap.initPosition = initPos; //+ Vector2{tileSize, tileSize};
	mOverAllGridMap.graphMaxArea = { screenWidth, screenHeight };
	mOverAllGridMap.col = (int)(screenWidth / tileSize);
	mOverAllGridMap.row = (int)(screenHeight/ tileSize);
	mOverAllGridMap.Load();

	mOverAllGraph.Initialize(mOverAllGridMap.gridGraph.GetNodeCount());
	for (int i = 0; i < static_cast<int>(mOverAllGraph.GetNodeCount()); ++i)
	{
		auto& currentNode = mOverAllGraph.GetNode(i);
		currentNode = PolygoneMapNode(mOverAllGridMap.gridGraph.GetNode(i));
		currentNode.worldGraphIndex = i;
	}

	mPolygon = PTEditablePolygon2D{};
	mIsBuilt = false;
}

void PolygonGridGraphMap::Update(float deltaTime)
{
	if (mIsBuilt == false)
	{
		BuildingMap();
		return;
	}

}

void PolygonGridGraphMap::Render(const Camera2D & camera)
{
	if (mIsBuilt == false)
	{
		//mOverAllGridMap.Render(camera);
		RenderPolygonGraph(camera);
		RenderPolygon(camera);
		return;
	}


	if (mIsRenderRegionTriangle)
		RenderTriangles(camera);
	if (mIsRenderGraph)
		RenderPolygonGraph(camera);
	
	RenderPolygon(camera);
	RenderWall(camera);
	RenderObs(camera);
}

void PolygonGridGraphMap::DebugUI(const Camera2D & camera)
{
	ImGui::Begin("Polygon Graph");
	ImGui::Checkbox("Toggle Triangle Region##PolygonGridGraph", &mIsRenderRegionTriangle);
	ImGui::Checkbox("Toggle Graph##PolygonGridGraph", &mIsRenderGraph);
	
	if ((mIsBuilt)&&(!mIsInUse))
	{
		if ((mIsAddingWall & (isAddingWallMask + isEditPointAMask + isEditPointBMask)) == 0)
		{
			if (ImGui::Button("Adding a Wall##PolygonGridGraph"))
			{
				mIsAddingWall = isAddingWallMask;
			}
		}
		else
		{
			ImGui::Text("[Choose Two Points to add A wall]");
			ImGui::Text("[Right button to remove]");
			ImGui::End();
			return;
		}

		if ((mIsAddingObbs & (isAddingObsMask + isEditObsPosMask)) == 0)
		{
			if (ImGui::Button("Adding a Obs##PolygonGridGraph"))
			{
				mIsAddingObbs = isAddingObsMask;
			}
		}
		else
		{
			ImGui::Text("[Choose A Points to add A Obs]");
			ImGui::Text(" -- <- [Q].Radius.[E] -> ++");
			ImGui::Text("[Right button to remove]");
			ImGui::End();
			return;
		}
	}

	ImGui::End();
}

void PolygonGridGraphMap::Unload()
{
}

void PolygonGridGraphMap::BuildingMap()
{
	Vector2 mousePos = { (float)EngineWrapper::GetMouseScreenX(), (float)EngineWrapper::GetMouseScreenY() };
	if (mPolygon.mVertices.empty())
		mPolygon.mVertices.push_back(mousePos);

	int currentOnTopNodeIndex = mOverAllGridMap.ConvertToMapIndex(mousePos);
	Vector2 newVertices = mOverAllGridMap.gridGraph.GetNode(currentOnTopNodeIndex).position;
	mIsBuilt = mPolygon.BuildingPolygon(newVertices);

	if (mIsBuilt)
	{
		//TODO cutpolygon and build the graph
		MingzhuoPolyCut();
		BuildPolygonGraph();
	}
	

}

void PolygonGridGraphMap::FinishBuildMap()
{
	std::vector<PolygoneMapNode> tempNodes;

	for (int i = 0; i < static_cast<int>(mPolygonGridGraph.GetNodeCount()); ++i)		// Put every valid node in to tempNodes from mOverAllGraph
	{
		auto& currentNode = mPolygonGridGraph.GetNode(i);

		if (currentNode.neighbors.empty())
			continue;

		//--------------------------------------------------------------------------------------//|
		size_t currentSize = tempNodes.size();														//|
		PolygoneMapNode newNode(currentNode);													//|
		newNode.worldGraphIndex = currentNode.worldGraphIndex;									//|
		newNode.inPolygonIndex = currentSize;													//|--- add newNode to tempNodes
		currentNode.inPolygonIndex = currentSize;												//|
		mOverAllGraph.GetNode(currentNode.worldGraphIndex).inPolygonIndex = currentSize;		//|
		tempNodes.push_back(std::move(newNode));												//|
		//--------------------------------------------------------------------------------------//|
	}

	for (auto& node : tempNodes)									// Update the Neighbors of each node
	{
		for (auto& neighborIndex : node.neighbors)					// Update indexinfo
		{
			neighborIndex = mPolygonGridGraph.GetNode(neighborIndex).inPolygonIndex;
		}
	}

	mPolygonGridGraph.Initialize(tempNodes.size());					// Update mPolygonGridGraph
	for (int i = 0; i < static_cast<int>(mPolygonGridGraph.GetNodeCount()); i++)
	{
		mPolygonGridGraph.GetNode(i) = tempNodes[i];
	}

	mIsInUse = true;
	// Done.
}

void PolygonGridGraphMap::AddingWall()
{
	Vector2 mousePos = { (float)EngineWrapper::GetMouseScreenX(), (float)EngineWrapper::GetMouseScreenY() };
	int currentInPolyIndex = ConvertToPolygonGraphIndex(mousePos);
	if (currentInPolyIndex == -1) currentInPolyIndex = 0;
	auto currentSelectPos = mPolygonGridGraph.GetNode(currentInPolyIndex).position;

	if ((mIsAddingWall & isAddingWallMask) != 0)
	{
		mIsAddingWall = isEditPointAMask;

		mWalls.emplace_back(currentSelectPos, currentSelectPos);
	}
	else if ((mIsAddingWall & isEditPointAMask) != 0)
	{
		mWalls.back().from = currentSelectPos;
		mWalls.back().to = currentSelectPos;
		
		if (EngineWrapper::IsMousePressed(Mouse::LBUTTON))
		{
			mIsAddingWall = isEditPointBMask;
		}
	} 
	else if ((mIsAddingWall & isEditPointBMask) != 0)
	{
		 mWalls.back().to = currentSelectPos;

		auto& a = mWalls.back().from;
		auto& b = mWalls.back().to;

		if (EngineWrapper::IsMousePressed(Mouse::LBUTTON))
		{
			if (((a.x == b.x) && (a.y != b.y)) || ((a.x != b.x) && (a.y == b.y)))
			{
				if (IsWallValid({ a, b }))
				{
					BuildWall({ a, b });
					mIsAddingWall = 0;
				}
			}
		}
	}

	if ((EngineWrapper::IsMousePressed(Mouse::RBUTTON))&&(((mIsAddingWall & isEditPointAMask) != 0)||((mIsAddingWall & isEditPointBMask) != 0)))
	{
		mWalls.pop_back();
		mIsAddingWall = 0;
	}
	
}

void PolygonGridGraphMap::BuildWall(const LineSegment& wall)
{
	bool isVerticle = NFGE::Math::IsZero(wall.from.x - wall.to.x);
	float nodeDist = mTileSize * 0.5f;

	int travelCount;
	if (isVerticle)
		travelCount = (int)((wall.to.y - wall.from.y) / nodeDist);
	else
		travelCount = (int)((wall.to.x - wall.from.x) / nodeDist);

	for (int i = 0; i < abs(travelCount); i++)
	{
		int dir = travelCount > 0 ? 1 : -1;

		if (isVerticle)
		{
			IsolateNode(ConvertToPolygonGraphIndex(wall.from + Vector2{ 0.0f, (float)(i * dir) * nodeDist }));
		}
		else
		{
			IsolateNode(ConvertToPolygonGraphIndex(wall.from + Vector2{ (float)(i * dir) * nodeDist , 0.0f }));
		}
	}
}

void PolygonGridGraphMap::AddingObs()
{
	Vector2 mousePos = { (float)EngineWrapper::GetMouseScreenX(), (float)EngineWrapper::GetMouseScreenY() };
	int currentInPolyIndex = ConvertToPolygonGraphIndex(mousePos);
	if (currentInPolyIndex == -1) currentInPolyIndex = 0;
	auto currentSelectPos = mPolygonGridGraph.GetNode(currentInPolyIndex).position;

	if ((mIsAddingObbs & isAddingObsMask) != 0)
	{
		mIsAddingObbs = isEditObsPosMask;

		mObstacles.emplace_back(currentSelectPos,5.0f);
	}
	else if ((mIsAddingObbs & isEditObsPosMask) != 0)
	{
		auto& currentObs = mObstacles.back();
		currentObs.center = currentSelectPos;
		if (IsKeyPressed(Keys::Q))
		{
			currentObs.radius -= ObsRadiusChangeSpeed;
			currentObs.radius = NFGE::Math::Max(0.0f, currentObs.radius);
		}
		if (IsKeyPressed(Keys::E))
		{
			currentObs.radius += ObsRadiusChangeSpeed;
		}

		if (EngineWrapper::IsMousePressed(Mouse::LBUTTON))
		{
			mIsAddingObbs = 0;
		}
	}

	if ((EngineWrapper::IsMousePressed(Mouse::RBUTTON))&&((mIsAddingObbs & isEditObsPosMask) != 0))
	{
		mObstacles.pop_back();
		mIsAddingObbs = 0;
	}
}

void PolygonGridGraphMap::BindTheAIWorld(AI::AIWorld & world)
{
	FinishBuildMap();
	for (auto& wall : mWalls)
	{
		world.AddWall(wall);
	}

	for (int i = 0; i < static_cast<int>(mPolygon.mSides); ++i)
	{
		world.AddWall({ mPolygon.GetVertices(i), mPolygon.GetVertices(i + 1) });
	}

	for (auto& obs : mObstacles)
	{
		world.AddObstacles(obs);
	}
}

void PolygonGridGraphMap::FindPathInPoly(const Vector2 & from, const Vector2 & to)
{
	int fromIndex = ClosestPolygonGraphIndex(from);
	int toIndex = ClosestPolygonGraphIndex(to);
	return PathFinding< AI::Graph<PolygoneMapNode>, GridMap_CanOpenNode_Djika, GridMap_GetGCost, GridMap_GetHCost>(mOverAllGridMap.GraphContext, mPolygonGridGraph, fromIndex, toIndex, SearchMethod::A_Star);
	
}

std::list<Vector2> PolygonGridGraphMap::GetPosPath()
{
	auto pathIndex = GetPath(mOverAllGridMap.GraphContext);
	std::list<Vector2> ret;
	
	for (auto index : pathIndex)
	{
		ret.push_front(mPolygonGridGraph.GetNode(index).position);
	}
	
	//-----------------------------------------------------------------------------------------------------------//|
	ShowClosedList<AI::Graph<PolygoneMapNode>>(mOverAllGridMap.GraphContext, mPolygonGridGraph, 10.0f, Colors::LightGreen);
	ShowPath<AI::Graph<PolygoneMapNode>>(pathIndex, mPolygonGridGraph, 10.0f, Colors::LightCoral);			 //|--- debug draw
	//-----------------------------------------------------------------------------------------------------------//|
	return ret;
}

void PolygonGridGraphMap::RenderPolygon(const Camera2D & camera)
{
	//TODO:: Render LineSegments and vertices
	if (mPolygon.mVertices.size() < 1)
		return;

	DrawScreenCircle(mPolygon.mVertices[0], 3.0f, Colors::Green);

	for (int i = 0; i < static_cast<int>(mPolygon.mVertices.size() - 1); i++)
	{
		DrawScreenCircle(mPolygon.mVertices[i], 3.0f, Colors::Green);
		DrawScreenLine(mPolygon.mVertices[i], mPolygon.mVertices[i + 1], Colors::Blue);
	}

	if (mIsBuilt == true)
	{
		DrawScreenCircle(mPolygon.mVertices.back(), 3.0f, Colors::Green);
		DrawScreenLine(mPolygon.mVertices[0], mPolygon.mVertices.back(), Colors::Blue);
	}
}

void PolygonGridGraphMap::RenderWall(const Camera2D & camera)
{
	for (auto& wall : mWalls)
	{
		DrawScreenLine(wall.from, wall.to, Colors::Blue);
	}
}

void PolygonGridGraphMap::RenderObs(const Camera2D & camera)
{
	for (auto& obs: mObstacles)
	{
		DrawScreenCircle(obs.center, obs.radius, Colors::Yellow);
	}
}

void PolygonGridGraphMap::RenderTriangles(const Camera2D & camera)
{
	for (auto& triangle : mTrianglesInPoly)
	{
		DrawScreenLine(triangle.p0, triangle.p1, Colors::White);
		DrawScreenLine(triangle.p1, triangle.p2, Colors::White);
		DrawScreenLine(triangle.p2, triangle.p0, Colors::White);
	}
}

void PolygonGridGraphMap::RenderPolygonGraph(const Camera2D & camera)
{
	if (mIsBuilt)
	{
		float simpleDrawScale = (mOverAllGridMap.xScale < mOverAllGridMap.yScale) ? mOverAllGridMap.xScale : mOverAllGridMap.yScale;
		ShowGraph(mPolygonGridGraph, mOverAllGridMap.allTiles, 5.0f *  simpleDrawScale * camera.GetScale(), camera);
	}
	else
	{
		OptmaisPolyGraphRender(camera);
	}
}

void PolygonGridGraphMap::OptmaisPolyGraphRender(const Camera2D & camera)
{
	auto& mapGraph = mOverAllGridMap.gridGraph;
	int graphRow = mOverAllGridMap.row;
	int graphCol = mOverAllGridMap.col;
	for (int i = 0; i < graphCol; i++)
	{
		DrawScreenLine(mapGraph.GetNode(i).position, mapGraph.GetNode((graphRow - 1) * graphCol + i).position, Colors::Gray);
	}
	for (int j = 0; j < mOverAllGridMap.row; j++)
	{
		DrawScreenLine(mapGraph.GetNode(graphCol * j).position, mapGraph.GetNode(graphCol * j + graphRow).position, Colors::Gray);
	}
}

void PolygonGridGraphMap::Cut(CutPolyContext& context)
{
	auto& it = context.mCurrentVertice;
	auto& listVertices = context.mVertices;
	for (it = listVertices.begin(); it != listVertices.end(); it++)
	{
		auto p0 = it;
		auto p1 = std::next(p0) == listVertices.end() ? (listVertices.begin()) : (std::next(p0));
		auto p2 = std::next(p1) == listVertices.end() ? (listVertices.begin()) : (std::next(p1));
		
		if (IsTriangleValid({ *p0, *p1, *p2 }, context, p1))
		{
			mTrianglesInPoly.emplace_back(*p0, *p1, *p2);
			context.mVertices.erase(p1);
			return;
		}
	}

	auto p0 = listVertices.rbegin();
	auto p1 = listVertices.begin();
	auto p2 = std::next(p1);
	if (IsTriangleValid({ *p0, *p1, *p2 }, context, p1))
	{
		mTrianglesInPoly.emplace_back(*p0, *p1, *p2);
		context.mVertices.erase(p1);
		return;
	}
}

void PolygonGridGraphMap::MingzhuoPolyCut()
{
	CutPolyContext cutContext;
	for (auto& vertice : mPolygon.mVertices)
	{
		cutContext.mVertices.push_back(vertice);
	}
	cutContext.mCurrentVertice = cutContext.mVertices.begin();

	bool isClockwise = mPolygon.mClockwiseCount > mPolygon.mCounterClockwiseCount;

	while(cutContext.mVertices.size() > 2)
	{
		Cut(cutContext);
	}
}

bool PolygonGridGraphMap::IsTriangleValid(const Triangle& tri, CutPolyContext& context, std::list<Vector2>::iterator cutPoint)
{
	if (mPolygon.IsIntercet({ tri.p0, tri.p2 }))
		return false;
	if (IsClockwise(tri.p0, tri.p1, tri.p2) != (mPolygon.mClockwiseCount > mPolygon.mCounterClockwiseCount))
		return false;
	if (IsIntercetTriangles({ tri.p0, tri.p2 }))
		return false;
	if (IsAnyPointsInTriangle(context.mVertices, tri))
		return false;
	
	bool isSameLine, isOverLap;
	if (customIntersect({ tri.p0, tri.p1 }, { tri.p1, tri.p2 }, isOverLap, isSameLine))
	{
		context.mVertices.erase(cutPoint);
		return false;
	}

	return true;
}

bool PolygonGridGraphMap::IsWallValid(const LineSegment & wall)
{
	/*if (mPolygon.IsIntercet(wall))
		return false;*/

	return true;
}

bool PolygonGridGraphMap::IsIntercetTriangles(const LineSegment& line)
{
	bool overLap, isSameLine;
	for (auto& triangle : mTrianglesInPoly)
	{
		if (customIntersect(line, { triangle.p0, triangle.p1 }, overLap, isSameLine))
			return true;
		if(customIntersect(line, { triangle.p1, triangle.p2 }, overLap, isSameLine))
			return true;
		if(customIntersect(line, { triangle.p2, triangle.p0 }, overLap, isSameLine))
			return true;
	}
	return false;
}

bool PolygonGridGraphMap::IsPointInTriangle(const Vector2 & point, const Triangle & tri)
{
	return (EngineWrapper::IsPointInTriangle(tri.p0, tri.p1, tri.p2, point));
}

bool PolygonGridGraphMap::IsAnyPointsInTriangle(const std::list<Vector2>& points, const Triangle & tri)
{
	for (auto& point : points)
	{
		if (IsPointInTriangle(point, tri))
			return true;
	}
	return false;
}

void PolygonGridGraphMap::BuildPolygonGraph()
{
	std::vector<PolygoneMapNode> tempNodes;

	for (int i = 0; i < static_cast<int>(mOverAllGraph.GetNodeCount()); ++i)		// Put every valid node in to tempNodes from mOverAllGraph
	{
		auto& currentNode = mOverAllGraph.GetNode(i);
		for (auto& tri : mTrianglesInPoly)
		{
			auto miniRect = EngineWrapper::GetMiniRect(tri);
			
			if (!EngineWrapper::PointInRect(currentNode.position, miniRect))
				continue;
			if (!EngineWrapper::IsPointInTriangle_Custom_OnEdgeCount(tri.p0, tri.p1, tri.p2, currentNode.position))
				continue;
			
			//--------------------------------------------------//|
			PolygoneMapNode newNode(currentNode);				//|
			newNode.worldGraphIndex = i;						//|
			newNode.inPolygonIndex = tempNodes.size();			//|--- add newNode to tempNodes
			currentNode.inPolygonIndex = tempNodes.size();		//|
			tempNodes.push_back(std::move(newNode));			//|
			break;												//|-- Make sure each node only add once
			//--------------------------------------------------//|
			
		}
	}
	
	for (auto& node : tempNodes)									// Update the Neighbors of each node
	{
		for (auto& neighborIndex : node.neighbors)					// Update indexinfo
		{
			neighborIndex = mOverAllGraph.GetNode(neighborIndex).inPolygonIndex;
		}

		//||||------------------------------------------------------// Remove invalide neighbors
		//vvvv																			 //|
		for (std::vector<size_t>::iterator iter = node.neighbors.begin(); iter != node.neighbors.end();)	 //|
		{																				 //|
			if (*iter == AI::InvalidNode)												 //|
				iter = node.neighbors.erase(iter);											 //|
			else
				++iter;
		}																				 //|
		//^^^^																			 //|
		//||||---------------------------------------------------------------------------//|
	}

	mPolygonGridGraph.Initialize(tempNodes.size());					// Update mPolygonGridGraph
	for (int i = 0; i < static_cast<int>(mPolygonGridGraph.GetNodeCount()); i++)
	{
		mPolygonGridGraph.GetNode(i) = tempNodes[i];
	}

	// Done.
}

int PolygonGridGraphMap::ConvertToPolygonGraphIndex(const Vector2 & position) const
{
	int overAllGraphIndex = mOverAllGridMap.ConvertToMapIndex(position);

	return(int)mOverAllGraph.GetNode(overAllGraphIndex).inPolygonIndex;
}

int PolygonGridGraphMap::ClosestPolygonGraphIndex(const Vector2 & position)
{
	int overAllGraphIndex = mOverAllGridMap.ConvertToMapIndex(position);

	if ((overAllGraphIndex < 0))
		return 0;
	if (overAllGraphIndex >= static_cast<int>(mOverAllGraph.GetNodeCount()))
		return static_cast<int>(mPolygonGridGraph.GetNodeCount() - 1);

	size_t ret = mOverAllGraph.GetNode(overAllGraphIndex).inPolygonIndex;
	
	if (ret == AI::InvalidNode)
	{
		mOverAllGridMap.GraphContext.Init(mOverAllGraph.GetNodeCount());
		mOverAllGridMap.GraphContext.Reset();
		auto customSearch = CustomSearch_fluidFill<AI::Graph<PolygoneMapNode>, AI::GraphSearch::PushBackPolicy, AI::GraphSearch::PopFrontPolicy, GridMap_CanOpenNode_DFSnBFS>(mOverAllGraph);
		customSearch.Execute(mOverAllGridMap.GraphContext);
		ASSERT(mOverAllGridMap.GraphContext.found == true, "[PolygonGridGraphMap] suppose to find the close node in the polyGrid to the current node which is ouside polyGird, but not");

		//-----------------------------------------------------------------------------------------------------------//|
		auto pathIndexs = GetPath(mOverAllGridMap.GraphContext);
		ShowClosedList<AI::Graph<PolygoneMapNode>>(mOverAllGridMap.GraphContext, mOverAllGraph, 10.0f, Colors::LightGreen);
		ShowPath<AI::Graph<PolygoneMapNode>>(pathIndexs, mOverAllGraph, 10.0f, Colors::LightCoral);			 //|--- debug draw
		//-----------------------------------------------------------------------------------------------------------//|

		ret = mOverAllGraph.GetNode(mOverAllGridMap.GraphContext.closedList.back()).inPolygonIndex;
	}
	return (int)ret;
}

void PolygonGridGraphMap::IsolateNode(size_t index)
{
	if (index == AI::InvalidNode)			//|
		return;								//|

	auto& currentNode = mPolygonGridGraph.GetNode(index);
	for (auto& neighbor : currentNode.neighbors)
	{
		auto& currentNeighbor = mPolygonGridGraph.GetNode(neighbor);

		auto iter = std::find(currentNeighbor.neighbors.begin(), currentNeighbor.neighbors.end(), index);
		if (iter != currentNeighbor.neighbors.end())
		//XASSERT(iter != currentNeighbor.neighbors.end(), "[PolygonGridGraphMap] -> Isolating node: node neighbor can not find the isolating node");
		{
			std::iter_swap(iter, currentNeighbor.neighbors.end() - 1);
			currentNeighbor.neighbors.pop_back();
		}
	}
	currentNode.neighbors.clear();
}
