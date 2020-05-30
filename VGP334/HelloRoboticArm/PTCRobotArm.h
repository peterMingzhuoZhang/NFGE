#ifndef INCLUDED_HELLOROBOTICARM_PTCROBOTARM_H
#define INCLUDED_HELLOROBOTICARM_PTCROBOTARM_H

#include "RobotArm.h"

using namespace NFGE::Graphics;

class PTCRobotArm : public RobotArm
{
public:
	RTGCN(PTCRobotArm)
	
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

#endif // !INCLUDED_HELLOROBOTICARM_PTCROBOTARM_H
