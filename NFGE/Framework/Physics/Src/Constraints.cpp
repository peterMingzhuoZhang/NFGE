//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#include "Precompiled.h"
#include "Constraints.h"
#include "Graphics/Inc/SkeletonModel.h"
#include "NFGEMath/Inc/NFGEMath.h"
#include "PhysicsWorld.h"

using namespace NFGE;
using namespace NFGE::Physics;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

namespace
{
	inline Vector3 GetDisplacement(const PhysicsWorld& world,  Particle* particle)
	{
		const float gamma = 1.0f - world.mSettings.drag;
		const float timeStepSqr = Math::Sqr(world.mSettings.timeStep);

		Math::Vector3 displacement = (particle->position - particle->lastPosition) * gamma + (particle->acceleration * timeStepSqr);
		return displacement;
	}
}

Fixed::Fixed(Particle* p)
	: Fixed(p, p->position)
{
}

Fixed::Fixed(Particle* p, const Math::Vector3& position)
	: mParticle(p)
	, mPosition(position)
{
}

void Fixed::Apply() const
{
	mParticle->SetPosition(mPosition);
}

void Fixed::DebugDraw() const
{
	Graphics::SimpleDraw::AddAABB(mPosition, mParticle->radius, Colors::Green);
}

Spring::Spring(Particle* a, Particle* b, float restLength)
	: mParticleA(a)
	, mParticleB(b)
	, mRestLength(restLength)
{
	if (mRestLength <= 0.0f)
		mRestLength = Math::Distance(a->position, b->position);
}

void Spring::Apply() const
{ 
	Math::Vector3 delta = mParticleB->position - mParticleA->position;
	float dist = Math::Magnitude(delta);
	float diff = (dist - mRestLength) / (dist * (mParticleA->invMass + mParticleB->invMass));
	if (isnan(diff))
	{
		return;
	}
	mParticleA->position += delta * diff * mParticleA->invMass;
	mParticleB->position -= delta * diff * mParticleB->invMass;
}

void Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(mParticleA->position, mParticleB->position, Colors::Green);
}

NFGE::Physics::Arriver::Arriver(Particle * a, Particle * b, float resetLength)
	: mParticleA(a)
	, mParticleB(b)
	, mRestLength(resetLength)
{
	ASSERT(!NFGE::Math::IsZero(resetLength), "[BFGE::Physics::Constraints::Arriver]Can not have resetLength 0");
}

void NFGE::Physics::Arriver::Apply() const
{
	Math::Vector3 delta = mParticleA->position - mParticleB->position;
	float dist = Math::Magnitude(delta);
	float diff = dist - mRestLength;
	float percentage;
	if (diff <= 0.0f)
	{
		diff = 0.0f;
		percentage = 0.0f;
	}
	else
	{
		percentage = 1.0f - mRestLength / diff;
	}

	Vector3 travelDist = delta * (percentage/* * mSpeedModifier*/);
	mParticleB->SetPosition(mParticleB->position + (travelDist));
}

void NFGE::Physics::Arriver::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(mParticleA->position, mParticleB->position, Colors::Chocolate);
}

