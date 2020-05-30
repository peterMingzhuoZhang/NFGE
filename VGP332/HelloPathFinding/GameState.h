#ifndef INCLUDE_NFGE_HELLOPATHFINDING_GAMESTATE_H
#define INCLUDE_NFGE_HELLOPATHFINDING_GAMESTATE_H

#include "EngineWrapper.h"

using namespace NFGE;
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
	
private:
	Camera2D myCamera2D;
	
};


#endif // !INCLUDE_NFGE_HELLOSTEERINGADVANCE_GAMESTATE_H

