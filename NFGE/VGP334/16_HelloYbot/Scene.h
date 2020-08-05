#ifndef INCLUDED_SCENE_HELLOYBOT_SCENE_H
#define INCLUDED_SCENE_HELLOYBOT_SCENE_H

#include "YBot.h"
#include "PTPlane.h"

using namespace NFGE;

class Scene
{
public:
	Scene();
	void Load();
	void Update(float deltaTime);
	void Render(const Graphics::Camera& camera);
	void DebugUI();
	void Unload();
private:
	Graphics::DirectionalLight* mLight;
	Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight> mLightBuffer;
	
	AI::AIWorld3D mWorld3D;
	Physics::PhysicsWorld mPhysicsWorld;
	//PTPlane mPlane;
	Graphics::Geometry::PlaneMesh mPlane2;
	//YBot mYBot{ mWorld3D };
	std::vector<std::unique_ptr<YBot>> mYBots;

	// Meta system test
	NFGE::GameObjectAllocator mGOAllocator;
	NFGE::GameObjectFactory mGameObjectFactory;
	NFGE::GameObject* mGameObject;

	int mCurrentControlIndex = -1;
};

#endif // !INCLUDED_SCENE_HELLOYBOT_SCENE_H
