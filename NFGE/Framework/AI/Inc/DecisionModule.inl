template <typename AgentType>
DecisionModule<AgentType>::DecisionModule(AgentType& agent)
	: mAgent(agent)
	, mCurrentStrategy(nullptr)
	, mCurrentGoal(nullptr)
{
}

template <typename AgentType>
DecisionModule<AgentType>::~DecisionModule()
{
	Purge();
}

template <typename AgentType>
void DecisionModule<AgentType>::AddStrategy(StrategyType* strategy)
{
	mStrategies.push_back(strategy);
}

template <typename AgentType>
void DecisionModule<AgentType>::Purge()
{
	// Remove all strategies
	for (auto strategy : mStrategies)
	{
		X::SafeDelete(strategy);
	}
	mStrategies.clear();

	// Remove current goal
	if (mCurrentGoal != nullptr)
	{
		mCurrentGoal->Terminate();
		X::SafeDelete(mCurrentGoal);
	}
}

template <typename AgentType>
void DecisionModule<AgentType>::Update()
{
	// Evaluate our priorities
	Arbitrate();

	// Update the current action
	if (mCurrentGoal != nullptr)
	{
		mCurrentGoal->Process();
	}
}

template <typename AgentType>
void DecisionModule<AgentType>::Arbitrate()
{
	float mostDesirable = 0.0f;
	StrategyType* bestStrategy = nullptr;

	// Find the most desirable strategy
	for (auto strategy : mStrategies)
	{
		float desirability = strategy->CalculateDesirability();
		if (desirability > mostDesirable)
		{
			mostDesirable = desirability;
			bestStrategy = strategy;
		}
	}

	// Check if our strategy has changed
	if (mCurrentStrategy != bestStrategy)
	{
		// Set new strategy
		mCurrentStrategy = bestStrategy;

		// Exit the current goal
		if (mCurrentGoal != nullptr)
		{
			mCurrentGoal->Terminate();
			X::SafeDelete(mCurrentGoal);
		}

		// Set new goal
		if (mCurrentStrategy != nullptr)
		{
			mCurrentGoal = mCurrentStrategy->CreateGoal();
		}
	}
}