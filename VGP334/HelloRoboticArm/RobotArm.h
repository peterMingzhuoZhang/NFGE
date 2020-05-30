#ifndef INCLUDED_HELLOROBOTICARM_ROBOTARM_H
#define INCLUDED_HELLOROBOTICARM_ROBOTARM_H

#include "BaseArmSegmentMesh.h"
#include "RobotArmSegment.h"

// Run Time Get Class Name
#define RTGCN(ClassName)\
virtual std::string GetTypeName() override { return #ClassName; } \
static std::string GET_TYPE_NAME() {return #ClassName; }

using namespace NFGE::Graphics;

class RobotArm
{
public:
	virtual std::string GetTypeName() = 0;

	void Load(int maxSeg);
	void UnLoad();
	void Update(float deltaTime);
	void DebugUI();
	void Render(const Camera& camera);

	void AddAnimation();
	void DeleteAnimation(int animationIndex);
	void PlayAniamtion(int animationIndex);
	void StopAniamtion();
	void AddKey(int animationIndex, float time);
	virtual void AddSegment(int segmentType) = 0;
private:
	virtual void OnLoad(int maxSeg) = 0;
	virtual void OnUnLoad() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnDebugUI() = 0;
	virtual void OnRender(const Camera& Camera) = 0;

	virtual void ControlSegment(float deltaTime) = 0;
	virtual void UpdateSegmentTransform(int index, float deltaTime) = 0;
	void ControlSelection();

	// Imgui Render
	void AnimationTabRender();
	void BuildTabRender();
	void UtilTabRender();
	
	// Util
	bool LoadFromFile(const char* fileName);
	bool SaveToFile(const char* fileName);
protected:
	PTCube mGeometry;
	std::vector<RobotBoneMesh> mAllTypeSegaments;

	int mCurrentSelectSegment = 0;
	int mCurrentChildBoneIndex = 0;
	int mMaxSeg = 10;
	std::vector<RobotArmSegment> mRobotArmSegments;

	int mCurrentAnimation = 0;
	float mAnimationTimeCounter = 0.0f;
	bool isAnimateing = false;
	bool isLoop = false;
	float nextFrameTime = 0.0f;

	char mUtilFileName[256] = "[.RobotArmPTC] Or [.RobotArmPTZ]";
	bool mUtilResult = false;
};

#endif // !INCLUDED_HELLOROBOTICARM_ROBOTARM_H
