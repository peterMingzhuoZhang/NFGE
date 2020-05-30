#ifndef INCLUDE_NFGE_AI_AIWORLD_H
#define INCLUDE_NFGE_AI_AIWORLD_H

#include "Agent.h"
#include "Graph.h"

namespace NFGE::AI
{
	class AIWorld
	{	
	public:
		using Obstacles = std::vector<NFGE::Math::Circle>;
		using Walls = std::vector<NFGE::Math::LineSegment>;
		using EntitiesMap = std::unordered_map<std::string, std::vector<Entity*>>;
		

		struct Grid
		{
			struct Cell
			{
				Cell(int totalAgents)
					:mAgents()
				{
					mAgents.reserve(totalAgents);
				}
				Agents mAgents;
			};

			Grid() = default;

			void Init(int row, int col, float size, int totatlAgents)
			{
				mRow = row;
				mCol = col;
				mSize = size;
				mCells.reserve(mRow * mCol);
				for (int i = 0; i < mRow * mCol; i++)
				{
					mCells.push_back(Cell(totatlAgents));
				}
				mHasInit = true;
			}

			void Init(float worldWidth, float worldHeight, int totatlAgents)
			{
				mSize = 10.0f;
				mRow = static_cast<int>(worldHeight / mSize);
				mCol = static_cast<int>(worldWidth / mSize);
				mCells.reserve(mRow * mCol);
				for (int i = 0; i < mRow * mCol; i++)
				{
					mCells.push_back(Cell(totatlAgents));
				}
				mHasInit = true;
			}

			void ClearGridCells()
			{
				for (auto& cell : mCells)
				{
					cell.mAgents.clear();
				}
			}

			int GetInCellIndex(NFGE::Math::Vector2 agentPos) const
			{
				int InCellCol = static_cast<int>(agentPos.x / mSize);
				int InCellRow = static_cast<int>(agentPos.y / mSize);
				return InCellRow * mCol + InCellCol;
			}

			void FenPeiAgent(Agent* agentPtr)
			{
				int InCellIndex = GetInCellIndex(agentPtr->position);
				if ((InCellIndex < 0) ||(InCellIndex >= static_cast<int>(mCells.size())))
					return;
				mCells[InCellIndex].mAgents.push_back(agentPtr);
			}

			//------------------------------------------------------------------------------------
			//-|||||||||||-- Debug Function ------------------------------------------------------
			//-|||||||||||------------------------------------------------------------------------
			//-vvvvvvvvvvv------------------------------------------------------------------------
			void DebugDraw()
			{
				for (int i = 0; i < static_cast<int>(mCells.size()); i++)
				{
					int row = i / mCol;
					int col = i % mCol;
					NFGE::Graphics::SimpleDraw::AddScreenRect(col * mSize, row * mSize, (col + 1) * mSize, (row + 1) * mSize, NFGE::Graphics::Colors::Orange);
				}
			}
			//-^^^^^^^^^^^------------------------------------------------------------------------
			//-|||||||||||------------------------------------------------------------------------
			//-|||||||||||------------------------------------------------------------------------
			//------------------------------------------------------------------------------------

			int mRow, mCol;
			float mSize = 10.0f;	// We have 10.0f as the mini cell size;
			std::vector<Cell> mCells;
			bool mHasInit = false;
		} mGrid;

		template<class EntityType>
		inline void RegisterEntity(EntityType* entityPtr);
		template<class EntityType>
		inline void UnregisterEntity(EntityType* entityPtr);
		template<class EntityType>
		inline const std::vector<EntityType*> GetEntities() const;

		void RegisterAgent(Agent* agent);
		void UnregisterAgent(Agent* agent);
		const Agents& GetAgents() const { return mAgents; }

		void GetNeighborhood(Agents& neigbor, const NFGE::Math::Circle & range) const;

		void AddObstacles(const NFGE::Math::Circle& obstacle);
		const Obstacles& GetObstacles() const { return mObstacles; }
		void ClearObstacles() { mObstacles.clear(); }

		void AddWall(const NFGE::Math::LineSegment& wall);
		const Walls& GetWalls() const { return mWalls; }

		void Update();
		void Update(float newCellSize);

	private:
		EntitiesMap mEntitiesMap{};
		Obstacles mObstacles{};		// Sepcial Entity
		Walls mWalls{};				// Sepcial Entity
		Agents mAgents{};			// Sepcial Entity

		template<class EntityType>
		inline std::vector<Entity*>& GetEntities_private() ;
	};

	// Template Implemanetation ------------------
	template<class EntityType>
	inline void AIWorld::RegisterEntity(EntityType* entityPtr)
	{
		std::string key = entityPtr->GetTypeName();
		std::vector<AI::Entity*>* theEntities;
		auto[it, success] = mEntitiesMap.try_emplace(std::move(key), std::vector<AI::Entity*>());
		if (success)
		{
			theEntities = &(it->second);
		}
		else
		{
			theEntities = &GetEntities_private<EntityType>();
		}

		theEntities->push_back(static_cast<AI::Entity*>(entityPtr));
	}

	template<class EntityType>
	inline void AIWorld::UnregisterEntity(EntityType* entityPtr)
	{
		std::vector<EntityType*>& theEntities = GetEntities_private<EntityType>();

		auto iter = std::find(theEntities.begin(), theEntities.end(), entityPtr);
		if (iter != theEntities.end())
		{
			std::iter_swap(iter, theEntities.end() - 1);
			theEntities.pop_back();
		}
	}

	template<class EntityType>
	inline const std::vector<EntityType*> AIWorld::GetEntities() const
	{
		//return const_cast<AIWorld*>(this)->GetEntities_private<EntityType>();			//Usine of const_cast technique that Peter Chan teaches me!!

		std::string key = EntityType::GET_TYPE_NAME();
		auto iter = mEntitiesMap.find(key);
		std::vector<EntityType*> ret{};
		if (iter != mEntitiesMap.end())
		{
			for (auto element : iter->second)
			{
				ret.push_back(reinterpret_cast<EntityType*>(element));
			}
		}
		return ret;
	}

	template<class EntityType>
	inline std::vector<Entity*>& AIWorld::GetEntities_private()
	{
		std::string key = EntityType::GET_TYPE_NAME();
		auto iter = mEntitiesMap.find(key);
		ASSERT(iter != mEntitiesMap.end(), "[AIWorld-FindEntities] Can not find Entity's vector of : %s", key.c_str());

		return iter->second;
	}
}


#endif // !INCLUDE_NFGE_AI_AIWORLD_H
