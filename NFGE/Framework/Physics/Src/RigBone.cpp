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
	mIdle_front = world.AddLine({}, {0.0f,0.0f,1.0f});
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
	mOri->SetPosition(Vector3{ 0.0f, 0.1f, 0.0f });
	mOri->invMass = 1.0f;
	mOri->gravityScale = 0.0f;
	
	
	
	//mCurrentToSuppose = ;
	//world.AddConstraint(new Spring(mSuppose_hori->mVertices.back(), mCurrent->mVertices.back()));
	//mFixedSuppose = new Fixed(mSuppose_hori->mVertices.back());
	//world.AddConstraint(mFixedSuppose);
	//world.AddConstraint(new Arriver(mSuppose_hori->mVertices.back(), mCurrent->mVertices.back()));
	world.AddRigBone(this);
}

NFGE::Physics::RigBone::RigBone(PhysicsWorld & world, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, NFGE::Graphics::Bone * bone, RigBone * childBone)
	: RigBone(world, tPosToParentMatrixes, bone)
{
	ConnectChildRigBone(world, childBone, tPosToParentMatrixes);
}

void NFGE::Physics::RigBone::Binding(const std::vector<NFGE::Math::Matrix4>& boneMatrixes, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, const NFGE::Math::Matrix4& toWorld)
{
	currentBoneMatrix = boneMatrixes[mBone->index] * toWorld;
	auto allInOneMat = tPosToParentMatrixes[mBone->index] * boneMatrixes[mBone->parentIndex] * toWorld;
	toWorldMat = allInOneMat;
	//auto allInOneMat = boneMatrixes[mBone->index] * toWorld;
	mIdle_front->mVertices.front()->SetPosition (Vector3::Zero() * allInOneMat);
	mIdle_front->mVertices.back()->SetPosition(Vector3{ 0.0f,0.0f,-1.0f } *allInOneMat);	// direction match with model initial direction
	mIdle_up->mVertices.front()->SetPosition(Vector3::Zero() * allInOneMat);
	mIdle_up->mVertices.back()->SetPosition(Vector3{ 0.0f,1.0f,0.0f } *allInOneMat);
	mIdle_right->mVertices.front()->SetPosition(Vector3::Zero() * allInOneMat);
	mIdle_right->mVertices.back()->SetPosition(Vector3{ 1.0f,0.0f,0.0f } *allInOneMat);
}

void NFGE::Physics::RigBone::LookTo(const NFGE::Math::Vector3& target)
{
	mOri->SetPosition(mIdle_front->mVertices.front()->position);
	if (isnan( mOri->position.x ))
	{
		mSuppose_up->mVertices.front()->SetPosition(mOri->position);
		mSuppose_front->mVertices.front()->SetPosition(mOri->position);
		mSuppose_right->mVertices.front()->SetPosition(mOri->position);
	}
	else
	{
		mSuppose_up->mVertices.front()->SetPosition(mOri->position);
		mSuppose_front->mVertices.front()->SetPosition(mOri->position);
		mSuppose_right->mVertices.front()->SetPosition(mOri->position);
	}

	if (mIsDominateByPhysics)
		return;
	Vector3 dir = Normalize(target - mSuppose_front->mVertices.front()->position);
	mSuppose_front->mVertices.back()->SetPosition(mSuppose_front->mVertices.front()->position + dir);
	mSuppose_up->mVertices.back()->SetPosition(mSuppose_up->mVertices.front()->position + Vector3{0.0f,1.0f,0.0f});
	mSuppose_right->mVertices.back()->SetPosition(mSuppose_right->mVertices.front()->position + Vector3{ 1.0f,0.0f,0.0f });
}

void NFGE::Physics::RigBone::RotateWith(const NFGE::Math::Matrix4& animationToParent)
{
	//TODO
	mOri->SetPosition(Vector3::Zero() * currentBoneMatrix);
	if (isnan(mOri->position.x ))
	{
		mSuppose_up->mVertices.front()->SetPosition(mOri->position);
		mSuppose_front->mVertices.front()->SetPosition(mOri->position);
		mSuppose_right->mVertices.front()->SetPosition(mOri->position);
	}
	else
	{
		mSuppose_up->mVertices.front()->SetPosition(mOri->position);
		mSuppose_front->mVertices.front()->SetPosition(mOri->position);
		mSuppose_right->mVertices.front()->SetPosition(mOri->position);
	}
	
	if (mIsDominateByPhysics)
		return;
	
	mSuppose_front->mVertices.back()->SetPosition(Vector3{ 0.0f,0.0f,-1.0f } *currentBoneMatrix);// direction match with model initial direction
	mSuppose_up->mVertices.back()->SetPosition(Vector3{ 0.0f,1.0f,0.0f } *currentBoneMatrix);
	mSuppose_right->mVertices.back()->SetPosition(Vector3{ 1.0f,0.0f,0.0f } *currentBoneMatrix);
	
	
}
 


