#ifndef INCLUDE_NFGE_AI_STATE_H
#define INCLUDE_NFGE_AI_STATE_H

#include "Common.h"

namespace NFGE::AI
{
	template<class AgentType>
	class State
	{
	public:
		virtual ~State() = default;
		virtual void Enter(AgentType& agent) = 0;
		virtual void Update(AgentType& agent, float deltaTime) = 0;
		virtual void Exit(AgentType& agent) = 0;
	};
} // namespace NFGE::AI

#endif // !INCLUDE_NFGE_AI_STATE_H
