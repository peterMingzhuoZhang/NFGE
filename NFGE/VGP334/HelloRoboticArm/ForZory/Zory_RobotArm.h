#ifndef INCLUDED_HELLOROBOTICARM_ZORY_ROBOTARM_H
#define INCLUDED_HELLOROBOTICARM_ZORY_ROBOTARM_H

#include "Zory_RobotArmMesh.h"

using namespace NFGE::Graphics;
using namespace NFGE::Math;

class Zory_RobotArm
{
public:

	void Load();
	
	void UnLoad();
	
	void Update(float deltaTime);
	
	void DebugUI() {}

	void Render(const Camera& camera);

	void AddSegment();
	
	void ControlSegment(float deltaTime);
private:
	
protected:
	
	Zory_RobotArmMesh mRobotArmMesh;
	
	int mMaxSeg = 3;
	std::vector<Bone> mRobotArmSegments;
	std::vector<Matrix4> boneMatrixes;		// Step 2
};

#endif // !INCLUDED_HELLOROBOTICARM_ZORY_ROBOTARM_H
