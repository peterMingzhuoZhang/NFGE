#pragma once

#include "MovementControllerShell.h"
class MovementController : public MovementControllerShell
{
public:

	MovementController(YBot& ybot)
		:mYBot(ybot)
	{

	}

	void Load() override;
	void Update(float deltaTime) override;
	void Set() override;
	
	void DebugUI() override;
private:
	YBot & mYBot;
	int mPlayedAnimationIndex = 0;

	bool mIsPressTurn = false;
	NFGE::Math::Vector2 mMouseLockPos;
};

