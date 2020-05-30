//====================================================================================================
// Filename:	Entity3D.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <Entity3D> which is a base class of any Entity3D, This allows us to
//				track a entity3D just with its index
//				It contains :	---	A static counter of all entity that has been create
//								---	A static vector of entity* to track all entity
//								---	A member for entity: position
//====================================================================================================

#ifndef INCLIDED_AI_ENTITY3D_H
#define INCLIDED_AI_ENTITY3D_H

#include <string>
#include <vector>
#include "Entity.h"
#include <NFGEMath/Inc/NFGEMath.h>

namespace NFGE::AI
{
	struct Entity3D
	{
		virtual std::string GetTypeName() = 0;
		static int mTotalEntity3D;
		static std::vector<Entity3D*> mAllEntity;	// Every entity ptr in the program will be there. Make sure create entity in top Scope level. (Like Global Scope or Objective pool)

		Entity3D()
		{
			mId = mTotalEntity3D;
			mTotalEntity3D++;
			mAllEntity.push_back(this);
		}

		int mId = 0;
		NFGE::Math::Vector3 position;
	};
}
#endif // !INCLIDED_AI_ENTITY_H
