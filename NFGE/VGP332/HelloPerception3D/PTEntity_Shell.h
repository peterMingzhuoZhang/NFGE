#ifndef HELLOPERCEPTION3D_PTENTITY_SHELL
#define HELLOPERCEPTION3D_PTENTITY_SHELL

#include <NFGE/Inc/NFGE.h>
#include "PTSphere.h"
#include "PTDynamicCone.h"
#include "PTEntityManager.h"
#include "PTLerpMachine.h"

using namespace NFGE::AI;
using namespace NFGE::Math;
using namespace NFGE;

// Using selfDefine type --------------------------------------------------------------------------------------------------
struct BehaviorControl
{
	float mSeparationWeight = 1.0f;
	float mAlignmentWeight = 1.0f;
	float mCohesionWeight = 1.0f;

	float mWanderWeight = 1.0f;
	float mObsAvoidWeight = 1.0f;
	float mNeighborRadius = 100.0f;
};

struct PTEntity_Shell : Agent3D
{
	PTEntity_Shell(AIWorld3D& world)
		: Agent3D(world)
		, mDiffuseColorLerper(&mDiffuseColor)
		, mAmbientColorLerper(&mAmbientColor)
	{}

	void Initialize() {}
	void Terminate() {}
	void Update(float deltaTime)
	{
		mDiffuseColorLerper.Update(deltaTime);
		mAmbientColorLerper.Update(deltaTime);
		OnUpdate(deltaTime);
	}
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void DebugDraw() = 0;

	bool mIsActive = false;
	bool mIsDebugDraw = false;

	float worldWidth;
	float worldHeight;
	float worldDepth;
	float mDebugLineHeight = 10.0f;
	float mHeight = 11.0f;
	
	NFGE::Graphics::Color mDiffuseColor;
	NFGE::Graphics::Color mAmbientColor;
	PTLerpMachine<NFGE::Graphics::Color> mDiffuseColorLerper;
	PTLerpMachine<NFGE::Graphics::Color> mAmbientColorLerper;


	BehaviorControl mBehaviorControl{};

	void ClampInScene()
	{
		auto strachWorldWidth = worldWidth + 100.0f;
		auto strachWorldHeight = worldHeight + 100.0f;
		auto strachWorldDepth = worldDepth + 100.0f;
		if ((position.x > strachWorldWidth))
			position.x = static_cast<float>((int)position.x % (int)strachWorldWidth - 100.0f);
		else if ((position.x < 0.0f - 100.0f))
			position.x = strachWorldWidth;
		if ((position.y > strachWorldHeight))
			position.y = static_cast<float>((int)position.y % (int)strachWorldHeight - 100.0f);
		else if ((position.y < 0.0f - 100.0f))
			position.y = strachWorldHeight;
		if ((position.z > strachWorldDepth))
			position.z = static_cast<float>((int)position.z % (int)strachWorldDepth - 100.0f);
		else if ((position.z < 0.0f - 100.0f))
			position.z = strachWorldDepth;
	}
	void Render_2D()
	{
		auto heading2D = Vector2{ heading.x, heading.z };
		auto position2D = Vector2{ position.x, position.z };
		if (!mIsDebugDraw)
			return;
		NFGE::Math::Vector2 forwardPoint = heading2D * radius * 2.0f;
		forwardPoint = forwardPoint + position2D;
		NFGE::Math::Vector2 leftPoint{ -heading.z * radius, heading.x * radius };
		leftPoint -= heading2D * radius * 1.0f;
		leftPoint = leftPoint + position2D;
		NFGE::Math::Vector2 rightPoint{ heading.z * radius, -heading.x * radius };
		rightPoint -= heading2D * radius * 1.0f;
		rightPoint = rightPoint + position2D;

		Vector3  leftPoint_3d = Vector3(leftPoint.x, mDebugLineHeight, leftPoint.y);
		Vector3  rightPoint_3d = Vector3(rightPoint.x, mDebugLineHeight, rightPoint.y);
		Vector3  forwardPoint_3d = Vector3(forwardPoint.x, mDebugLineHeight, forwardPoint.y);

		NFGE::sApp.DrawLine(leftPoint_3d, forwardPoint_3d, mDiffuseColor);
		NFGE::sApp.DrawLine(rightPoint_3d, forwardPoint_3d, mDiffuseColor);
		NFGE::sApp.DrawLine(leftPoint_3d, rightPoint_3d, mDiffuseColor);
	}
	void UpdateBehaviorControl(const BehaviorControl& control)
	{
		mBehaviorControl = control;
	}
};

#endif // !HELLOPERCEPTION3D_PTENTITY_SHELL
