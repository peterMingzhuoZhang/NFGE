//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#include "Precompiled.h"
#include "RigBone.h"

#include "PhysicsWorld.h"
#include "Constraints.h"

using namespace NFGE::Math;

namespace
{
	bool isLeft_custom(Vector3 a, Vector3 b)
	{
		auto cross = Cross(a, b);
		return  cross.x + cross.y + cross.z > 0;
	}
}


NFGE::Physics::RigBone::RigBone(PhysicsWorld& world, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, NFGE::Graphics::Bone* bone)
	: mInitHeading(mInitHeading)
	, mBone(bone)
{
	mIdle_front = world.AddLine({}, { 0.0f,0.0f,1.0f });
	mIdle_front->mVertices.back()->gravityScale = 0.0f;
	mIdle_front->mVertices.front()->gravityScale = 0.0f;
	mSuppose_front = world.AddLine({}, { 0.0f,0.0f,1.0f });
	mSuppose_front->mVertices.back()->gravityScale = 0.0f;
	mSuppose_front->mVertices.front()->gravityScale = 0.0f;

	mIdle_up = world.AddLine({}, { 0.0f,1.0f,0.0f });
	mIdle_up->mVertices.back()->gravityScale = 0.0f;
	mIdle_up->mVertices.front()->gravityScale = 0.0f;
	mSuppose_up = world.AddLine({}, { 0.0f,1.0f,.0f });
	mSuppose_up->mVertices.back()->gravityScale = 0.0f;
	mSuppose_up->mVertices.front()->gravityScale = 0.0f;

	mIdle_right = world.AddLine({}, { 1.0f,0.0f,0.0f });
	mIdle_right->mVertices.back()->gravityScale = 0.0f;
	mIdle_right->mVertices.front()->gravityScale = 0.0f;
	mSuppose_right = world.AddLine({}, { 1.0f,0.0f,.0f });
	mSuppose_right->mVertices.back()->gravityScale = 0.0f;
	mSuppose_right->mVertices.front()->gravityScale = 0.0f;

	mParticles.reserve(4);
	mParticles.push_back(mSuppose_front->mVertices[0]);
	mParticles.push_back(mSuppose_front->mVertices[1]);
	mParticles.push_back(mSuppose_up->mVertices[0]);
	mParticles.push_back(mSuppose_up->mVertices[1]);
	mParticles.push_back(mSuppose_right->mVertices[0]);
	mParticles.push_back(mSuppose_right->mVertices[1]);


	mOri = world.AddParticle(new Particle());
	mOri->SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	mOri->invMass = 1.0f;
	mOri->gravityScale = 0.0f;



	//mCurrentToSuppose = ;
	//world.AddConstraint(new Spring(mSuppose_hori->mVertices.back(), mCurrent->mVertices.back()));
	//mFixedSuppose = new Fixed(mSuppose_hori->mVertices.back());
	//world.AddConstraint(mFixedSuppose);
	//world.AddConstraint(new Arriver(mSuppose_hori->mVertices.back(), mCurrent->mVertices.back()));
	world.AddRigBone(this);
}

void NFGE::Physics::RigBone::Binding(const std::vector<NFGE::Math::Matrix4>& boneMatrixes, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, const NFGE::Math::Matrix4& adjustMat, const NFGE::Math::Matrix4& toWorld)
{
	auto inverseAdjust = Inverse(adjustMat);
	currentBoneMatrix =  boneMatrixes[mBone->index] * adjustMat *toWorld;
	toLocalParentMat = Inverse( boneMatrixes[mBone->parentIndex] * adjustMat *toWorld);
	NFGE::Math::Matrix4 allInOneMat;
	if (mBone->parentIndex != -1)
	{
		allInOneMat =  tPosToParentMatrixes[mBone->index] * boneMatrixes[mBone->parentIndex] * adjustMat * toWorld;
	}
	else
	{
		allInOneMat =  boneMatrixes[mBone->index] * adjustMat * toWorld;
	}

	toWorldMat = allInOneMat;
	//auto allInOneMat = boneMatrixes[mBone->index] * toWorld;

	localAdjustMatrix = Matrix4::sRotationX(adjustToFacingFrontRotation_X) * Matrix4::sRotationY(adjustToFacingFrontRotation_Y) * Matrix4::sRotationZ(adjustToFacingFrontRotation_Z);
	
}

