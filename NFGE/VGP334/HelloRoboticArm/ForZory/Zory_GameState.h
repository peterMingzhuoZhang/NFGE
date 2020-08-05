#ifndef HELLOROBOTARM_ZORY_GAMESTATE_H
#define HELLOROBOTARM_ZORY_GAMESTATE_H

#include "Zory_Scene.h"
#include "Zory_RobotArm.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

class Zory_GameState : public NFGE::AppState
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
	Zory_Scene mScene;
	Zory_RobotArm mArm;	
};

#endif // !HELLOROBOTARM_ZORY_GAMESTATE_H

