#ifndef INCLUDED_AI_DECISIONMODULE_H
#define INCLUDED_AI_DECISIONMODULE_H

#include "Goal.h"
#include "GoalComposite.h"
#include "Strategy.h"

namespace NFGE::AI {

template <typename AgentType>
class DecisionModule
{
public:
	typedef Goal<AgentType> GoalType;
	typedef Strategy<AgentType> StrategyType;

	DecisionModule(AgentType& agent);
	~DecisionModule();

	void AddStrategy(StrategyType* strategy);
	void Purge();

	void Update();

private:
	void Arbitrate();

	AgentType& mAgent;
	std::vector<StrategyType*> mStrategies;
	StrategyType* mCurrentStrategy;
	GoalType* mCurrentGoal;
};

#include "DecisionModule.inl"

} // namespace AI

#endif // #ifndef INCLUDED_AI_DECISIONMODULE_H