#include "GameState.h"

namespace
{
	const float WorldWidth = 3000.0f;
	const float WorldHeight = 2000.0f;
	const int BoardLength = 5;
	const float CameraSpeedMultiplyer = 0.005f;
	const float ScoreDisappearTime = 1.0f;
	const int TotalBeansInField = 10;
	
	const NFGE::Graphics::Color BeanColor = NFGE::Graphics::Colors::Pink;

	const float BlurSpeed = 25.0f;

	const int CheatCodeCount = 7;
	const NFGE::Input::KeyCode CheatCheck[] = { NFGE::Input::KeyCode::P,	//Æ¤
												NFGE::Input::KeyCode::T,	//ÌØ
												NFGE::Input::KeyCode::S,	//ÊÇ
												NFGE::Input::KeyCode::W,	//ÎÒ
												NFGE::Input::KeyCode::N,	//ÄÐ
												NFGE::Input::KeyCode::P,	//Åó
												NFGE::Input::KeyCode::Y		//ÓÑ
	};
	int CheatCheckIndex = 0;

	
}

void GameState::Initialize()
{
	mBlurUnit = dynamic_cast<NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur*>(NFGE::Graphics::PostProcessManager::Get()->ActivePostProcessUnit(NFGE::Graphics::PostProcessType::DepthBlur));
	mBlurSpeed = BlurSpeed;

	mWorldWidth = WorldWidth;
	mWorldHeight = WorldHeight;

	world.mGrid.Init(WorldWidth, WorldHeight, 1000);


	mCamera.Load();
	Graphics::GraphicsSystem::Get()->SetClearColor(Graphics::Colors::WhiteSmoke);
	mStartButton.Load({900.0f,500.0f},0,"Start",30.0f,150.0f, 75.0f,NFGE::Graphics::Colors::LightPink, NFGE::Graphics::Colors::LightPink);
	mScoreLable.Load(std::to_string(mScore), 50.0f, { 0.0f,0.0f },Colors::Pink);
	mGameOverLabel.Load("GameOver", 150.0f, NFGE::Math::Vector2{ mCamera.mScreenWidth * 0.5f,200.0f }, Colors::Pink);
	
	mGabby.Initialize(NFGE::Graphics::Colors::LightPink,mWorldWidth,mWorldHeight, "Gabby");
	mGabby.steeringModule->GetBehavior < AI::ArriveBehavior >("Arrive")->SetActive(true);
	mGabby.steeringModule->GetBehavior< AI::ObstacleAvoidBehavior>("ObstacleAvoid")->SetActive(false);

	mPts.reserve(1000);

	mScoreDisappearCounter = ScoreDisappearTime;
	mGameOverDisappearCounter = ScoreDisappearTime;
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	if (mIsDarkMode)
		NFGE::Graphics::GraphicsSystem::Get()->SetClearColor(NFGE::Graphics::Colors::DarkSlateGray);
	else
		NFGE::Graphics::GraphicsSystem::Get()->SetClearColor(NFGE::Graphics::Colors::WhiteSmoke);

	auto mousePosX = NFGE::Input::InputSystem::Get()->GetMouseScreenX();
	auto mousePosY = NFGE::Input::InputSystem::Get()->GetMouseScreenY();
	auto mousePos = NFGE::Math::Vector2{ (float)mousePosX,(float)mousePosY };

	Bluring(deltaTime);
	mBlurUnit->SetBlurPower(mBlurPower);

	mGabby.destination = mCamera.ConvertToWorld( mousePos);

	mGabby.Update(deltaTime);
	for (auto& pt : mPts)
	{
		pt.destination = mGabby.position;
		pt.steeringModule->GetBehavior < AI::FleeBehavior >("Flee")->panicPoint = mGabby.position;

		if (mIsDarkMode)
		{
			behaviorControl.mFleeWeight = 10.0f;
			behaviorControl.mChaseWeight = 0.0f;
		}
		else
		{
			behaviorControl.mFleeWeight = 0.0f;
			behaviorControl.mChaseWeight = 10.0f;
		}

		pt.Update(behaviorControl, deltaTime);
		pt.maxSpeed = 100.0f + mScore * 7.0f;
		if (pt.maxSpeed > 300.0f)
		{
			pt.maxSpeed = 300.0f;
		}

		if (NFGE::Math::Distance(pt.position,mGabby.position) > 700.0f)
		{
			pt.ActionShowName();
		}

		if (!mIsGameOver&&NFGE::Math::Intersect(NFGE::Math::Circle{mGabby.position,mGabby.radius}, NFGE::Math::Circle{ pt.position,pt.radius }))
		{
			//Run GameOver
			StartGameOver();
			break;
		}
	}


	mStartButton.Update(this, mousePos, deltaTime);
	mScoreLable.SetPos(mCamera.GetViewPosition());
	mGameOverLabel.SetPos(mCamera.GetViewPosition() - NFGE::Math::Vector2{0.0f,200.0f});
	

	switch (mCurrentState)
	{
	case State::Start:
	{
		if (mStartButton.IsSelecting())
		{
			ChangeToGameState();
		}
	}
		break;
	case State::Game:
	{
		CheatChecking();
		mGameOverDisappearCounter += deltaTime;
		mScoreDisappearCounter += deltaTime;
		auto displacement = mGabby.position - mCamera.GetViewPosition();
		mCamera.SetViewPos(mCamera.GetViewPosition() + displacement * NFGE::Math::Magnitude(displacement)* CameraSpeedMultiplyer * CameraSpeedMultiplyer);
		
		for (auto& bean : mBeans)
		{
			bean.Update(this, mGabby, deltaTime);
		}

	}
		break;
	case State::GameOver:
	{
		if (mStartButton.IsSelecting())
		{
			ChangeToGameState();
		}
	}
		break;
	default:
		break;
	}
	UpdatePhysicsParticle(deltaTime);
	mPhysicsWorld.Update(deltaTime);
	
}

