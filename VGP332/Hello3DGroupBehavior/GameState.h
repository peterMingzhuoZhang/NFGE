#ifndef HELLO3DGROUPBEHAVIRO_GAMESTATE_HEADER
#define HELLO3DGROUPBEHAVIRO_GAMESTATE_HEADER

#include "AIWorld_3D.h"
#include <ImGui/Inc/imgui.h>

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
	void randomGenerateObs();
	void CameraControl(Camera& camera);
	void ShowUI();
private:
	
	AIWorld_3D world_3D;
	float mWorldWidth = 1280.0f;
	float mWorldHeight = 720.0f;

	const int col = 8;
	const int row = 14;
	const float size = 100.0f;

	std::vector<SphereDude*>dudes;
	int totalDude = 100;

	BehaviorControl behaviorControl;

	bool isDrawDebugLine = false;

	

	

};

#endif // !HELLO3DGROUPBEHAVIRO_GAMESTATE_HEADER

