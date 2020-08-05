#ifndef HELLO3DGROUPBEHAVIOR_CONE_HEADER
#define HELLO3DGROUPBEHAVIOR_CONE_HEADER

#include "PTGeometry_Shell.h"

struct PTCone : PTGemotry_Shell
{
	void Initialize(int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader);
	bool mIsInitialize = false;
	float mRadius;
private:
	void OnRender() override;
	void OnUpdate(float deltaTime) override;

};

#endif // !HELLO3DGROUPBEHAVIOR_SPHERE_HEADER
