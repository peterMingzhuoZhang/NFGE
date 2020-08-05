#ifndef INCLUDED_SCENE_HELLOSKELETON_SCENE_H
#define INCLUDED_SCENE_HELLOSKELETON_SCENE_H

#include "PTSFSkeleton.h"
#include "PTModel.h"

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

	PTSFSkeleton mPTSFSkeleton;
	PTModel mModel;
	SkeletonModel mModel2;

};

#endif // !INCLUDED_SCENE_HELLOSKELETON_SCENE_H
