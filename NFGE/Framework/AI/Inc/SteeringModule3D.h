#ifndef INCLUDE_NFGE_AI_STEERINGMODULE3D_H
#define INCLUDE_NFGE_AI_STEERINGMODULE3D_H

#include "SteeringBehavior3D.h"

namespace NFGE::AI
{
	class SteeringModule3D
	{
	public:
		SteeringModule3D(Agent3D& agent);

		template <class BehaviorType>
		BehaviorType* AddBehavior(std::string name);

		template <class BehaviorType>
		BehaviorType* GetBehavior(const std::string& name);

		NFGE::Math::Vector3 Calculate();

	private:
		using BehaviorMap = std::unordered_map<std::string, std::unique_ptr<SteeringBehavior3D>>;

		Agent3D& mAgent;
		BehaviorMap mBehaviors;
	};

	template <class BehaviorType>
	BehaviorType* SteeringModule3D::AddBehavior(std::string name)
	{
		auto[it, succcess] = mBehaviors.try_emplace(std::move(name), std::make_unique<BehaviorType>());
		return succcess ? static_cast<BehaviorType*>(it->second.get()) : nullptr;
	}

	template <class BehaviorType>
	BehaviorType* SteeringModule3D::GetBehavior(const std::string& name)
	{
		auto iter = mBehaviors.find(name);
		return iter == mBehaviors.end() ? nullptr : static_cast<BehaviorType*>(iter->second.get());
	}
}

#endif // !INCLUDE_NFGE_AI_STEERINGMODULE3D_H