void GameState::Render()
{

	auto alpha2 = (ScoreDisappearTime - mGameOverDisappearCounter) / ScoreDisappearTime;
	if (alpha2 < 0.0f) alpha2 = 0.0f;
	mGameOverLabel.Render(mCamera, alpha2);
	mStartButton.Render(mCamera);
	auto alpha = (ScoreDisappearTime - mScoreDisappearCounter) / ScoreDisappearTime;
	if (alpha < 0.0f) alpha = 0.0f;
	mScoreLable.SetText(std::to_string(mScore));
	mScoreLable.Render(mCamera, alpha);
	mGabby.Render(mCamera);
	for (auto& pt : mPts)
	{
		pt.Render(mCamera);
	}

	for (auto& bean : mBeans)
	{
		bean.Render(mCamera);
	}


	RenderObstacle();
	RenderPhysicsParticle();
	RenderBoard();
}

void GameState::DebugUI()
{

}

void GameState::UpdatePhysicsParticle(float deltaTime)
{
	int index = 0;

	bool isAllSleep = true;
	for (auto particle : mParticles)
	{
		if (!particle->sleep)
		{
			isAllSleep = false;
			particleLifeTimeCounter[index] += deltaTime;
			if (particleLifeTimeCounter[index] >= effectParticleLifeTime)
			{
				particle->sleep = true;
			}
		}
		++index;
	}

	if (isAllSleep)
	{
		mPhysicsWorld.ClearDynamic();
		particleLifeTimeCounter.clear();
		mParticles.clear();
	}
}

void GameState::RenderPhysicsParticle()
{
	int index = 0;
	for (auto particle : mParticles)
	{
		if (!particle->sleep)
		{
			auto renderPos = mCamera.ConvertToScreen(Vector2{ particle->position.x, particle->position.y });
			NFGE::sApp.DrawScreenCircle(renderPos, 0.75f, NFGE::Graphics::Color{ Colors::Pink.x, Colors::Pink.y,Colors::Pink.z, (effectParticleLifeTime - particleLifeTimeCounter[index]) / effectParticleLifeTime });
		}
		++index;
	}
}

void GameState::RenderObstacle()
{
	auto obstacles = world.GetObstacles();
	int index = 0;
	for (auto& obstacle : obstacles)
	{
		for (size_t i = 0; i < obstacleThickness; i++)
		{
			auto renderPos = mCamera.ConvertToScreen(obstacle.center);
			SimpleDraw::AddScreenCircle(renderPos, obstacle.radius - i * 1.0f, mObstacleColors[mObstacleColoIndices[index]], 32.0f);
		}
		++index;
	}
}

