//====================================================================================================
// Filename:	MemoryRecord.h
// Created by:	Peter Chan
// Update by:	Mingzhuo Zhang
// Description:	Has a structure <MemoryRecord> which is a base class of Memory of any entity
//				It contains :	---	A Map of Proporty<variant>(C++17), that for store any info about
//									this memory
//								---	A basic infos for MemoryRecord: lastRecordTime & importance
//								---	A member for entity: position
//====================================================================================================

#ifndef INCLIDED_AI_MEMORYRECORD_H
#define INCLIDED_AI_MEMORYRECORD_H

#include <Core/Inc/Core.h>	// For the Assert

namespace NFGE::AI {

	using Property = std::variant<int, float, NFGE::Math::Vector2, NFGE::Math::Vector3>;

	struct MemoryRecord
	{
		std::unordered_map<std::string, Property> proporties;
		//X::Math::Vector2 lastSeenPosition;
		float lastRecordTime = 0.0f;
		float importance = 0.0f;

		template <class PropertyType>
		PropertyType GetProperty(std::string name);

		template <class PropertyType>
		bool EditProporty(std::string name, const PropertyType& Proporty);

		template <class PropertyType>
		bool AddProperty(std::string name, const PropertyType& porperty);
	};

	using MemoryRecords = std::list<MemoryRecord>;		// TODO:: Change to std::vector in future. Cache fridenly has priority.

	//Template function implemention-----------------------------------
	template <class PropertyType>
	PropertyType MemoryRecord::GetProperty(std::string name)
	{
		auto iter = proporties.find(name);
		ASSERT(iter != proporties.end(), "[MemoryRecord] Can not find property with name: %s. ", name.c_str());
		return std::get<PropertyType>(iter->second);
	}
	
	template <class PropertyType>
	bool MemoryRecord::EditProporty(std::string name, const PropertyType& Proporty)
	{
		auto iter = proporties.find(name);
		if (iter == proporties.end())
			return false;
		else
		{
			iter->second = Proporty;
			return true;
		}
	}

	template <class PropertyType>
	bool MemoryRecord::AddProperty(std::string name, const PropertyType& porperty)
	{
		Property newProperty = porperty;
		auto[it, success] = proporties.try_emplace(std::move(name), std::move(newProperty));
		return success;
	}


} // namespace NFGE::AI 

#endif // !INCLIDED_AI_MEMORYRECORD_H
