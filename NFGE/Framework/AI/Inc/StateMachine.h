#ifndef INCLUDE_NFGE_AI_STATEMACHINE_H
#define INCLUDE_NFGE_AI_STATEMACHINE_H

#include "State.h"

namespace NFGE::AI
{
	template <class AgentType>
	class StateMachine
	{
	public:
		using StateType = State<AgentType>;
		
		StateMachine(AgentType& agent);

		template <class NewStateType>
		void AddState();

		void Update(float deltaTime);
		void ChangeState(std::string stateName);

	private:
		using StateMap = std::unordered_map<std::string, std::unique_ptr<StateType>>;

		AgentType& mAgent;
		StateType* mCurrentState = nullptr;
		StateMap mStates;
	};

	template <class AgentType>
	StateMachine<AgentType>::StateMachine(AgentType& agent)
		:mAgent(agent)
	{
	}

	template <class AgentType>
	template <class NewStateType>
	void StateMachine<AgentType>::AddState()
	{
		static_assert(std::is_base_of<StateType, NewStateType>::value, "[StateMachine] Can only add state for AgentType.");
		auto[state, success] = mStates.emplace(NewStateType::GetName(), std::make_unique<NewStateType>());
		if (!mCurrentState)
		{ 
			mCurrentState = state->second.get();
			mCurrentState->Enter(mAgent);
		}
	}

	template <class AgentType>
	void StateMachine<AgentType>::Update(float deltaTime)
	{
		mCurrentState->Update(mAgent, deltaTime);
	}

	template <class AgentType>
	void StateMachine<AgentType>::ChangeState(std::string stateName)
	{
		auto iter = mStates.find(stateName);
		if (iter == mStates.end())
			return;

		mCurrentState->Exit(mAgent);
		mCurrentState = iter->second.get();
		mCurrentState->Enter(mAgent);
	}

} // namespace NFGE::AI

#endif // !INCLUDE_NFGE_AI_STATEMACHINE_H
