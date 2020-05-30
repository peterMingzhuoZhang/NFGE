#ifndef INCLUDE_NFGE_AI_STEERINGMODULE_H
#define INCLUDE_NFGE_AI_STEERINGMODULE_H

#include "SteeringBehavior.h"

namespace NFGE::AI
{
	class SteeringModule
	{
	public:
		SteeringModule(Agent& agent);

		template <class BehaviorType>
		BehaviorType* AddBehavior(std::string name);

		template <class BehaviorType>
		BehaviorType* GetBehavior(const std::string& name);

		NFGE::Math::Vector2 Calculate();

	private:
		using BehaviorMap = std::unordered_map<std::string, std::unique_ptr<SteeringBehavior>>;

		Agent& mAgent;
		BehaviorMap mBehaviors;
	};

	template <class BehaviorType>
	BehaviorType* SteeringModule::AddBehavior(std::string name)
	{
		auto [it, succcess] = mBehaviors.try_emplace(std::move(name), std::make_unique<BehaviorType>());
		return succcess ? static_cast<BehaviorType*>(it->second.get()) : nullptr;
	}

	template <class BehaviorType>
	BehaviorType* SteeringModule::GetBehavior(const std::string& name)
	{
		auto iter = mBehaviors.find(name);
		return iter == mBehaviors.end() ? nullptr : static_cast<BehaviorType*>(iter->second.get());
	}
}

#endif // !INCLUDE_NFGE_AI_STEERINGMODULE_H