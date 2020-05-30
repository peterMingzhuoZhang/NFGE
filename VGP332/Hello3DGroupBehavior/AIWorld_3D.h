#ifndef HELLO3DGROUPBEHAVIOR_AIWORLD_3D_HEADER
#define HELLO3DGROUPBEHAVIOR_AIWORLD_3D_HEADER

#include "SphereDude.h"

using namespace NFGE;

struct AIWorld_3D
{
	void Initialize(float width, float height);
	void Terminate();
	void Update();
	void Render();

	AI::AIWorld world;
	NFGE::Graphics::DirectionalLight mLight;
	LightBuffer mLightBuffer;

	float mWorldWidth;
	float mWorldHeight;
};

#endif // !HELLO3DGROUPBEHAVIOR_AIWORLD_3D_HEADER

