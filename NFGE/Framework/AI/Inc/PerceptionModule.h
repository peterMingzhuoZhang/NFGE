#ifndef INCLIDED_AI_PERCEPTIONMODULE_H
#define INCLIDED_AI_PERCEPTIONMODULE_H

#include "MemoryRecord.h"
#include "Sensor.h"

namespace NFGE::AI
{
	class Agent;

	using ImportanceCalculator = std::function<void(MemoryRecord&)>;

	class PerceptionModule
	{
	public:
		PerceptionModule(Agent& agent, ImportanceCalculator importanceCaluculartor);
		//~PerceptionModule();

		template<class SensorType>
		SensorType* AddSensor(std::string name);

		template<class SensorType>
		SensorType* GetSensor(const std::string& name);

		void Update(float deltaTime);

		MemoryRecords& GetMemoryRecords()  { return mMemory; }
		void SetMemorySpan(float duration) { mMemorySpan = duration; }
		
	private:
		using SensorMap = std::unordered_map<std::string, std::unique_ptr<Sensor>>;

		Agent& mAgent;
		SensorMap mSensors;
		ImportanceCalculator mCalculator;
		MemoryRecords mMemory;
		float mMemorySpan = 5.0f; // how long to remeber
	};

	template <class SensorType>
	SensorType* PerceptionModule::AddSensor(std::string name)
	{
		//auto[it, succcess] = mSensors.try_emplace(std::move(name), std::make_unique<SensorType>());
		//return succcess ? static_cast<SensorType*>(it->second.get()) : nullptr;

		auto[it, succcess] = mSensors.try_emplace(std::move(name), nullptr);
		if (succcess)
			it->second = std::make_unique<SensorType>();
		return static_cast<SensorType*>(it->second.get());
	}

	template <class SensorType>
	SensorType* PerceptionModule::GetSensor(const std::string& name)
	{
		auto iter = mSensors.find(name);
		return iter == mSensors.end() ? nullptr : static_cast<SensorType*>(iter->second.get());
	}
}

#endif // !INCLIDED_AI_PERCEPTIONMODULE_H
