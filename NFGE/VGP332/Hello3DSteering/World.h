#ifndef HELLO3DSTEERING_AIWORLD_3D_HEADER
#define HELLO3DSTEERING_AIWORLD_3D_HEADER

#include "ConeDude.h"

using namespace NFGE;

namespace Hello3DSteering
{

	struct World
	{
		void Initialize(float width, float height, float depth);
		void Terminate();
		void Update();
		void Render();

		AI::AIWorld3D world;
		NFGE::Graphics::DirectionalLight mLight;
		LightBuffer mLightBuffer;

		float mWorldWidth;
		float mWorldHeight;
		float mWorldDepth;
	};
}

#endif // !HELLO3DGROUPBEHAVIOR_AIWORLD_3D_HEADER

