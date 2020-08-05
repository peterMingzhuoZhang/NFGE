#pragma once

#include "MovementControllerShell.h"
class MovementController_AllDirection : public MovementControllerShell
{
public:
	struct YBOtBlendContext
	{
		float mAccelation = 0.0f;
		float mDecelation = 0.0f;
		float mCurrentSpeed = 0.0f;
		NFGE::Math::Vector3 mFacingDirection = NFGE::Math::Vector3(0.0f, 0.0f, 1.0f);
		NFGE::Math::Vector3 mMovementDirection = NFGE::Math::Vector3(0.0f, 0.0f, 1.0f);
		NFGE::Math::Vector3 mPlayerHeading = NFGE::Math::Vector3(0.0f, 0.0f, 1.0f);

		bool IsNeedToTurn = false;
		bool IsTurning = false;
		bool mIsHeadRotationFreeze = false;
		float mTurnDot = 1.0f;
		NFGE::Math::Vector3 mTempTurnToDirection = NFGE::Math::Vector3(0.0f, 0.0f, 1.0f);
	};
public:
	MovementController_AllDirection(YBot& ybot);

	void Load() override;
	void Update(float deltaTime) override;
	void Set() override;

	void DebugUI() override;
private:
	YBot& mYbot;

	NFGE::Graphics::BlendTree<YBOtBlendContext> mBlendTree;
	YBOtBlendContext* mBlendContext;

	NFGE::Math::Vector2 mMouseLockPos;
	bool mIsMouseNotMovingDirection = true;
};

