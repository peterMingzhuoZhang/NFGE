#ifndef HELLOPERCEPTION3D_AIWORLD_3D_HEADER
#define HELLOPERCEPTION3D_AIWORLD_3D_HEADER

#include "PTEntity_Shell.h"

using namespace NFGE;

struct AIWorld_3D
{
	void Initialize(float width, float height, float depth);
	void Terminate();
	void Update(float deltaTime);
	void Update(float neighborRange, float deltaTime);
	void Render();
	void DebugDraw();

	AI::AIWorld3D world;
	NFGE::Graphics::DirectionalLight mLight;
	LightBuffer mLightBuffer;

	float mWorldWidth;
	float mWorldHeight;
	float mWorldDepth;

	NFGE::Graphics::Color mCellBoxColor;
	float mAlpha;
};

#endif // !HELLOPERCEPTION3D_AIWORLD_3D_HEADER