void NFGE::Physics::RigBone::LookTo(const NFGE::Math::Vector3& target)
{
	auto front = Vector3{ 0.0f,0.0f,1.0f } *localAdjustMatrix;
	auto right = Cross(Vector3::YAxis, front);
	auto up = Cross(front, right);

	mIdle_front->mVertices.front()->SetPosition(Vector3::Zero() * toWorldMat);
	mIdle_front->mVertices.back()->SetPosition(-front * toWorldMat);	// direction match with model initial direction
	mIdle_up->mVertices.front()->SetPosition(Vector3::Zero() * toWorldMat);
	mIdle_up->mVertices.back()->SetPosition(up *toWorldMat);
	mIdle_right->mVertices.front()->SetPosition(Vector3::Zero() * toWorldMat);
	mIdle_right->mVertices.back()->SetPosition(right *toWorldMat);

	mOri->SetPosition(mIdle_front->mVertices.front()->position);
	
	mSuppose_up->mVertices.front()->SetPosition(mOri->position);
	mSuppose_front->mVertices.front()->SetPosition(mOri->position);
	mSuppose_right->mVertices.front()->SetPosition(mOri->position);
	
	if (mIsDominateByPhysics)
		return;
	Vector3 dir = Normalize(target - mSuppose_front->mVertices.front()->position) ;
	Vector3 dirRight = Normalize( Cross(Vector3::YAxis, dir));
	Vector3 dirUp = Normalize(Cross(dir, dirRight));

	mSuppose_front->mVertices.back()->SetPosition(mSuppose_front->mVertices.front()->position + dir /** Matrix4::sIdentity().Rotation(dirRight, adjustToFacingFrontRotation_X) * Matrix4::Matrix4::sIdentity().Rotation(dirUp, adjustToFacingFrontRotation_Y) * Matrix4::sIdentity().Rotation(dir,adjustToFacingFrontRotation_Z)*/);
	mSuppose_up->mVertices.back()->SetPosition(mSuppose_up->mVertices.front()->position + dirUp);
	mSuppose_right->mVertices.back()->SetPosition(mSuppose_right->mVertices.front()->position + dirRight);
}

void NFGE::Physics::RigBone::RotateWith(const NFGE::Math::Matrix4& animationToParent)
{
	//TODO
	auto front = Vector3{ 0.0f,0.0f,1.0f } /**localAdjustMatrix*/;
	auto right = Cross(Vector3::YAxis, front);
	auto up = Cross(front, right);

	mIdle_front->mVertices.front()->SetPosition(Vector3::Zero() * toWorldMat);
	mIdle_front->mVertices.back()->SetPosition(-front * toWorldMat);	// direction match with model initial direction
	mIdle_up->mVertices.front()->SetPosition(Vector3::Zero() * toWorldMat);
	mIdle_up->mVertices.back()->SetPosition(up *toWorldMat);
	mIdle_right->mVertices.front()->SetPosition(Vector3::Zero() * toWorldMat);
	mIdle_right->mVertices.back()->SetPosition(right *toWorldMat);

	mOri->SetPosition(Vector3::Zero() * currentBoneMatrix);

	
	mSuppose_up->mVertices.front()->SetPosition(mOri->position);
	mSuppose_front->mVertices.front()->SetPosition(mOri->position);
	mSuppose_right->mVertices.front()->SetPosition(mOri->position);
	

	if (mIsDominateByPhysics)
		return;

	auto front_suppose = Vector3{ 0.0f,0.0f,1.0f } /**localAdjustMatrix*/;
	auto right_suppose = Cross(Vector3::YAxis, front);
	auto up_suppose = Cross(front, right);

	mSuppose_front->mVertices.back()->SetPosition(-front_suppose *currentBoneMatrix);// direction match with model initial direction
	mSuppose_up->mVertices.back()->SetPosition(up_suppose *currentBoneMatrix);
	mSuppose_right->mVertices.back()->SetPosition(right_suppose *currentBoneMatrix);


}



