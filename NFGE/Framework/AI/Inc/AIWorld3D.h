#ifndef INCLUDE_NFGE_AI_AIWORLD3D_H
#define INCLUDE_NFGE_AI_AIWORLD3D_H

#include "Agent3D.h"
#include "Graph.h"

namespace NFGE::AI
{
	class AIWorld3D
	{
	public:
		using Obstacle3Ds = std::vector<NFGE::Math::Sphere>;
		using Wall3Ds = std::vector<NFGE::Math::Plane>;
		using Entity3DsMap = std::unordered_map<std::string, std::vector<Entity3D*>>;


		struct Grid3D
		{
			struct Cell3D
			{
				Cell3D(int totalAgents)
					:mAgents()
				{
					mAgents.reserve(totalAgents);
				}
				Agent3Ds mAgents;
			};

			Grid3D() = default;

			void Init(int row, int col,int height, float size, int totatlAgents)
			{
				mRow = row;
				mCol = col;
				mHeight = height;
				mSize = size;
				mCells.reserve(mRow * mCol * mHeight);
				for (int i = 0; i < mRow * mCol * mHeight; i++)
				{
					mCells.push_back(Cell3D(totatlAgents));
				}
				mHasInit = true;
			}

			void Init(float worldWidth, float worldHeight, float worldDepth, int totatlAgents)
			{
				mSize = 10.0f;
				mRow = static_cast<int>(worldDepth / mSize);
				mCol = static_cast<int>(worldWidth / mSize);
				mHeight = static_cast<int>(worldHeight / mSize);
				mCells.reserve(mRow * mCol * mHeight);
				for (int i = 0; i < mRow * mCol * mHeight; i++)
				{
					mCells.push_back(Cell3D(totatlAgents));
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

			int GetInCellIndex(NFGE::Math::Vector3 agentPos) const
			{
				int InCellCol = static_cast<int>(agentPos.x / mSize);
				int InCellRow = static_cast<int>(agentPos.z / mSize);
				int InCellHeight = static_cast<int>(agentPos.y / mSize);
				return InCellRow * mCol + InCellCol + InCellHeight * (mCol + mRow);
			}

			void FenPeiAgent(Agent3D* agentPtr)
			{
				int InCellIndex = GetInCellIndex(agentPtr->position);
				if ((InCellIndex < 0) || (InCellIndex >= static_cast<int>(mCells.size())))
					return;
				mCells[InCellIndex].mAgents.push_back(agentPtr);
			}


			int mRow, mCol, mHeight;
			float mSize;
			std::vector<Cell3D> mCells;
			bool mHasInit = false;
		} mGrid;

		template<class EntityType>
		inline void RegisterEntity3D(EntityType* entityPtr);
		template<class EntityType>
		inline void UnregisterEntity3D(EntityType* entityPtr);
		template<class EntityType>
		inline const std::vector<EntityType*> GetEntity3Ds() const;

		void RegisterAgent3D(Agent3D* agent);
		void UnregisterAgent3D(Agent3D* agent);
		const Agent3Ds& GetAgent3Ds() const { return mAgents; }

		void GetNeighborhood(Agent3Ds& neighbors, const NFGE::Math::Sphere & range) const;

		void AddObstacle3D(const NFGE::Math::Sphere& obstacle);
		const Obstacle3Ds& GetObstacle3Ds() const { return mObstacles; }

		void AddWall3D(const NFGE::Math::Plane& wall);
		const Wall3Ds& GetWalls() const { return mWalls; }

		void Update(float deltaTime);
		void Update(float neighborRange, float deltaTime);

	private:
		Entity3DsMap mEntitiesMap{};
		Obstacle3Ds mObstacles{};		// Sepcial Entity
		Wall3Ds mWalls{};				// Sepcial Entity
		Agent3Ds mAgents{};			// Sepcial Entity

		template<class EntityType>
		inline std::vector<Entity3D*>& GetEntity3Ds_private();
	};

	// Template Implemanetation ------------------
	template<class EntityType>
	inline void AIWorld3D::RegisterEntity3D(EntityType* entityPtr)
	{
		std::string key = entityPtr->GetTypeName();
		std::vector<AI::Entity3D*>* theEntities;
		auto[it, success] = mEntitiesMap.try_emplace(std::move(key), std::vector<AI::Entity3D*>());
		if (success)
		{
			theEntities = &(it->second);
		}
		else
		{
			theEntities = &GetEntity3Ds_private<EntityType>();
		}

		theEntities->push_back(static_cast<AI::Entity3D*>(entityPtr));
	}

	template<class EntityType>
	inline void AIWorld3D::UnregisterEntity3D(EntityType* entityPtr)
	{
		std::vector<EntityType*>& theEntities = GetEntity3Ds_private<EntityType>();

		auto iter = std::find(theEntities.begin(), theEntities.end(), entityPtr);
		if (iter != theEntities.end())
		{
			std::iter_swap(iter, theEntities.end() - 1);
			theEntities.pop_back();
		}
	}

	template<class EntityType>
	inline const std::vector<EntityType*> AIWorld3D::GetEntity3Ds() const
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
	inline std::vector<Entity3D*>& AIWorld3D::GetEntity3Ds_private()
	{
		std::string key = EntityType::GET_TYPE_NAME();
		auto iter = mEntitiesMap.find(key);
		ASSERT(iter != mEntitiesMap.end(), "[AIWorld-FindEntities] Can not find Entity's vector of : %s", key.c_str());

		return iter->second;
	}
}


#endif // !INCLUDE_NFGE_AI_AIWORLD_H