void NFGE::Physics::RigBone::CaculateRotationTransformAixes()
{
	idleDir_front = /*NFGE::Math::Normalize*/(mIdle_front->mVertices.back()->position - mIdle_front->mVertices.front()->position);
	auto currentDir_front = /*NFGE::Math::Normalize*/(mSuppose_front->mVertices.back()->position - mSuppose_front->mVertices.front()->position);
	idleDir_up = /*NFGE::Math::Normalize*/(mIdle_up->mVertices.back()->position - mIdle_up->mVertices.front()->position);
	auto currentDir_up = /*NFGE::Math::Normalize*/(mSuppose_up->mVertices.back()->position - mSuppose_up->mVertices.front()->position);
	idleDir_right = /*NFGE::Math::Normalize*/(mIdle_right->mVertices.back()->position - mIdle_right->mVertices.front()->position);
	auto currentDir_right = /*NFGE::Math::Normalize*/(mSuppose_right->mVertices.back()->position - mSuppose_right->mVertices.front()->position);

	auto toWorldRotMat = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(idleDir_front, Vector3::YAxis));
	auto toLocalMatrix = NFGE::Math::Inverse(toWorldRotMat);

	idleDir_front = TransformCoord( idleDir_front, toLocalMatrix);
	currentDir_front = TransformCoord( currentDir_front , toLocalMatrix);
	idleDir_up = idleDir_up * toLocalMatrix;
	currentDir_up = currentDir_up * toLocalMatrix;

	idleDir_right = TransformCoord(idleDir_right, toLocalMatrix);
	currentDir_right = TransformCoord(currentDir_right, toLocalMatrix);

	//Constrix X rotation--- 
	auto xCross = Cross({ 0.0f, idleDir_up.y, idleDir_up.z }, { 0.0f, currentDir_up.y,currentDir_up.z });
	bool xRotDir = xCross.x > 0.0f;
	float xRotRad = acosf(Dot(Normalize({ 0.0f,idleDir_up.y, idleDir_up.z }), Normalize({ 0.0f,currentDir_up.y, currentDir_up.z })));
	//if (xRotRad >= Constants::Pi * 0.5f) xRotRad = Constants::Pi - xRotRad;
	if (!xRotDir)
	{
		if (xRotRad > mXMaxRad)
			mSupposeXRotation = (mXMaxRad);
		else
			mSupposeXRotation = (xRotRad);

	}
	else
	{
		if (xRotRad > mXMinRad)
			mSupposeXRotation = (-mXMinRad);
		else
			mSupposeXRotation = (-xRotRad);
	}

	//Constrix Y rotation--- 
	auto yCross = Cross({ idleDir_front.x, 0.0f, idleDir_front.z }, { currentDir_front.x, 0.0f, currentDir_front.z });
	bool yRotDir = yCross.y > 0.0f;
	float yRotRad = acosf(Dot(Normalize({ idleDir_front.x, 0.0f, idleDir_front.z }), Normalize({ currentDir_front.x , 0.0f, currentDir_front.z })));
	if (yRotDir)
	{
		if (yRotRad > mYMaxRad)
			mSupposeYRotation = (mYMaxRad);
		else
			mSupposeYRotation = (yRotRad);

	}
	else
	{
		if (yRotRad > mYMinRad)
			mSupposeYRotation = (-mYMinRad);
		else
			mSupposeYRotation = (-yRotRad);
	}

	//Constrix Z rotation--- 
	auto zCross = Cross({ idleDir_right.x, idleDir_right.y , 0.0f}, { currentDir_right.x, currentDir_right.y , 0.0f});
	bool zRotDir = zCross.z > 0.0f;
	float zRotRad = acosf(Dot(Normalize({ idleDir_right.x, idleDir_right.y , 0.0f}), Normalize({ currentDir_right.x , currentDir_right.y , 0.0f})));
	if (isnan(zRotRad))
		zRotRad = 0.0f;
	if (zRotDir)
	{
		if (zRotRad > mZMaxRad)
			mSupposeZRotation = (-mZMaxRad); // Negate in the max rad because the model from Maxium is in the right-handed rule system
		else
			mSupposeZRotation = (-zRotRad); // Negate in the max rad because the model from Maxium is in the right-handed rule system
	
	}
	else
	{
		if (zRotRad > mZMinRad)
			mSupposeZRotation = (mZMinRad); 
		else
			mSupposeZRotation = (zRotRad); 
	}

}

