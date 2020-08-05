#pragma once
#include <NFGE/Inc/NFGE.h>

#include "PTButton.h"
#include "Bean.h"
#include "Dude.h"
#include "PTCamera.h"
#include "PTAnimation.h"

using namespace NFGE;
using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

const float effectParticleLifeTime = 1.0f;
const int obstacleThickness = 2;

enum class State
{
	Start,
	Game,
	GameOver
};

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
		int howManyObs = 20 + NFGE::Math::Random() % 20;
		for (int i = 0; i < howManyObs; i++)
		{
			float randomX = NFGE::Math::RandomFloat(0.0f, mWorldWidth) - mWorldWidth * 0.5f;
			float randomY = NFGE::Math::RandomFloat(0.0f, mWorldHeight) - mWorldHeight * 0.5f;
			float randomR = NFGE::Math::RandomFloat(20.0f, 50.0f);
			world.AddObstacles(NFGE::Math::Circle(randomX, randomY, randomR));
			mObstacleColoIndices.push_back(NFGE::Math::Random() % mObstacleColors.size());
		}
	}
	void UpdatePhysicsParticle(float deltaTime);
	void RenderPhysicsParticle();
	void RenderObstacle();
	void RenderBoard();
	void GeneratePt();
	void StartGameOver();
	void Bluring(float deltaTime);
	void CheatChecking();

	void ChangeToGameState();
	void ChangeToGameOverState();
	void ChangeToStartState();

private:
	friend class PTButton;
	friend class Bean;


	NFGE::AI::AIWorld world;
	std::vector<TriangleDude> dudes; 

	PTButton mStartButton;

	TriangleDude mGabby{world};
	int mScore;
	PTLabel mScoreLable;
	PTLabel mGameOverLabel;
	float mScoreDisappearCounter = 0.0f;
	float mGameOverDisappearCounter = 0.0f;

	std::vector<Bean> mBeans;

	std::vector<TriangleDude> mPts;
	BehaviorControl behaviorControl;

	PTCamera mCamera;

	NFGE::Physics::PhysicsWorld mPhysicsWorld;
	std::vector<float> particleLifeTimeCounter;
	std::vector<NFGE::Physics::Particle*> mParticles;

	std::vector<size_t> mObstacleColoIndices;
	std::vector<NFGE::Graphics::Color> mObstacleColors = {Graphics::Colors::LightBlue,Graphics::Colors::LightCoral, Graphics::Colors::LightGreen};

	State mCurrentState;

	float mWorldWidth = 0.0f;
	float mWorldHeight = 0.0f;

	NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur* mBlurUnit;
	float mBlurPower = 0.0f;
	float mBlurSpeed = 0.0f;
	bool mIsGameOver = false;

	bool mIsDarkMode = false;
};

