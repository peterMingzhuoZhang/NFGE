#ifndef INCLUDED_SCENE_HELLORTENTACLE_SCENE_H
#define INCLUDED_SCENE_HELLORTENTACLE_SCENE_H

#include <NFGE/Inc/NFGE.h>
#include "PCTentacle.h"
#include "PATentacle.h"

using namespace NFGE;

class Scene
{
public:
	void Load();
	void Update(float deltaTime);
	void Render(const Graphics::Camera& camera);
	void DebugUI();
	void Unload();
private:
	Graphics::DirectionalLight mLight;
	Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight> mLightBuffer;

	PCTentacle mTentaclePC;
	PATentacle mTentaclePA;
	bool isSelectPA = false;
};

#endif // !INCLUDED_SCENE_HELLOTENTACLE_SCENE_H
