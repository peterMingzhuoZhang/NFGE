#ifndef NFGE_HELLOYBOT_MOVEMENTCONTROLLER_H
#define NFGE_HELLOYBOT_MOVEMENTCONTROLLER_H

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

#endif // !NFGE_HELLOYBOT_MOVEMENTCONTROLLER_H
