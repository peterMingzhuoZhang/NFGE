#ifndef HELLOPERCEPTION3D_TINYFISH_HEADER
#define HELLOPERCEPTION3D_TINYFISH_HEADER

#include "PTCone.h"
#include "SeeWeed.h"

struct TinyFish : PTEntity_Shell
{
	RTGCN(TinyFish)
		//- static ------------------------------------------------------
		static PTCone mGeometry;
	static Graphics::Color mRegularColor;
	static Graphics::Color mHappyColor;
	static Graphics::Color mPanicColor;
	static Graphics::Color mDefaultAmbient;
	//---------------------------------------------------------------

	TinyFish(AI::AIWorld3D& world);

	void Initialize(float width, float height, float depth);
	void Terminate();

	void OnUpdate(float deltaTime) override;

	void Render();

	void DebugDraw();

	void ResetPosition(const Vector3& pos);
	bool TryCollectSeeWeed(SeeWeed& SeeWeed);

	std::unique_ptr<AI::PerceptionModule3D> mPerceptionModule;
	std::unique_ptr<AI::SteeringModule3D> steeringModule;

private:
	std::unique_ptr<AI::StateMachine<TinyFish>> mStateMachine;
	void BornAChild();
};


#endif // !HELLOPERCEPTION3D_TINYFISH_HEADER
