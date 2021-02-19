#ifndef HELLOGROUPBEHAVIRO_GAMESTATE_HEADER
#define HELLOGROUPBEHAVIRO_GAMESTATE_HEADER

#include "TriangleDude.h"
#include <ImGui/Inc/imgui.h>

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;
using namespace NFGE;

class GameState : public NFGE::AppState
{
public:
	void Initialize() override
	{
		randomGenerateObs();

		for (int i = 0; i < totalDude; i++)
		{
			dudes.push_back(new TriangleDude(world));
		}

		world.mGrid.Init((float)NFGE::sApp.GetScreenWidth(), (float)NFGE::sApp.GetScreenHeight(), (int)dudes.size());

		for (int i = 0; i < dudes.size(); i++)
		{
			dudes[i]->Initialize();
			dudes[i]->steeringModule->GetBehavior< NFGE::AI::WanderBehavior >("Wander")->SetActive(true);
			dudes[i]->steeringModule->GetBehavior< NFGE::AI::GroupSeparationBehavior>("GroupSeparation")->SetActive(true);
			dudes[i]->steeringModule->GetBehavior< NFGE::AI::GroupCohesionBehavior>("GroupCohesion")->SetActive(true);
			dudes[i]->steeringModule->GetBehavior< NFGE::AI::GroupAlignmentBehavior>("GroupAlignment")->SetActive(true);
			dudes[i]->steeringModule->GetBehavior< NFGE::AI::ObstacleAvoidBehavior>("ObsAvoid")->SetActive(true);
		}
	}
	void Terminate() override
	{

	}

	void Update(float deltaTime) override
	{
		world.Update(dudes[0]->neighbourRange);

		for (int i = 0; i < dudes.size(); i++)
		{
			dudes[i]->Update(behaviorControl, deltaTime);
			
			Vector2 mousePos = { (float)NFGE::Input::InputSystem::Get()->GetMouseScreenX(),(float)NFGE::Input::InputSystem::Get()->GetMouseScreenY() };
			dudes[i]->UpdateFleeBehavior(isFlee, mousePos, 200.0f, 20.0f);
			
		}

		
		

		auto inputSystem = InputSystem::Get();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			NFGEApp::ShutDown();
			return;
		}

		mDeltaTime = deltaTime;
	}
	void Render() override
	{
		for (int i = 0; i < dudes.size(); i++)
		{
			dudes[i]->mIsDebugDraw = isDrawDebugLine;
			dudes[i]->isDrawTexture = isSpriteRender;
			dudes[i]->Render();
		}

		//---------------------------------------------------------------------------------------------------------------------------
		NFGE::sApp.DrawScreenCircle(dudes[0]->position, dudes[0]->neighbourRange, NFGE::Graphics::Colors::LightPink);

		for (auto& dude1Neighbor : dudes[0]->neighborhood)
		{
			NFGE::sApp.DrawScreenCircle(dude1Neighbor->position, 10.0f, NFGE::Graphics::Colors::HotPink);
		}
		//---------------------------------------------------------------------------------------------------------------------------

		auto& obs = world.GetObstacles();
		for (int i = 0; i < obs.size(); i++)
		{
			NFGE::sApp.DrawScreenCircle(obs[i].center, obs[i].radius, NFGE::Graphics::Colors::Azure);
		}
		
		NFGE::Graphics::SimpleDraw::Render(camera);
	}
	void DebugUI() override
	{
		ShowUI();
	}

private:

private:
	Camera camera;
	NFGE::AI::AIWorld world;
	const int col = 8;
	const int row = 14;
	const float size = 100.0f;

	std::vector<TriangleDude*>dudes;
	int totalDude = 1500;

	BehaviorControl behaviorControl;

	bool isDrawDebugLine = false;
	bool isSpriteRender = true;
	bool isFlee = false;

	float mDeltaTime;

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

	void ShowUI()
	{
		
		ImGui::Begin("Util");
		ImGui::Text("FPS: %f", 1.0f / mDeltaTime);
		ImGui::End();

		ImGui::Begin("Behavior Control");
		ImGui::SliderFloat("Wander weight:", &(behaviorControl.mWanderWeight), 0.0f, 10.0f);
		ImGui::SliderFloat("Group Alignment weight:", &(behaviorControl.mAlignmentWeight), 0.0f, 10.0f);
		ImGui::SliderFloat("Group Separation weight:", &(behaviorControl.mSeparationWeight), 0.0f, 10.0f);
		ImGui::SliderFloat("Group Cohesion weight:", &(behaviorControl.mCohesionWeight), 0.0f, 10.0f);
		ImGui::SliderFloat("Group ObsAvoid weight:", &(behaviorControl.mObsAvoidWeight), 0.0f, 10.0f);
		ImGui::SliderFloat("Group Neighbor radius:", &(behaviorControl.mNeighborRadius), 0.0f, 500.0f);
		ImGui::End();

		ImGui::Begin("ShortCut");
		if (ImGui::Button("Spread Out!"))
		{
			behaviorControl.mWanderWeight = 2.0f;
			behaviorControl.mAlignmentWeight = 1.0f;
			behaviorControl.mSeparationWeight = 8.0f;
			behaviorControl.mCohesionWeight = 0.0f;
		}
		if (ImGui::Button("Group In!"))
		{
			behaviorControl.mWanderWeight = 2.0f;
			behaviorControl.mAlignmentWeight = 1.0f;
			behaviorControl.mSeparationWeight = 0.0f;
			behaviorControl.mCohesionWeight = 8.5f;
		}
		if (ImGui::Button("Free Roam!"))
		{
			behaviorControl.mWanderWeight = 4.5f;
			behaviorControl.mAlignmentWeight = 0.0f;
			behaviorControl.mSeparationWeight = 0.0f;
			behaviorControl.mCohesionWeight = 0.0f;
		}
		if (ImGui::Button("Fish Like!"))
		{
			behaviorControl.mWanderWeight = 3.5f;
			behaviorControl.mAlignmentWeight = 4.0f;
			behaviorControl.mSeparationWeight = 1.0f;
			behaviorControl.mCohesionWeight = 3.0f;
		}

		if (ImGui::Button("Toggle Flee"))
		{
			isFlee = isFlee ? false : true;
		}

		ImGui::End();

		ImGui::Begin("Eyes Blind Alert!");
		if (ImGui::Button("Toggle Debug Line"))
		{
			
			isDrawDebugLine = isDrawDebugLine ? false : true;
		}
		if (ImGui::Button("Toggle Sprite Render"))
		{

			isSpriteRender = isSpriteRender? false : true;
		}

		if (isDrawDebugLine)
		{
			world.DebugUI();
		}
		ImGui::End();
	}

};

#endif // !HELLOGROUPBEHAVIRO_GAMESTATE_HEADER

