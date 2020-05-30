#ifndef INCLIDED_AI_Agent_H
#define INCLIDED_AI_Agent_H

#include "Entity.h"

namespace NFGE::AI
{
	class AIWorld;
	class Agent;

	using Agents = std::vector<Agent*>;

	class Agent : public Entity
	{
	public:
		RTGCN(Agent)
		// RULE: You can only create agents, if you have an AIWorld
		Agent(AIWorld& world);
		~Agent();

		NFGE::Math::Matrix3 LocalToWorld();
		

		AIWorld& mWorld;
		Agents neighborhood;
		//X::Math::Vector2 position{0.0f, 0.0f}; move in to Entity
		NFGE::Math::Vector2 velocity{ 0.0f, 0.0f };
		NFGE::Math::Vector2 destination{ 0.0f, 0.0f };
		NFGE::Math::Vector2 heading{0.0f, 1.0f};
		float radius = 1.0f;
		float neighbourRange = 50.0f;
		float maxSpeed = 1.0f;
		float mass = 1.0f;
	protected:
		void UpdateNeighbor();
	};

} // namespace NFGE::AI

#endif // !INCLIDED_AI_Agent_H
