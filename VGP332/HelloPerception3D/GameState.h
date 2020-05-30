#ifndef HELLOPERCEPTION3D_GAMESTATE_HEADER
#define HELLOPERCEPTION3D_GAMESTATE_HEADER

#include "AIWorld_3D.h"
#include "SeeWeed.h"
#include "TinyFish.h"
#include "PreditorFish.h"
#include "DestroyerFish.h"

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
	float mDeltaTime;

	AIWorld_3D world_3D;

	const int col = 16;
	const int row = 28;
	const int height = 16;
	float size = 100.0f;
	
	float mWorldWidth;
	float mWorldHeight ;
	float mWorldDepth ;

	const int maxOfObs = 10;

	int mTreeCount = 3;
	std::vector<SeeWeedTree> mTrees;

	void TinyFishBehaviorControl_DebugUI();

};

#endif // !HELLOPERCEPTION3D_GAMESTATE_HEADER

