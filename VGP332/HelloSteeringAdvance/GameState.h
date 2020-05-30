#ifndef INCLUDE_NFGE_HELLOSTEERINGADVANCE_GAMESTATE_H
#define INCLUDE_NFGE_HELLOSTEERINGADVANCE_GAMESTATE_H

#include <NFGE/Inc/NFGE.h>

#include "TriangleDude.h"

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
	void randomGenerateObs()
	{
		int howManyObs = NFGE::Math::Random() % 30;
		for (int i = 0; i < howManyObs; i++)
		{
			float randomX = NFGE::Math::RandomFloat(0.0f, (float)NFGE::sApp.GetScreenWidth());
			float randomY = NFGE::Math::RandomFloat(0.0f, (float)NFGE::sApp.GetScreenHeight());
			float randomR = NFGE::Math::RandomFloat(20.0f, 50.0f);
			world.AddObstacles(NFGE::Math::Circle(randomX, randomY, randomR));
		}
	}
private:
	NFGE::AI::AIWorld world;
	std::vector<TriangleDude> dudes;

};


#endif // !INCLUDE_NFGE_HELLOSTEERINGADVANCE_GAMESTATE_H

