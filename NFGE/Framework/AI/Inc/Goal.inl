template <typename AgentType>
Goal<AgentType>::Goal(AgentType& agent)
	: mAgent(agent)
	, mStatus(Status::Inactive)
{
}

template <typename AgentType>
Goal<AgentType>::~Goal()
{
}

template <typename AgentType>
void Goal<AgentType>::ActivateIfInactive()
{
	if (mStatus == Status::Inactive)
	{
		Activate();
	}
}