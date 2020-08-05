#ifndef HELLO3DSTEERING_GAMESTATE_HEADER
#define HELLO3DSTEERING_GAMESTATE_HEADER

#include "World.h"
#include <ImGui/Inc/imgui.h>

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;
using namespace NFGE::AI;

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
	
	Hello3DSteering::World world_3D;
	float mWorldWidth = 1280.0f;
	float mWorldHeight = 720.0f;
	float mWorldDepth = 720.0f;

	const int col = 16;
	const int row = 28;
	const int height = 16;
	const float size = 50.0f;

	std::vector<ConeDude*>dudes;
	int totalDude = 20;

	BehaviorControl behaviorControl;

	bool isDrawDebugLine = false;

	

	

};

#endif // !HELLO3DGROUPBEHAVIRO_GAMESTATE_HEADER

