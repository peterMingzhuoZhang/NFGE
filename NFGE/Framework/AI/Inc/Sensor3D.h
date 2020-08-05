#ifndef INCLIDED_AI_SENSOR3D_H
#define INCLIDED_AI_SENSOR3D_H

#include "MemoryRecord.h"

namespace NFGE::AI{

	class Agent3D;

	class Sensor3D
	{
	public:
		virtual ~Sensor3D() = default;

		virtual void Update(Agent3D& agent, MemoryRecords& memory, float deltaTime) = 0;
	};

} // namespace NFGE::AI

#endif // !INCLIDED_AI_SENSOR3D_H