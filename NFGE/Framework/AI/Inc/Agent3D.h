#ifndef INCLIDED_AI_AGENT3D_H
#define INCLIDED_AI_AGENT3D_H

#include "Entity3D.h"

namespace NFGE::AI
{
	class AIWorld3D;
	class Agent3D;

	using Agent3Ds = std::vector<Agent3D*>;

	class Agent3D : public Entity3D
	{
	public:
		RTGCN(Agent)
			// RULE: You can only create agents, if you have an AIWorld
			Agent3D(AIWorld3D& world);
		~Agent3D();

		NFGE::Math::Matrix4 LocalToWorld();
		NFGE::Math::Matrix4 LocalToWorldMatrix();


		AIWorld3D& mWorld;
		Agent3Ds neighborhood;
		//X::Math::Vector2 position{0.0f, 0.0f}; move in to Entity
		NFGE::Math::Vector3 velocity{ 0.0f, 0.0f, 0.0f };
		NFGE::Math::Vector3 destination{ 0.0f, 0.0f, 0.0f };
		NFGE::Math::Vector3 heading{ 0.0f, 0.0f, 1.0f };
		float radius = 1.0f;
		float neighbourRange = 50.0f;
		float maxSpeed = 1.0f;
		float mass = 1.0f;
	protected:
		void UpdateNeighbor();
	};

} // namespace NFGE::AI

#endif // !INCLIDED_AI_Agent_H
