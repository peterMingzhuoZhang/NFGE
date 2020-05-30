//====================================================================================================
// Filename:	GameState.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <GameState> which is a base class of any game state. This allows us to
//				Create game state in NFGE::Engine
//====================================================================================================

#ifndef HELLOPERCEPTION_GAMESTATE_H
#define HELLOPERCEPTION_GAMESTATE_H

#include "SCV.h"
#include "Carrier.h"
#include "Mineral.h"
#include "PolygonGridGraphMap.h"


extern PolygonGridGraphMap polygonMap;


class BuildPolyGraphState : public NFGE::AppState
{
public:
	virtual void Initialize()
	{
		polygonMap.Load((float)GetScreenWidth(), (float)GetScreenHeight(), 32.0f);
		myCamera2D = Camera2D(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenWidth()));
	}
	virtual void Terminate() 
	{
	}

	virtual void Update(float deltaTime)
	{
		polygonMap.Update(deltaTime);
		if (polygonMap.mIsBuilt)
		{
			polygonMap.AddingWall();
			polygonMap.AddingObs();
		}

	};
	virtual void Render()
	{
		polygonMap.Render(myCamera2D);
	}
	virtual void DebugUI() 
	{
		polygonMap.DebugUI(myCamera2D);
		ImGui::Begin("State Control");
		if(ImGui::Button("Ready"))
		{
			NFGEApp::ChangeState("GameState_1");
		}
		ImGui::End();
	}

private:
	Camera2D myCamera2D;
};

class MainGameState : public NFGE::AppState
{
public:
	MainGameState()
	{
		for (int i = 0; i < 6; ++i)
		{
			mScvs.push_back(new SCV(world));
		}
		//TODO add Minerals
		for (int i = 0; i < 2; ++i)
		{
			mCarriers.push_back(new Carrier(world));
		}
		for (int i = 0; i < 6; ++i)
		{
			mMinerals.push_back(new Mineral());
		}
	}
	virtual void Initialize()
	{
		polygonMap.BindTheAIWorld(world);

		for (auto& scv : mScvs)
		{
			scv->Initialize(&polygonMap);
		}
		//TODO add Minerals
		for (auto& carrier : mCarriers)
		{
			carrier->Initialize(&polygonMap);
			world.RegisterEntity<Carrier>((carrier));
		}
		for (auto& mineral : mMinerals)
		{
			mineral->Load(NFGE::Math::Vector2{32.0f,32.0f}, &polygonMap);
			world.RegisterEntity<Mineral>((mineral));
		}

		myCamera2D = Camera2D(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenWidth()));
	}
	virtual void Terminate() 
	{
		for (int i = 0; i < 6; ++i)
		{
			delete mScvs[i];
		}
		//TODO add Minerals
		for (int i = 0; i < 2; ++i)
		{
			delete mCarriers[i];
		}
		for (int i = 0; i < 6; ++i)
		{
			delete mMinerals[i];
		}
	}

	virtual void Update(float deltaTime)
	{
		polygonMap.Update(deltaTime);
		for (auto& scv : mScvs)
		{
			scv->Update(deltaTime);
		}

		for (auto& carrier : mCarriers)
		{
			carrier->Update(deltaTime);
		}

		for (auto& mineral : mMinerals)
		{
			mineral->Update(deltaTime);
		}
	}
	virtual void Render() 
	{
		polygonMap.Render(myCamera2D);
		for (auto& scv : mScvs)
		{
			scv->Render();
		}

		for (auto& carrier : mCarriers)
		{
			carrier->Render();
		}

		for (auto& mineral : mMinerals)
		{
			mineral->Render(myCamera2D);
		}
	}
	virtual void DebugUI()
	{
		polygonMap.DebugUI(myCamera2D);
	}
private:
	Camera2D myCamera2D;
	AI::AIWorld world{};
	std::vector<SCV*> mScvs;
	std::vector<Carrier*> mCarriers;
	std::vector<Mineral*> mMinerals;
};


#endif // !HELLOPERCEPTION_GAMESTATE_H