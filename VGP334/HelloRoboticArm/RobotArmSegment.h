#ifndef INCLUDED_HELLOROBOTICARM_ROBOTARMSEGMENT_H
#define INCLUDED_HELLOROBOTICARM_ROBOTARMSEGMENT_H

#include <NFGE/Inc/NFGE.h>

using namespace NFGE::Math;
using namespace NFGE::Graphics;

struct RobotArmSegment
{
public:
	RobotArmSegment() = default;
	RobotArmSegment(int type, const Bone& bone, const Matrix4& boneMat, const Vector3& animaPos, const Quaternion& animaRot);
	RobotArmSegment(const RobotArmSegment& other);
	
	void UnLoad();
	void LoadFromFile(FILE* inputFile);
	void SaveToFile(FILE* outputFile);

public:
	int mArmSegmentType;
	Bone mBone;
	Matrix4 boneMatrix;
	Vector3		mAnitmatePosition;
	Quaternion	mAnitmateRotation;

	std::vector<std::unique_ptr<Animation>> mAnimations;
	AnimationBuilder mAnimationBuilder;

};

#endif // !INCLUDED_HELLOROBOTICARM_ROBOTARMSEGMENT_H
