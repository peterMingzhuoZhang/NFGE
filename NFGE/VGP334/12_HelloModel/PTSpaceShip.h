#ifndef HELLOMODEL_PTSPACESHIP_H
#define HELLOMODEL_PTSPACESHIP_H

#include "PTModel.h"

using namespace NFGE;

struct PTSpaceShip : PTModel , AI::Agent3D
{
	// - Test ------------------
	std::unique_ptr<NFGE::Graphics::Animation> mJumpAnimationPtr;
	NFGE::Graphics::AnimationBuilder mAnimationBuilder;
	bool mAnimationActive = false;
	float mAnimationTimer = 0.0f;
	// -------------------------


	PTSpaceShip(AI::AIWorld3D & world)
		:Agent3D(world)
	{

	}
	void Load(const Vector3& position, const Vector3& rotation, const Vector3& scale);
	void Update(float deltaTime);
	void Render(Camera& camera);
	void DebugUI();
	void UnLoad();
	void OnRender(const Camera& camera) override;

private:
	std::unique_ptr<AI::SteeringModule3D> mSteeringModule;
	uint8_t mControlState;
	float mXRotationMagitude;
	float mYRotationMagitude;
	float mDestinationZRotation;
	Vector3 mUp;


	void HookCamera(Camera& camera, float deltaTime);
	void Control(float deltaTime);
	void ShipAction(float deltaTime);
	void UpdateSimplePhysics(float deltaTime);
};

#endif // !HELLOMODEL_PTSPACESHIP_H
