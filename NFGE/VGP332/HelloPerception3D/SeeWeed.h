#ifndef HELLOPERCEPTION3D_SEEWEED_HEADER
#define HELLOPERCEPTION3D_SEEWEED_HEADER

#include "PTEntity_Shell.h"


using namespace NFGE::Math;

struct SeeWeed;
struct SeeWeedTree
{
	void Initialize(float seeWeedEcoVal, AI::AIWorld3D& world, float worldWidth, float worldHeight, float worldDepth);
	void Terminate();
	void Reset(const Vector3& newPos, float seeWeedEcoVal);
	void Update(float deltaTime );
	void Render();
	void DebugDraw();

	AI::Graph<SeeWeed*> mSeeWeedGraph;
	int mTotalSeeWeed;
	const int maxNeighbor = 7;
	Vector3 position;
	float mWorldWidth;
	float mWorldHeight;
	float mWorldDepth;

	std::vector<SeeWeed*> mAllSeeWeed;

private:
	void ResetPosition();
};

//spring
struct Spring
{
	int neighbor;
	float springPower;
	bool operator==(const Spring& other )
	{
		return (neighbor == other.neighbor);
	}
};
struct SeeWeed : PTEntity_Shell
{
	RTGCN(SeeWeed)
	//- static ------------------------------------------------------
	static PTSphere mSphere;
	//---------------------------------------------------------------

	SeeWeed(AI::AIWorld3D& world);

	void Initialize(float width, float height, float depth);
	void Terminate();

	void Load();
	void OnUpdate(float deltaTime);

	void Render();

	void DebugDraw();

	std::unique_ptr<AI::SteeringModule3D> steeringModule;
	NFGE::Graphics::Color mColor = NFGE::Graphics::Colors::Green;

	//float velocity_Y = 0.0f;
	bool mIsGetEatting = false;
	float mEatTimmer = 0.0f;
	float mEatTakeTime = 1.0f;
	Agent3D* whoseEating = nullptr;

	int mInGraphIndex = -1;

	
	std::vector<Spring> mSprings;

	NFGE::Math::Vector3 supposePosition;

	void UpdateSprint(const AI::Graph<SeeWeed*>& graph, float deltaTime);
	void RenderSpringLine(const AI::Graph<SeeWeed*>& graph);
	void GetEat(Agent3D* entityPtr);
private:
	void UpdatePhysics(float deltaTime);
	
	void ClampInToWorld();
};

#endif // !HELLOPERCEPTION3D_SEEWEED_HEADER