void GameState::RenderBoard()
{
	auto renderScale = mCamera.GetScale();
	auto usingWitdth = mWorldWidth / renderScale;
	auto usingHeight = mWorldHeight / renderScale;
	auto renderPosition = mCamera.ConvertToScreen(NFGE::Math::Vector2{ -usingWitdth * 0.5f,-usingHeight * 0.5f});

	float left = renderPosition.x;
	float top = renderPosition.y;
	float right = renderPosition.x + usingWitdth;
	float bottom = renderPosition.y + usingHeight;

	auto drawColor = NFGE::Graphics::Colors::Pink;
	NFGE::sApp.DrawScreenRect(left, top, right, bottom, drawColor);
	for (int i = 0; i < BoardLength; ++i)
	{
		++left;
		++top;
		--right;
		--bottom;
		NFGE::sApp.DrawScreenRect(left, top, right, bottom, drawColor);
	}
}

void GameState::GeneratePt()
{
	
	mPts.emplace_back(TriangleDude{world});
	auto& pt = mPts.back();
	pt.Initialize(NFGE::Graphics::Colors::CadetBlue, mWorldWidth, mWorldHeight, "Pt.");
	pt.position = mGabby.position + NFGE::Math::RandomUnitCircle() * 700.0f;
	//pt.destination = mGabby.position;
	//auto pursuit = pt.steeringModule->GetBehavior < AI::PursuitBehavior>("Pursuit");
	//pursuit->SetActive(true);
	auto seek = pt.steeringModule->GetBehavior < AI::SeekBehavior >("Seek");
	seek->SetActive(true);
	auto wanderBehavior = pt.steeringModule->GetBehavior < AI::WanderBehavior >("Wander");
	wanderBehavior->SetParama(20.0f, 10.0f, 1.0f);
	wanderBehavior->SetActive(true);
	auto GroupSeparation = pt.steeringModule->GetBehavior < NFGE::AI::GroupSeparationBehavior>("GroupSeparation");
	GroupSeparation->SetActive(true);
	auto GroupAlignment = pt.steeringModule->GetBehavior < NFGE::AI::GroupAlignmentBehavior>("GroupAlignment");
	GroupAlignment->SetActive(true);
	auto GroupCohesion = pt.steeringModule->GetBehavior < NFGE::AI::GroupCohesionBehavior>("GroupCohesion");
	GroupCohesion->SetActive(true);
	auto flee = pt.steeringModule->GetBehavior < NFGE::AI::FleeBehavior>("Flee");
	flee->SetActive(true);

	
}

void GameState::StartGameOver()
{
	mIsGameOver = true;
	mBlurPower = 0.01f;
	mBlurSpeed = BlurSpeed;
}

void GameState::Bluring(float deltaTime)
{
	if (mIsGameOver)
	{
		mBlurPower += mBlurSpeed* deltaTime;
		if (mBlurPower > 10.0f)
		{
			mBlurPower = 10.0f;
			mBlurSpeed *= -1.0f;
			mGameOverDisappearCounter -= ScoreDisappearTime * deltaTime;
			ChangeToGameOverState();
		}
		if (mBlurPower<=0.0f)
		{
			mIsGameOver = false;
		}
	}
}

void GameState::CheatChecking()
{
	if (CheatCheckIndex == CheatCodeCount)
	{
		//TODO darkMode
		mIsDarkMode = !mIsDarkMode;
		CheatCheckIndex = 0;
	}

	auto inputSystem = NFGE::Input::InputSystem::Get();

	if (inputSystem->IsAnyKeyPressed())
	{
		if (inputSystem->IsKeyPressed(CheatCheck[CheatCheckIndex]))
		{
			CheatCheckIndex++;
		}
		else
		{
			CheatCheckIndex = 0;
		}
	}
}

void GameState::ChangeToGameState()
{
	randomGenerateObs();
	mBeans.reserve(TotalBeansInField);
	mScore = 0;
	mIsGameOver = false;
	for ( int i = 0; i < TotalBeansInField; i++)
	{
		Bean bean;
		bean.Load(mWorldWidth,mWorldHeight,BeanColor);
		mBeans.push_back(bean);
	}

	mCurrentState = State::Game;
}

void GameState::ChangeToGameOverState()
{
	mBeans.clear();
	for (auto& pt : mPts)
	{
		world.UnregisterAgent(&pt);
	}
	mPts.clear();
	world.ClearObstacles();

	mGabby.position = mCamera.ConvertToScreen(mGabby.position);
	mCamera.Load();
	mGameOverDisappearCounter = 0.0f;
	mScoreDisappearCounter = 0.0f;
	mStartButton.Reset();
	mGabby.ActionShowName();
	mCurrentState = State::GameOver;
}

void GameState::ChangeToStartState()
{
	mCurrentState = State::Start;
}
