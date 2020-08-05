#ifndef HELLOSKELETON_GAMESTATE_H
#define HELLOSKELETON_GAMESTATE_H

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
	//void CameraFollow(Camera& camera);
	void ShowUI();
private:
	float mdeltaTime;

	Scene mScene;
};

#endif // !HELLOSKELETON_GAMESTATE_H