void NFGE::Physics::RigBone::CaculateTransform()
{
	idleDir_front = NFGE::Math::Normalize(mIdle_front->mVertices.back()->position - mIdle_front->mVertices.front()->position);
	auto currentDir_front = NFGE::Math::Normalize(mSuppose_front->mVertices.back()->position - mSuppose_front->mVertices.front()->position);
	idleDir_up = NFGE::Math::Normalize(mIdle_up->mVertices.back()->position - mIdle_up->mVertices.front()->position);
	auto currentDir_up = NFGE::Math::Normalize(mSuppose_up->mVertices.back()->position - mSuppose_up->mVertices.front()->position);
	idleDir_right = NFGE::Math::Normalize(mIdle_right->mVertices.back()->position - mIdle_right->mVertices.front()->position);
	auto currentDir_right = NFGE::Math::Normalize(mSuppose_right->mVertices.back()->position - mSuppose_right->mVertices.front()->position);
	if (mIsLooking)
	{
		currentDir_right = Normalize(Cross(Vector3::YAxis, currentDir_front));
		currentDir_up = Normalize(Cross(currentDir_front, currentDir_right));
	}
	

	// Not using eurlarAngle because it is free at all three rotation axis hard to control with constrains.
	//Vector3 eurlarAngle = NFGE::Math::GetEular( NFGE::Math::QuaternionFromTo(idleDir_front, currentDir_front));


	auto toWorldRotMat = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(idleDir_front, Vector3::YAxis));

	auto toLocalMatrix = NFGE::Math::Inverse(toWorldRotMat);
	if (mIsLooking)
	{
		toLocalMatrix = toLocalParentMat;
	}

	idleDir_front = TransformNormal(idleDir_front, toLocalMatrix);
	currentDir_front = TransformNormal(currentDir_front, toLocalMatrix);
	idleDir_up = TransformNormal(idleDir_up, toLocalMatrix);
	currentDir_up = TransformNormal(currentDir_up, toLocalMatrix);

	idleDir_right = TransformNormal(idleDir_right, toLocalMatrix);
	currentDir_right = TransformNormal(currentDir_right, toLocalMatrix);

	//Constrix X rotation--- 
	auto xCross = Cross({ 0.0f, idleDir_up.y, idleDir_up.z }, { 0.0f, currentDir_up.y,currentDir_up.z });
	bool xRotDir = xCross.x > 0.0f;
	float xRotRad = acosf(Dot(Normalize({ 0.0f,idleDir_up.y, idleDir_up.z }), Normalize({ 0.0f,currentDir_up.y, currentDir_up.z })));
	if (isnan(xRotRad) || (idleDir_up.y - idleDir_up.z) * (idleDir_up.y - idleDir_up.z) < 0.0000001f) // avoid Gambolock
	{
		xCross = Cross({ 0.0f, idleDir_front.y, idleDir_front.z }, { 0.0f, currentDir_front.y,currentDir_front.z });
		xRotDir = xCross.x > 0.0f;
		xRotRad = acosf(Dot(Normalize({ 0.0f,idleDir_front.y, idleDir_front.z }), Normalize({ 0.0f,currentDir_front.y, currentDir_front.z })));
		
	}

	mRotationAngleX_noConstrain = xRotRad;
	//if (xRotRad >= Constants::Pi * 0.5f) xRotRad = Constants::Pi - xRotRad;
	if (!xRotDir)
	{
		if (xRotRad > mXMaxRad)
			mSupposeXRotation = mXMaxRad;
		else
			mSupposeXRotation = xRotRad;

	}
	else
	{
		if (xRotRad > mXMinRad)
			mSupposeXRotation = -mXMinRad;
		else
			mSupposeXRotation = -xRotRad;
	}

	if (isNeedDirectionFlipX && mIsLooking)
	{
		if (isFlipInSmallAngle_X)
		{
			bool isFlipX = (Dot(idleDir_front, currentDir_front) > 0.0f);
			if (isFlipX)
			{
				mSupposeXRotation *= -1.0f;
			}
		}
		else
		{
			bool isFlipX = (Dot(idleDir_front, currentDir_front) < 0.0f);
			if (isFlipX)
			{
				mSupposeXRotation *= -1.0f;
			}
		}
	}
	

	if (isNeedFlipX&& mIsLooking)
	{
		mSupposeXRotation *= -1.0f;
	}

	//Constrix Y rotation--- 
	auto yCross = Cross({ idleDir_front.x, 0.0f, idleDir_front.z }, { currentDir_front.x, 0.0f, currentDir_front.z });
	bool yRotDir = yCross.y > 0.0f;
	float yRotRad = acosf(Dot(Normalize({ idleDir_front.x, 0.0f, idleDir_front.z }), Normalize({ currentDir_front.x , 0.0f, currentDir_front.z })));
	if (isnan(yRotRad) || (idleDir_front.x - idleDir_front.z) * (idleDir_front.x - idleDir_front.z) < 0.0000001f) // avoid Gambolock
	{
		yCross = Cross({ idleDir_right.x, 0.0f, idleDir_right.z }, { currentDir_right.x, 0.0f, currentDir_right.z });
		yRotDir = yCross.y > 0.0f;
		yRotRad = acosf(Dot(Normalize({ idleDir_right.x, 0.0f, idleDir_right.z }), Normalize({ currentDir_right.x , 0.0f, currentDir_right.z })));
	}

	mRotationAngleY_noConstrain = yRotRad;
	if (yRotDir)
	{
		if (yRotRad > mYMaxRad)
			mSupposeYRotation = mYMaxRad;
		else
			mSupposeYRotation = yRotRad;

	}
	else
	{
		if (yRotRad > mYMinRad)
			mSupposeYRotation = -mYMinRad;
		else
			mSupposeYRotation = -yRotRad;
	}

	if (isNeedDirectionFlipY && mIsLooking)
	{
		if (isFlipInSmallAngle_Y)
		{
			bool isFlipY = (Dot(idleDir_front, currentDir_front) > 0.0f);
			if (isFlipY)
			{
				mSupposeYRotation *= -1.0f;
			}
		}
		else
		{
			bool isFlipY = (Dot(idleDir_front, currentDir_front) < 0.0f);
			if (isFlipY)
			{
				mSupposeYRotation *= -1.0f;
			}
		}
	}

	if (isNeedFlipY&& mIsLooking)
	{
		mSupposeYRotation *= -1.0f;
	}
 
	//Constrix Z rotation--- 
	auto zCross = Cross({ idleDir_right.x, idleDir_right.y , 0.0f }, { currentDir_right.x, currentDir_right.y , 0.0f });
	bool zRotDir = zCross.z > 0.0f;
	float zRotRad = acosf(Dot(Normalize({ idleDir_right.x, idleDir_right.y , 0.0f }), Normalize({ currentDir_right.x , currentDir_right.y , 0.0f })));
	if (isnan(zRotRad) || (idleDir_right.x - idleDir_right.y) * (idleDir_right.x - idleDir_right.y) < 0.0000001f) // avoid Gambolock
	{
		zCross = Cross({ idleDir_up.x, idleDir_up.y , 0.0f }, { currentDir_up.x, currentDir_up.y , 0.0f });
		zRotDir = zCross.z > 0.0f;
		zRotRad = acosf(Dot(Normalize({ idleDir_up.x, idleDir_up.y , 0.0f }), Normalize({ currentDir_up.x , currentDir_up.y , 0.0f })));
	}

	mRotationAngleZ_noConstrain = zRotRad;
	if (isnan(zRotRad))
		zRotRad = 0.0f;
	if (zRotDir)
	{
		if (zRotRad > mZMaxRad)
			mSupposeZRotation = -mZMaxRad; // Negate in the max rad because the model from Maxium is in the right-handed rule system
		else
			mSupposeZRotation = -zRotRad; // Negate in the max rad because the model from Maxium is in the right-handed rule system

	}
	else
	{
		if (zRotRad > mZMinRad)
			mSupposeZRotation = mZMinRad;
		else
			mSupposeZRotation = zRotRad;
	}

	if (isNeedDirectionFlipZ && mIsLooking)
	{
		if (isFlipInSmallAngle_Z)
		{
			bool isFlipZ = (Dot(idleDir_front, currentDir_front) > 0.0f);
			if (isFlipZ)
			{
				mSupposeZRotation *= -1.0f;
			}
		}
		else
		{
			bool isFlipZ = (Dot(idleDir_front, currentDir_front) < 0.0f);
			if (isFlipZ)
			{
				mSupposeZRotation *= -1.0f;
			}
		}
	}

	if (isNeedFlipZ&& mIsLooking)
	{
		mSupposeZRotation *= -1.0f;
	}

	mTranslate = NFGE::Math::Translation(
		TransformCoord(mSuppose_front->mVertices.front()->position, toLocalMatrix) -
		TransformCoord(mIdle_front->mVertices.front()->position, toLocalMatrix));

	// because of the right-handed rule system of the Orginal Model
	mTranslate._41 *= -1.0f;
	//mTranslate._42 *= -1.0f;
	mTranslate._43 *= -1.0f;
}

