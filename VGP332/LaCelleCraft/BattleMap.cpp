#include "BattleMap.h"
#include "Base.h"

//-------------------------------------------------------------------------------------------------------------------------------------//|
BattleMap myBattleMap;																											   //|
struct GridMap_CanOpenNode_Djika																									   //|
{																																	   //|
	bool operator()(AI::GraphSearch::Context& context, size_t n)																	   //|
	{																																   //|
		if (context.closed[n])																										   //|
			return false;																											   //|
		if (myBattleMap.IsWall(n))																									   //|
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
		if (myBattleMap.IsWall(n))																									   //|--- Grid map and its policies
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
		size_t fromColumn = parentIndex % myBattleMap.col;																			   //|
		size_t fromRow = parentIndex / myBattleMap.col;																				   //|
		size_t toColume = nodeIndex % myBattleMap.col;																				   //|
		size_t toRow = nodeIndex / myBattleMap.col;																					   //|
		float currentGCost = myBattleMap.allTiles[myBattleMap.gridGraph.GetNode(nodeIndex).tileType].walkThroughCost;
		return context.g[parentIndex] + ((fromColumn != toColume && fromRow != toRow) ? 1.414f*currentGCost : 1.0f*currentGCost);	   //|
																																	   //|
	}																																   //|
};																																	   //|
struct GridMap_GetHCost																												   //|
{																																	   //|
	float operator()(AI::GraphSearch::Context& context, size_t nodeIndex)															   //|
	{																																   //|
																																	   //|
		size_t fromColumn = nodeIndex % myBattleMap.col;																				   //|
		size_t fromRow = nodeIndex / myBattleMap.col;																					   //|
		size_t toColume = context.end % myBattleMap.col;																				   //|
		size_t toRow = context.end / myBattleMap.col;																					   //|
																																	   //|
		return  sqrtf(static_cast<float>((toColume - fromColumn) * (toColume - fromColumn) + (toRow - fromRow) *(toRow - fromRow)));   //|
	}																																   //|
};																																	   //|
																																	   //|
																																	   //-------------------------------------------------------------------------------------------------------------------------------------//|

void BattleMap::Load(std::string fileName)
{
	TextureGridMap::Open(fileName);
}

void BattleMap::Unload()
{
	TextureGridMap::Unload();
}

void BattleMap::Update(float deltaTime)
{
	TextureGridMap::Update(deltaTime);
	for (auto& Resource : mAllResource)
	{
		Resource.Update(deltaTime);
	}
}

void BattleMap::Render(const Camera2D & camera)
{
	TextureGridMap::Render(camera);
	for (auto& Resource : mAllResource)
	{
		Resource.Render(camera);
	}
}

void BattleMap::DebugUIUpdate(float deltaTime, const Camera2D & camera)
{
	TextureGridMap::DebugUIUpdate(deltaTime, camera);

	bool isThereSeleted = false;
	for (auto& Resource : mAllResource)
	{
		Resource.DebugUIUpdate(deltaTime, camera);
		if (Resource.mIsEditorSelected)
			isThereSeleted = true;
	}

	for (auto& base : mBases)
	{
		base.Update(deltaTime);
		base.Render(camera);
		base.DebugUIUpdate(deltaTime, camera);
		if (base.mIsEditorSelected)
			isThereSeleted = true;
	}

	if (!isThereSeleted)
	{
		Vector2 mousePos = { static_cast<float>(GetMouseScreenX()), static_cast<float>(GetMouseScreenY()) };
		ImGui::Begin("Battle Map");
		if (ImGui::Button("Add Money Resource"))
		{
			mAllResource.emplace_back();
			mAllResource.back().Load(*this, camera.ConvertToWorld(mousePos), ResourceType::Money, 10000);
			mAllResource.back().mIsEditorSelected = true;
		}
		if (ImGui::Button("Add Base"))
		{
			mBases.emplace_back();
			mBases.back().EditorLoad(*this, camera.ConvertToWorld(mousePos));
			mBases.back().mIsEditorSelected = true;
		}

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Battle Map");
		if (ImGui::Button("Delete"))
		{
			int i = 0;
			bool isResourceGotSelected = false;
			for (; i < mAllResource.size(); ++i)
			{
				if (mAllResource[i].mIsEditorSelected)
				{
					isResourceGotSelected = true;
					break;
				}
			}

			if (isResourceGotSelected)
			{
				mAllResource.erase(mAllResource.begin() + i);
			}
			else
			{
				i = 0;
				for (; i < mBases.size(); ++i)
				{
					if (mBases[i].mIsEditorSelected)
					{
						break;
					}
				}
				mBases.erase(mBases.begin() + i);
			}
		}
		ImGui::End();
	}
	
}

void BattleMap::FindPath(int start, int end)
{
	PathFinding<AI::Graph<MapNode>, GridMap_CanOpenNode_Djika, GridMap_GetGCost, GridMap_GetHCost>(GraphContext, gridGraph, start, end, SearchMethod::A_Star);
}

void BattleMap::Serialize_txt(std::ofstream & myfile)
{
	for (auto& base : mBases)
	{
		base.RemoveFromMap();
	}

	for (auto& resource: mAllResource)
	{
		resource.RemoveFromMap();
	}

	TextureGridMap::Serialize_txt(myfile);
	
	// serilize resource info here
	myfile << "TotalResourceCount: " << mAllResource.size() << "\n";
	for (auto& Resource : mAllResource)
	{
		myfile 
			<< static_cast<int>(Resource.mResourceType) << " "
			<< Resource.mPosition.x << " " 
			<< Resource.mPosition.y << " "
			<< Resource.mTotalAmount << "\n";
		Resource.PutOnMap();
	}

	// serilize base info here
	myfile << "TotalCommanderCount: " << mBases.size() << "\n";
	for (auto& base : mBases)
	{
		myfile
			<< base.mPosition.x << " "
			<< base.mPosition.y << "\n";
		base.PutOnMap();
	}

}

void BattleMap::Deserialize_txt(std::ifstream & theFile)
{
	TextureGridMap::Deserialize_txt(theFile);
	
	std::string line;
	std::stringstream tokenizer;
	std::string temp;
	// deserialize resource info here
	int resourceCount = 0;
	getline(theFile, line); // get rid of the empty line from last step of retriving all map node index by getchar()
	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> resourceCount;

	mAllResource.clear();
	for (int i = 0; i < resourceCount; ++i)
	{
		mAllResource.emplace_back();

		float initPosX, initPosY;
		int type, totalAmount;
	
		getline(theFile, line);
		tokenizer = std::stringstream(line);
		tokenizer >> type;
		tokenizer >> initPosX;
		tokenizer >> initPosY;
		tokenizer >> totalAmount;

		mAllResource.back().Load(*this, Vector2{ initPosX, initPosY }, static_cast<ResourceType>(type), totalAmount);
	}

	//// deserialize resource info here
	getline(theFile, line); // get rid of the empty line from last step of retriving all map node index by getchar()
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> mTotalCommander;

	mBases.clear();
	for (int i = 0; i < mTotalCommander; ++i)
	{
		mBases.emplace_back();

		float initPosX, initPosY;

		getline(theFile, line);
		tokenizer = std::stringstream(line);
		tokenizer >> initPosX;
		tokenizer >> initPosY;

		mBases.back().EditorLoad(*this,Vector2{ initPosX, initPosY });
	}

}
