#ifndef HELLOMODEL_GAMESTATE_H
#define HELLOMODEL_GAMESTATE_H

#include <NFGE/Inc/NFGE.h>
#include "PTModel.h"
#include "PTSpaceShip.h"

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
	//void CameraFollow(Camera& camera, );
	void ShowUI();
private:
	NFGE::AI::AIWorld3D world;
	PTSpaceShip mSpaceShip{world};
	PTModel mModel_0;
	PTModel mModel_1;
	PTModel mModel_2;

	Camera mShadowCamera;
	Camera* mCurrentCamera;
};

#endif // !HELLOMODEL_GAMESTATE_H

