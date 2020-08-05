#ifndef HELLO3DGROUPBEHAVIOR_PTDYNAMICCONE_HEADER
#define HELLO3DGROUPBEHAVIOR_PTDYNAMICCONE_HEADER

#include "PTGeometry_Shell.h"

struct DynamicValueData
{
	float currentTimePorpotion;
	float effectRange;
	float bottomY;
	float padding;
};
using DynamicValueBuffer = NFGE::Graphics::TypedConstantBuffer<DynamicValueData>;

struct PTDynamicCone : PTGemotry_Shell
{
	void Initialize(int row, int col, float mRadius, float height, float dynamicDuration, const wchar_t* vertexShader, const wchar_t* pixelShader);
	bool mIsInitialize = false;
	float mRadius;

	DynamicValueBuffer mDynamicValueBuffer;
	float mDynamicCycleDuration;
	float mDynamicCycleCounter;
private:
	void OnRender() override;
	void OnUpdate(float deltaTime) override;
	void OnTerminate() override;

};

#endif // !HELLO3DGROUPBEHAVIOR_PTDYNAMICCONE_HEADER
