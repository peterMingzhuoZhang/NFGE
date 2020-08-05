#ifndef INCLUDED_SCENE_HELLOROBOTICARM_SCENE_H
#define INCLUDED_SCENE_HELLOROBOTICARM_SCENE_H

#include <NFGE/Inc/NFGE.h>

using namespace NFGE;

class Scene
{
public:
	void Load();
	void Update(float deltaTime);
	void Render(const Graphics::Camera& camera);
	void Unload();
private:
	Graphics::DirectionalLight mLight;
	Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight> mLightBuffer;
};

#endif // !INCLUDED_SCENE_HELLOROBOTICARM_SCENE_H