NFGE::Math::Matrix4 NFGE::Physics::RigBone::GetTransform() const
{
	if (mIsLooking)
	{
		
		return mXRotMat * mYRotMat * mZRotMat * mTranslate;
	}
	else
	{
		return mXRotMat * mYRotMat * mZRotMat * mTranslate;
	}
}

NFGE::Math::Matrix4 NFGE::Physics::RigBone::GetModelAdjustTransform() const
{
	return Matrix4::sRotationX(ModelAdjust_X) *Matrix4::sRotationY(ModelAdjust_Y) *Matrix4::sRotationZ(ModelAdjust_Z);
}

NFGE::Math::Matrix4 NFGE::Physics::RigBone::GetModelAdjust_2Transform() const
{
	return Matrix4::sRotationX(ModelAdjust_X_2) *Matrix4::sRotationY(ModelAdjust_Y_2) *Matrix4::sRotationZ(ModelAdjust_Z_2);
}

void NFGE::Physics::RigBone::Update(float deltaTime, bool isLooking, const NFGE::Math::Vector3 & target)
{
	if (isLooking)
	{
		mSupposeXRotation += ModelAdjust_X;
		mSupposeYRotation += ModelAdjust_Y;
		mSupposeZRotation += ModelAdjust_Z;
	}
	//update current rotations
	float percentage = mRotationSpeed * deltaTime;
	if (percentage > 1.0f) percentage = 1.0f;
	mCurrentXRotation += (mSupposeXRotation - mCurrentXRotation) * percentage;
	mCurrentYRotation += (mSupposeYRotation - mCurrentYRotation) * percentage;
	mCurrentZRotation += (mSupposeZRotation - mCurrentZRotation) * percentage;
	mXRotMat = NFGE::Math::Matrix4::sRotationX(mCurrentXRotation);
	mYRotMat = NFGE::Math::Matrix4::sRotationY(mCurrentYRotation);
	mZRotMat = NFGE::Math::Matrix4::sRotationZ(mCurrentZRotation);

	/*if (mIsDominateByPhysics)
	return;*/

	mIsLooking = isLooking;
	if (isLooking)
	{
		LookTo(target);
	}
	else
	{
		RotateWith(mBone->toParentTransform);
	}

	//caculate suppose rotations
	CaculateTransform();
	//msuppose_verticle & msuppose_horiz

	// make sure the suppose follow the constraining setting
	//mSuppose_front->mVertices.back()->SetPosition(mSuppose_front->mVertices.front()->position + (Vector3{ 0.0f,0.0f,-1.0f } *mXRotMat * mYRotMat * mZRotMat * toWorldMat - Vector3::Zero() * toWorldMat));
	//mSuppose_up->mVertices.back()->SetPosition(mSuppose_up->mVertices.front()->position + (Vector3{ 0.0f,1.0f,0.0f } *mXRotMat * mYRotMat * mZRotMat * toWorldMat - Vector3::Zero() * toWorldMat));
	//mSuppose_right->mVertices.back()->SetPosition(mSuppose_right->mVertices.front()->position + (Vector3{ 1.0f,0.0f,0.0f } *mXRotMat * mYRotMat * mZRotMat * toWorldMat - Vector3::Zero() * toWorldMat));
	// todo add zrotation
}

