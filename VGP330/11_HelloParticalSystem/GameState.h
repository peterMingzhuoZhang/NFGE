#ifndef HELLOPARTICALSYSTEM_GAMESTATE_H
#define HELLOPARTICALSYSTEM_GAMESTATE_H

#include "PTPartical.h"

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
	int mBlendStateSelected = 0;
	PTParticleEmitter mParticleEmmiter;
	float mDeltaTime;
};

#endif // !HELLOPARTICALSYSTEM_GAMESTATE_H