NFGE::Math::Matrix4 NFGE::Physics::RigBone::GetRotationTransform() const
{
	return mXRotMat * mYRotMat * mZRotMat;
}

void NFGE::Physics::RigBone::ConnectChildRigBone(PhysicsWorld& world, RigBone * childBone, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes)
{
	mChildBoneOri = world.AddParticle(new Particle());
	mChildBoneOri->SetPosition(childBone->GetPosition() + Vector3{ 0.0f, -0.1f, 0.0f });
	mChildBoneOri->gravityScale = 0.0f;
	mChildBoneOri->invMass = 1.0f;
	world.AddConstraint(new Spring(childBone->mOri, mChildBoneOri));

	mChildBoneOri->SetPosition(NFGE::Math::TransformCoord(mChildBoneOri->position, tPosToParentMatrixes[childBone->mBone->index]));
	world.AddConstraint(new Spring(mSuppose_front->mVertices.back(), mChildBoneOri));
	world.AddConstraint(new Spring(mSuppose_front->mVertices.front(), mChildBoneOri));
	world.AddConstraint(new Spring(mSuppose_up->mVertices.back(), mChildBoneOri));
	world.AddConstraint(new Spring(mSuppose_up->mVertices.front(), mChildBoneOri));
	world.AddConstraint(new Spring(mSuppose_right->mVertices.back(), mChildBoneOri));
	world.AddConstraint(new Spring(mSuppose_right->mVertices.front(), mChildBoneOri));

	world.AddConstraint(new Spring(mSuppose_right->mVertices.back(), mSuppose_front->mVertices.back()));
	world.AddConstraint(new Spring(mSuppose_front->mVertices.back(), mSuppose_up->mVertices.back()));
	world.AddConstraint(new Spring(mSuppose_up->mVertices.back(), mSuppose_right->mVertices.back()));

}

void NFGE::Physics::RigBone::Update(float deltaTime, bool isLooking, const NFGE::Math::Vector3 & target)
{
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

	if (isLooking)
	{
		LookTo(target);
	}
	else
	{
		RotateWith(mBone->toParentTransform);
	}

	//caculate suppose rotations
	CaculateRotationTransformAixes();
	//msuppose_verticle & msuppose_horiz

	// make sure the suppose follow the constraining setting
	mSuppose_front->mVertices.back()->SetPosition(mSuppose_front->mVertices.front()->position + (Vector3{0.0f,0.0f,-1.0f} * mXRotMat * mYRotMat * mZRotMat * toWorldMat - Vector3::Zero() * toWorldMat));
	mSuppose_up->mVertices.back()->SetPosition(mSuppose_up->mVertices.front()->position + (Vector3{ 0.0f,1.0f,0.0f } *mXRotMat * mYRotMat * mZRotMat * toWorldMat - Vector3::Zero() * toWorldMat));
	mSuppose_right->mVertices.back()->SetPosition(mSuppose_right->mVertices.front()->position + (Vector3{ 1.0f,0.0f,0.0f } *mXRotMat * mYRotMat * mZRotMat * toWorldMat - Vector3::Zero() * toWorldMat));
	// todo add zrotation
}

void NFGE::Physics::RigBone::DebugDraw() const
{
	NFGE::Graphics::SimpleDraw::AddSphere(mIdle_front->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Red, 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mIdle_up->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Red, 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mSuppose_front->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Yellow , 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mSuppose_up->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Pink, 4, 2);
}

NFGE::Math::Vector3 NFGE::Physics::RigBone::GetPosition() const
{
	return mSuppose_front->mVertices.front()->position; 
}