#ifndef HELLOROBOTARM_GAMESTATE_H
#define HELLOROBOTARM_GAMESTATE_H

#include <NFGE/Inc/NFGE.h>
#include "PTCRobotArm.h"
#include "PTZRobotArm.h"
#include "Scene.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

class GameState : public NFGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void CameraControl(Camera& camera);
	void ShowUI();
private:
	Scene mScene;
	//PTCRobotArm PTCArm;
	//PTZRobotArm PTZArm;
	std::vector<RobotArm*> arms;
	int mCurrentSelected = -1;

	float mDeltaTime;
	
};

#endif // !HELLOROBOTARM_GAMESTATE_H

