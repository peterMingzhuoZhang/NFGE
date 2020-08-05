#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace NFGE;

AI::PerceptionModule::PerceptionModule(Agent & agent, ImportanceCalculator caculator)
	:mAgent(agent)
	,mCalculator(std::move(caculator))
{

}

void AI::PerceptionModule::Update(float deltaTime)
{
	// Update sensors and refreash/ add memory records
	for (auto& [name, sensor] : mSensors)
		sensor->Update(mAgent, mMemory, deltaTime);
	
	if (mMemory.empty())
		return;

	//for (auto iter = mMemory.begin(); iter != mMemory.end();)
	//{
	//	if (iter->lastRecordTime + mMemorySpan < X::GetTime())
	//		iter = mMemory.erase(iter);
	//	else
	//		++iter;
	//}

	for (auto iter = mMemory.begin(); iter != mMemory.end();)
	{
		//TODO: remove any records older than memory than memory Span
		mCalculator(*iter);

		iter->lastRecordTime += deltaTime;
		if (iter->lastRecordTime > mMemorySpan)
			mMemory.erase(iter++);
		else
			iter++;

		// Calculator importance on new records
		if (mMemory.empty())
			return;

	}

	

	// Sort records by importance
	mMemory.sort([](MemoryRecord& a, MemoryRecord& b)
	{
		return a.importance > b.importance;
	});
}
