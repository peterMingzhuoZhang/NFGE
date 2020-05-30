//====================================================================================================
// Filename:	Entity.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <Entity> which is a base class of any Entity, This allows us to
//				track a entity just with its index
//				It contains :	---	A static counter of all entity that has been create
//								---	A static vector of entity* to track all entity
//								---	A member for entity: position
//====================================================================================================

#ifndef INCLIDED_AI_ENTITY_H
#define INCLIDED_AI_ENTITY_H

#include <string>
#include <vector>
#include <NFGEMath/Inc/NFGEMath.h>

namespace NFGE::AI
{
	// Run Time Get Class Name
#define RTGCN(ClassName)\
virtual std::string GetTypeName() override { return #ClassName; } \
static std::string GET_TYPE_NAME() {return #ClassName; }

	struct Entity
	{
		virtual std::string GetTypeName() = 0;
		static int mTotalEntity;
		static std::vector<Entity*> mAllEntity;	// Every entity ptr in the program will be there. Make sure create entity in top Scope level. (Like Global Scope or Objective pool)

		Entity()
		{
			mId = mTotalEntity;
			mTotalEntity++;
			mAllEntity.push_back(this);
		}

		int mId = 0;
		NFGE::Math::Vector2 position;
	};
}
#endif // !INCLIDED_AI_ENTITY_H
