#ifndef HELLOPERCEPTION3D_DESTROYERFISH_HEADER
#define HELLOPERCEPTION3D_DESTROYERFISH_HEADER

#include "PTEntity_Shell.h"

struct DestroyerFish : PTEntity_Shell
{
	RTGCN(DestroyerFish)
	//- static ------------------------------------------------------
	static PTSphere mSphere;
	//---------------------------------------------------------------
	DestroyerFish(AI::AIWorld3D& world);

	void Initialize();
	void Terminate();
	void OnUpdate(float deltaTime);
	void Render();


};

#endif // !HELLOPERCEPTION3D_DESTROYERFISH_HEADER
