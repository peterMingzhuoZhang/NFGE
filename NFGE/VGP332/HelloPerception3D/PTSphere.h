#ifndef HELLOPERCEPTION3D_SPHERE_HEADER
#define HELLOPERCEPTION3D_SPHERE_HEADER

#include "PTGeometry_Shell.h"

struct PTSphere : PTGemotry_Shell
{
	void Initialize( int row, int col, float mRadius, const wchar_t* vertexShader, const wchar_t* pixelShader);

	bool mIsInitialize = false;
	float mRadius;
private:
	void OnRender() override;
	void OnUpdate(float deltaTime) override;
};

#endif // !HELLOPERCEPTION3D_SPHERE_HEADER