NFGE::Physics::JointRotation::JointRotation(PhysicsWorld& world, RagPart * ragPart, NFGE::Graphics::SkeletonModel* skeleton, float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
	: mRagPart(ragPart)
	, mWorld(world)
	, mSkeleton(skeleton)
	, mXMin(xMin)
	, mXMax(xMax)
	, mYMin(xMin)
	, mYMax(xMax)
	, mZMin(xMin)
	, mZMax(xMax)
{

}

void NFGE::Physics::JointRotation::Apply() const
{
	auto adjustMatrix = mSkeleton->mContext.finalAdjustMatrix;
	auto toWorldMatrix = mSkeleton->mContext.finalToWorld;

	// TODO:: use tpos to constrain rotation in Future
	//auto boneMatrix = mSkeleton->mContext.tPosToParentMatrix[mRagPart->mBoneIndex] * mSkeleton->mContext.boneMatrix[mSkeleton->mContext.mSkeleton.bones[mRagPart->mBoneIndex]->parentIndex];
	auto boneMatrix = mRagPart->mIdleToParentMatrix * mSkeleton->mContext.boneMatrix[mSkeleton->mContext.mSkeleton.bones[mRagPart->mBoneIndex]->parentIndex];
	auto finalToWorldMat = NFGE::Math::Inverse(adjustMatrix) * toWorldMatrix;

	auto transformMatrix = adjustMatrix * boneMatrix * finalToWorldMat;

	float halfLength = mRagPart->mLength * 0.5f;
	float halfWidth = mRagPart->mWidth * 0.5f;
	float halfHeight = mRagPart->mHeight * 0.5f;

	idleBUL = NFGE::Math::Vector3{ -halfLength, halfHeight, halfWidth } * transformMatrix;
	idleBBL = NFGE::Math::Vector3{ -halfLength, -halfHeight, halfWidth } * transformMatrix;
	idleBBR = NFGE::Math::Vector3{ halfLength, -halfHeight, halfWidth } * transformMatrix;
	idleFBL = NFGE::Math::Vector3{ -halfLength, -halfHeight, -halfWidth } * transformMatrix;


	//auto idleUpVector = NFGE::Math::Normalize(idleBUL - idleBBL);
	//auto idelRightVector = -NFGE::Math::Normalize(idleBBR - idleBBL);
	//auto idleFrontVector = NFGE::Math::Normalize(idleFBL - idleBBL);

	auto idleFrontVector = NFGE::Math::Normalize(idleFBL - idleBBL);
	auto idelRightVector = NFGE::Math::Normalize(Cross(NFGE::Math::Normalize(idleBUL - idleBBL), idleFrontVector));
	auto idleUpVector = NFGE::Math::Normalize(Cross(idleFrontVector, idelRightVector));

	auto currentBUL = mRagPart->pBUL->position;
	auto currentBBL = mRagPart->pBBL->position;
	auto currentBBR = mRagPart->pBBR->position;
	auto currentFBL = mRagPart->pFBL->position;

	//auto currentUpVector = NFGE::Math::Normalize(currentBUL - currentBBL);
	//auto currentRightVector = -NFGE::Math::Normalize(currentBBR - currentBBL);
	//auto currentFrontVector = NFGE::Math::Normalize(currentFBL - currentBBL);

	auto currentFrontVector = Normalize(currentFBL - currentBBL);
	auto currentRightVector = Normalize(Cross(Normalize(currentBUL - currentBBL), currentFrontVector));
	auto currentUpVector = Normalize(Cross(currentFrontVector, currentRightVector));

	

	auto quat = QuaternionFromTo(idleFrontVector, currentFrontVector);
	auto eurlar = NFGE::Math::GetEular(quat);

	float xRotation = 0.0f;
	float yRotation = 0.0f;
	float zRotation = 0.0f;

	bool isNeedReset = false;
	
	//if (xRotation > mXMax)
	//{
	//	xRotation = mXMax;
	//	isNeedReset = true;
	//}
	//if (xRotation < -mXMin)
	//{
	//	xRotation = -mXMin;
	//	isNeedReset = true;
	//}
	//	
	//
	////Constrix Y rotation--- 
	//if (yRotation > mYMax)
	//{
	//	yRotation = mYMax;
	//	isNeedReset = true;
	//}
	//if (yRotation < -mYMin)
	//{
	//	yRotation = -mYMin;
	//	isNeedReset = true;
	//}
	//
	////Constrix Z rotation--- 
	//if (zRotation > mZMax)
	//{
	//	zRotation = mZMax;
	//	isNeedReset = true;
	//}
	//if (zRotation < -mZMin)
	//{
	//	zRotation = -mZMin;
	//	isNeedReset = true;
	//}

	//if (isNeedReset)
	{
		Matrix4 finalMat = adjustMatrix * NFGE::Math::Matrix4::sRotationX(xRotation) *
			NFGE::Math::Matrix4::sRotationY(yRotation) *
			NFGE::Math::Matrix4::sRotationZ(zRotation) *
			boneMatrix * finalToWorldMat;

		auto yVelocity = mRagPart->pCenter->position - mRagPart->pCenter->lastPosition;
		mRagPart->pCenter->SetPosition(Vector3{ 0.0f,0.0f,0.0f } * finalMat);
		mRagPart->pCenter->SetVelocity(yVelocity);
		yVelocity = mRagPart->pFUL->position - mRagPart->pFUL->lastPosition;
		mRagPart->pFUL->SetPosition(NFGE::Math::Vector3{ -halfLength, halfHeight, -halfWidth } * finalMat );
		mRagPart->pFUL->SetVelocity(yVelocity);
		yVelocity = mRagPart->pFUR->position - mRagPart->pFUR->lastPosition;
		mRagPart->pFUR->SetPosition(NFGE::Math::Vector3{ halfLength, halfHeight, -halfWidth } * finalMat);
		mRagPart->pFUR->SetVelocity(yVelocity);
		yVelocity = mRagPart->pFBL->position - mRagPart->pFBL->lastPosition;
		mRagPart->pFBL->SetPosition(NFGE::Math::Vector3{ -halfLength, -halfHeight, -halfWidth } * finalMat );
		mRagPart->pFBL->SetVelocity(yVelocity);
		yVelocity = mRagPart->pFBR->position - mRagPart->pFBR->lastPosition;
		mRagPart->pFBR->SetPosition(NFGE::Math::Vector3{ halfLength, -halfHeight, -halfWidth } * finalMat);
		mRagPart->pFBR->SetVelocity(yVelocity);
		yVelocity = mRagPart->pBUL->position - mRagPart->pBUL->lastPosition;
		mRagPart->pBUL->SetPosition(NFGE::Math::Vector3{ -halfLength, halfHeight, halfWidth } * finalMat);
		mRagPart->pBUL->SetVelocity(yVelocity);
		yVelocity = mRagPart->pBUR->position - mRagPart->pBUR->lastPosition;
		mRagPart->pBUR->SetPosition(NFGE::Math::Vector3{ halfLength, halfHeight, halfWidth } * finalMat);
		mRagPart->pBUR->SetVelocity(yVelocity);
		yVelocity = mRagPart->pBBL->position - mRagPart->pBBL->lastPosition;
		mRagPart->pBBL->SetPosition(NFGE::Math::Vector3{ -halfLength, -halfHeight, halfWidth } * finalMat);
		mRagPart->pBBL->SetVelocity(yVelocity);
		yVelocity = mRagPart->pBBR->position - mRagPart->pBBR->lastPosition;
		mRagPart->pBBR->SetPosition(NFGE::Math::Vector3{ halfLength, -halfHeight, halfWidth } * finalMat);
		mRagPart->pBBR->SetVelocity(yVelocity);
	}
}

void NFGE::Physics::JointRotation::ApplyFromParent(JointRotation* parent)
{
	if (parent != nullptr)
	{
		auto translateMatrix = parent->GetChildTranslationMatrix();
		mRagPart->pCenter->SetPosition(mRagPart->pCenter->position * translateMatrix);
		mRagPart->pFUL->SetPosition(mRagPart->pFUL->position * translateMatrix);
		mRagPart->pFUR->SetPosition(mRagPart->pFUR->position * translateMatrix);
		mRagPart->pFBL->SetPosition(mRagPart->pFBL->position * translateMatrix);
		mRagPart->pFBR->SetPosition(mRagPart->pFBR->position * translateMatrix);
		mRagPart->pBUL->SetPosition(mRagPart->pBUL->position * translateMatrix);
		mRagPart->pBUR->SetPosition(mRagPart->pBUR->position * translateMatrix);
		mRagPart->pBBL->SetPosition(mRagPart->pBBL->position * translateMatrix);
		mRagPart->pBBR->SetPosition(mRagPart->pBBR->position * translateMatrix);
	}

	if (mChildJoint.empty())
	{
		Apply();
		return;
	}

	auto adjustMatrix = mSkeleton->mContext.finalAdjustMatrix;
	auto toWorldMatrix = mSkeleton->mContext.finalToWorld;

	auto parentBoneMatrix = mSkeleton->mContext.boneMatrix[mSkeleton->mContext.mSkeleton.bones[mRagPart->mBoneIndex]->parentIndex];
	auto boneMatrix_tPos = mSkeleton->mContext.tPosToParentMatrix[mRagPart->mBoneIndex] * parentBoneMatrix;
	auto boneMatrix = mSkeleton->mContext.boneMatrix[mRagPart->mBoneIndex];
	auto finalToWorldMat = NFGE::Math::Inverse(adjustMatrix) * toWorldMatrix;

	auto transformMatrix_tPos = adjustMatrix * boneMatrix_tPos * finalToWorldMat;
	auto transformMatrix = adjustMatrix * boneMatrix * finalToWorldMat;
	auto tPosToParent = mSkeleton->mContext.tPosToParentMatrix[mRagPart->mBoneIndex];
	auto toParentMatrix = mSkeleton->mContext.mSkeleton.bones[mRagPart->mBoneIndex]->toParentTransform;

	auto center = NFGE::Math::Vector3{ 0.0f, 0.0f, 0.0f } *parentBoneMatrix;
	auto idleUpVector = NFGE::Math::Normalize(NFGE::Math::Vector3{ 0.0f, 1.0f, 0.0f } *parentBoneMatrix- center);
	auto idleFrontVector = NFGE::Math::Normalize(Cross(NFGE::Math::Vector3::XAxis, idleUpVector));
	if (isnan(idleFrontVector.x)) { idleFrontVector = Vector3::ZAxis; }
	auto idleRightVector = NFGE::Math::Normalize(Cross(idleUpVector, idleFrontVector));

	
	currentUpVector = NFGE::Math::Normalize(mRagPart->pCenter->position - center);
	currentFrontVector = NFGE::Math::Normalize(Cross(NFGE::Math::Vector3::XAxis, currentUpVector));
	if (isnan(currentFrontVector.x)) { currentFrontVector = Vector3::ZAxis; }
	currentRightVector = NFGE::Math::Normalize(Cross(currentUpVector, currentFrontVector));

	auto translationMatrix = NFGE::Math::Matrix4::sTranslation(mRagPart->pCenter->position - center);

	auto quat = QuaternionFromTo(idleFrontVector, currentFrontVector);
	auto eurlar = NFGE::Math::GetEular(quat);

	float xRotation = isnan(eurlar.x) ? 0.0f : eurlar.x;
	float yRotation = isnan(eurlar.y) ? 0.0f : eurlar.y;
	float zRotation = isnan(eurlar.z) ? 0.0f : eurlar.z;

	bool isNeedReset = false;

	if (xRotation > mXMax)
	{
		xRotation = mXMax;
		isNeedReset = true;
	}
	if (xRotation < -mXMin)
	{
		xRotation = -mXMin;
		isNeedReset = true;
	}


	//Constrix Y rotation--- 
	if (yRotation > mYMax)
	{
		yRotation = mYMax;
		isNeedReset = true;
	}
	if (yRotation < -mYMin)
	{
		yRotation = -mYMin;
		isNeedReset = true;
	}

	//Constrix Z rotation--- 
	if (zRotation > mZMax)
	{
		zRotation = mZMax;
		isNeedReset = true;
	}
	if (zRotation < -mZMin)
	{
		zRotation = -mZMin;
		isNeedReset = true;
	}

	

	//if (isNeedReset)
	{
		Matrix4 finalOrientationMat = NFGE::Math::Matrix4::sIdentity() * NFGE::Math::Matrix4::sRotationX(xRotation) *
			NFGE::Math::Matrix4::sRotationY(yRotation) *
			NFGE::Math::Matrix4::sRotationZ(zRotation);
		auto orientationTPos = tPosToParent;
		orientationTPos._41 = 0.0f; orientationTPos._42 = 0.0f; orientationTPos._43 = 0.0f;
		Matrix4 finalMat = finalOrientationMat * translationMatrix;

		currentFrontVector = Vector3{ 0.0f,0.0f,1.0f } * finalOrientationMat;
		currentUpVector = Vector3{ 0.0f,1.0f,0.0f } * finalOrientationMat;
		currentRightVector = Vector3{ 1.0f,0.0f,0.0f } * finalOrientationMat;

		auto newCenter = Vector3{ 0.0f,0.0f,0.0f } * adjustMatrix * finalMat * mSkeleton->mContext.boneMatrix[mSkeleton->mContext.mSkeleton.bones[mRagPart->mBoneIndex]->parentIndex] * finalToWorldMat;
		auto oldCenter = mRagPart->pCenter->position;

		mChildTranslationMatrix = NFGE::Math::Matrix4::sTranslation(newCenter - oldCenter);

		mSkeleton->mContext.mSkeleton.bones[mRagPart->mBoneIndex]->toParentTransform = adjustMatrix * finalMat * Inverse(adjustMatrix);
		//mSkeleton->mContext.boneMatrix[mRagPart->mBoneIndex] = mSkeleton->mContext.tPosToParentMatrix[mRagPart->mBoneIndex] * mSkeleton->mContext.boneMatrix[mSkeleton->mContext.mSkeleton.bones[mRagPart->mBoneIndex]->parentIndex];

		for (auto child : mChildJoint)
		{
			child->ApplyFromParent(this);
		}

	}


}

void NFGE::Physics::JointRotation::DebugDraw() const
{
	//NFGE::Graphics::SimpleDraw::AddSphere({idleBUL,0.2f},NFGE::Graphics::Colors::DarkRed);
	//NFGE::Graphics::SimpleDraw::AddSphere({ idleBBL,0.2f }, NFGE::Graphics::Colors::DarkCyan);
	//NFGE::Graphics::SimpleDraw::AddSphere({ idleBBR,0.2f }, NFGE::Graphics::Colors::DarkGoldenrod);
	//NFGE::Graphics::SimpleDraw::AddSphere({ idleFBL,0.2f }, NFGE::Graphics::Colors::DarkOrange);

	NFGE::Graphics::SimpleDraw::AddSphere({ mRagPart->pCenter->position,0.2f }, NFGE::Graphics::Colors::DarkRed);
	NFGE::Graphics::SimpleDraw::AddSphere({ mRagPart->pCenter->position + currentUpVector * 5.0f,0.2f }, NFGE::Graphics::Colors::DarkCyan);
	NFGE::Graphics::SimpleDraw::AddSphere({ mRagPart->pCenter->position + currentRightVector * 5.0f,0.2f }, NFGE::Graphics::Colors::DarkGoldenrod);
	NFGE::Graphics::SimpleDraw::AddSphere({ mRagPart->pCenter->position + currentFrontVector * 5.0f,0.2f }, NFGE::Graphics::Colors::Violet);
		
		
		
}