void NFGE::Physics::RigBone::DebugDraw() const
{
	NFGE::Graphics::SimpleDraw::AddSphere(mIdle_front->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Red, 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mIdle_up->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Red, 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mSuppose_front->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Yellow, 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mSuppose_up->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Pink, 4, 2);

	if (mBone->index == 5)
	{
		ImGui::Begin("Head bone rig imfomatiom:");
		ImGui::Text("rotation eurlar angle X -> pitch: %f", RAD2DEG(mRotationAngleX_noConstrain));
		ImGui::Text("rotation eurlar angle Y -> pitch: %f", RAD2DEG(mRotationAngleY_noConstrain));
		ImGui::Text("rotation eurlar angle Z -> pitch: %f", RAD2DEG(mRotationAngleZ_noConstrain));
		ImGui::End();
	}
}

NFGE::Math::Vector3 NFGE::Physics::RigBone::GetPosition() const
{
	return mSuppose_front->mVertices.front()->position;
}

bool NFGE::Physics::RigBone::IsStable() const
{
	bool ret = true;
	if (mLastHitParticle != nullptr)
	{
		ret = (NFGE::Math::DistanceSqr(mLastHitPoint, mLastHitParticle->position) < mOffEffectThrushHoldSqr);
	}

	return ret && (NFGE::Math::MagnitudeSqr(mSuppose_front->mVertices.back()->acceleration) <= mStableThrushHold);
}
