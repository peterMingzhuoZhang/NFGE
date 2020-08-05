#ifndef INCLIDED_AI_SENSOR_H
#define INCLIDED_AI_SENSOR_H

#include "MemoryRecord.h"

namespace NFGE::AI{

	class Agent;

	class Sensor
	{
	public:
		virtual ~Sensor() = default;

		virtual void Update(Agent& agent, MemoryRecords& memory, float deltaTime) = 0;
	};

} // namespace NFGE::AI

#endif // !INCLIDED_AI_SENSOR_H