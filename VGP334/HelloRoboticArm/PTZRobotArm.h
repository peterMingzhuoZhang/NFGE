#ifndef INCLUDED_HELLOROBOTICARM_PTZROBOTARM_H
#define INCLUDED_HELLOROBOTICARM_PTZROBOTARM_H

#include "RobotArm.h"

using namespace NFGE::Graphics;

class PTZRobotArm : public RobotArm
{
public:
	RTGCN(PTZRobotArm)

	void AddSegment(int segmentType) override;
private:
	void OnLoad(int maxSeg) override;
	void OnUnLoad() override;
	void OnUpdate(float deltaTime) override;
	void OnDebugUI() override;
	void OnRender(const Camera& Camera) override;
private:
	virtual void ControlSegment(float deltaTime) override;
	virtual void UpdateSegmentTransform(int index, float deltaTime) override;
};

#endif // !INCLUDED_HELLOROBOTICARM_PTZROBOTARM_H
