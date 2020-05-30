#ifndef LACELLECOLLEGE_GAMESTATE_HEADER
#define LACELLECOLLEGE_GAMESTATE_HEADER

#include <NFGE/Inc/NFGE.h>
#include <ImGui/Inc/imgui.h>
#include "GameSettingManager.h"
#include "ObjectPoolManager.h"
#include "LevelManager.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;
using namespace EngineWrapper;

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

	bool isDebugMode = false;

};

#endif // !LACELLECOLLEGE_GAMESTATE_HEADER

