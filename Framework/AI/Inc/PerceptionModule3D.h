#ifndef INCLIDED_AI_PERCEPTIONMODULE3D_H
#define INCLIDED_AI_PERCEPTIONMODULE3D_H

#include "MemoryRecord.h"
#include "Sensor3D.h"

namespace NFGE::AI
{
	class Agent3D;

	using ImportanceCalculator = std::function<void(MemoryRecord&)>;

	class PerceptionModule3D
	{
	public:
		PerceptionModule3D(Agent3D& agent, ImportanceCalculator importanceCaluculartor);
		//~PerceptionModule();

		template<class SensorType>
		SensorType* AddSensor(std::string name);

		template<class SensorType>
		SensorType* GetSensor(const std::string& name);

		void Update(float deltaTime);

		MemoryRecords& GetMemoryRecords()  { return mMemory; }

		void SetMemorySpan(float duration) { mMemorySpan = duration; }
		
	private:
		using SensorMap = std::unordered_map<std::string, std::unique_ptr<Sensor3D>>;

		Agent3D& mAgent;
		SensorMap mSensors;
		ImportanceCalculator mCalculator;
		MemoryRecords mMemory;
		float mMemorySpan = 5.0f; // how long to remeber
	};

	template <class SensorType>
	SensorType* PerceptionModule3D::AddSensor(std::string name)
	{
		auto[it, succcess] = mSensors.try_emplace(std::move(name), std::make_unique<SensorType>());
		return succcess ? static_cast<SensorType*>(it->second.get()) : nullptr;
	}

	template <class SensorType>
	SensorType* PerceptionModule3D::GetSensor(const std::string& name)
	{
		auto iter = mSensors.find(name);
		return iter == mSensors.end() ? nullptr : static_cast<SensorType*>(iter->second.get());
	}
}

#endif // !INCLIDED_AI_PERCEPTIONMODULE_H
