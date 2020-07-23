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


NFGE::Physics::RigBone::RigBone(PhysicsWorld& world, const std::vector<NFGE::Math::Matrix4>& boneMatrixes, NFGE::Graphics::Bone* bone, NFGE::Physics::Particle* parentOriParticle)
	: mInitHeading(mInitHeading)
	, mBone(bone)
{
	mIdle_hori = world.AddLine({}, {0.0f,0.0f,1.0f});
	mIdle_hori->mVertices.back()->gravityScale = 0.0f;
	mIdle_hori->mVertices.front()->gravityScale = 0.0f;
	mSuppose_hori = world.AddLine({}, { 0.0f,0.0f,1.0f });
	mSuppose_hori->mVertices.back()->gravityScale = 0.0f;
	mSuppose_hori->mVertices.front()->gravityScale = 0.0f;

	mIdle_vert = world.AddLine({}, { 0.0f,1.0f,0.0f });
	mIdle_vert->mVertices.back()->gravityScale = 0.0f;
	mIdle_vert->mVertices.front()->gravityScale = 0.0f;
	mSuppose_vert = world.AddLine({}, { 0.0f,1.0f,.0f });
	mSuppose_vert->mVertices.back()->gravityScale = 0.0f;
	mSuppose_vert->mVertices.front()->gravityScale = 0.0f;

	mParticles.reserve(4);
	mParticles.push_back(mSuppose_hori->mVertices[0]);
	mParticles.push_back(mSuppose_hori->mVertices[1]);
	mParticles.push_back(mSuppose_vert->mVertices[0]);
	mParticles.push_back(mSuppose_vert->mVertices[1]);

	if (parentOriParticle == nullptr)
	{
		mParentOri = world.AddParticle(new Particle());
		mParentOri->SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
		mOri = world.AddParticle(new Particle());
		mOri->SetPosition(Vector3{ 0.0f, 0.1f, 0.0f });
	}
	else
	{
		mParentOri = parentOriParticle;
		mOri = world.AddParticle(new Particle());
		mOri->SetPosition(mParentOri->position * NFGE::Math::Inverse(boneMatrixes[bone->index]));
	}
	mParentOri->gravityScale = 0.0f;
	mOri->gravityScale = 0.0f;
	world.AddConstraint(new Spring(mParentOri, mOri));
	//mCurrentToSuppose = ;
	//world.AddConstraint(new Spring(mSuppose_hori->mVertices.back(), mCurrent->mVertices.back()));
	//mFixedSuppose = new Fixed(mSuppose_hori->mVertices.back());
	//world.AddConstraint(mFixedSuppose);
	//world.AddConstraint(new Arriver(mSuppose_hori->mVertices.back(), mCurrent->mVertices.back()));
	world.AddRigBone(this);
}

void NFGE::Physics::RigBone::Binding(const std::vector<NFGE::Math::Matrix4>& boneMatrixes, const std::vector<NFGE::Math::Matrix4>& tPosToParentMatrixes, const NFGE::Math::Matrix4& toWorld)
{
	currentBoneMatrix = boneMatrixes[mBone->index] * toWorld;
	auto allInOneMat = tPosToParentMatrixes[mBone->index] * boneMatrixes[mBone->parentIndex] * toWorld;
	toWorldMat = allInOneMat;
	//auto allInOneMat = boneMatrixes[mBone->index] * toWorld;
	mIdle_hori->mVertices.front()->SetPosition (Vector3::Zero() * allInOneMat);
	mIdle_hori->mVertices.back()->SetPosition(Vector3{ 0.0f,0.0f,-1.0f } *allInOneMat);	// direction match with model initial direction
	mIdle_vert->mVertices.front()->SetPosition(Vector3::Zero() * allInOneMat);
	mIdle_vert->mVertices.back()->SetPosition(Vector3{ 0.0f,1.0f,0.0f } *allInOneMat);
}

void NFGE::Physics::RigBone::LookTo(const NFGE::Math::Vector3& target)
{
	mParentOri->SetPosition(mIdle_hori->mVertices.front()->position);
	if (isnan( mOri->position.x ))
	{
		mSuppose_vert->mVertices.front()->SetPosition(mParentOri->position);
		mSuppose_hori->mVertices.front()->SetPosition(mParentOri->position);
	}
	else
	{
		mSuppose_vert->mVertices.front()->SetPosition(mOri->position);
		mSuppose_hori->mVertices.front()->SetPosition(mOri->position);
	}

	if (mIsDominateByPhysics)
		return;
	Vector3 dir = Normalize(target - mSuppose_hori->mVertices.front()->position);
	mSuppose_hori->mVertices.back()->SetPosition(mSuppose_hori->mVertices.front()->position + dir);
	mSuppose_vert->mVertices.back()->SetPosition(mSuppose_vert->mVertices.front()->position + Vector3{0.0f,1.0f,0.0f});
}

void NFGE::Physics::RigBone::RotateWith(const NFGE::Math::Matrix4& animationToParent)
{
	//TODO
	mParentOri->SetPosition(Vector3::Zero() * currentBoneMatrix);
	if (isnan(mOri->position.x ))
	{
		mSuppose_vert->mVertices.front()->SetPosition(mParentOri->position);
		mSuppose_hori->mVertices.front()->SetPosition(mParentOri->position);
	}
	else
	{
		mSuppose_vert->mVertices.front()->SetPosition(mOri->position);
		mSuppose_hori->mVertices.front()->SetPosition(mOri->position);
	}
	
	if (mIsDominateByPhysics)
		return;
	mSuppose_hori->mVertices.back()->SetPosition(Vector3{0.0f,0.0f,-1.0f} * currentBoneMatrix);// direction match with model initial direction
	mSuppose_vert->mVertices.back()->SetPosition(Vector3{ 0.0f,1.0f,0.0f } * currentBoneMatrix);
}
 
NFGE::Math::Matrix4 NFGE::Physics::RigBone::GetRotationTransformQuaternion() const
{
	//[Hard code Fix] unknow problem by flip y

	if (mSuppose_hori->mVertices.back()->position.z > mSuppose_hori->mVertices.front()->position.z)
	{
		float delta = mSuppose_hori->mVertices.back()->position.y - mSuppose_hori->mVertices.front()->position.y;
		mSuppose_hori->mVertices.back()->position.y = mSuppose_hori->mVertices.front()->position.y;
		mSuppose_hori->mVertices.back()->position.y -= delta;
	}
	//[Hard code Fix] finish

	auto idleDir = /*NFGE::Math::Normalize*/(mIdle_hori->mVertices.back()->position - mIdle_hori->mVertices.front()->position);
	auto currentDir = /*NFGE::Math::Normalize*/(mSuppose_hori->mVertices.back()->position - mSuppose_hori->mVertices.front()->position);

	auto toWorldMatrix = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(idleDir, Vector3::YAxis));
	auto toLocalMatrix = NFGE::Math::Inverse(toWorldMatrix);

	idleDir = idleDir * toLocalMatrix;
	currentDir = currentDir * toLocalMatrix;


	//Constrix X rotation--- 
	auto xCross = Cross({ 0.0f, idleDir.y, idleDir.z }, { 0.0f, currentDir.y,currentDir.z });
	bool xRotDir = xCross.x > 0.0f;
	float xRotRad = acosf(Dot(Normalize({ 0.0f,idleDir.y, idleDir.z }), Normalize({ 0.0f,currentDir.y, currentDir.z })));
	NFGE::Math::Quaternion bringBackRotation_x;
	if (!xRotDir)
	{
		if (xRotRad > mXMaxRad)
			bringBackRotation_x = QuaternionRotationAxis(xCross, mXMaxRad - xRotRad);
	}
	else
	{
		if (xRotRad > mXMinRad)
			bringBackRotation_x = QuaternionRotationAxis(xCross, mXMinRad - xRotRad);
	}
	//Constrix X rotation--- 

	//Constrix Y rotation--- 
	auto yCross = Cross({ idleDir.x, 0.0f, idleDir.z }, { currentDir.x, 0.0f, currentDir.z });
	bool yRotDir = yCross.y > 0.0f;
	float yRotRad = acosf(Dot(Normalize({ idleDir.x, 0.0f, idleDir.z }), Normalize({ currentDir.x, 0.0f, currentDir.z })));
	NFGE::Math::Quaternion bringBackRotation_y;
	if (!yRotDir)
	{
		if (yRotRad > mYMaxRad)
			bringBackRotation_y = QuaternionRotationAxis(yCross, mYMaxRad - yRotRad);
	}
	else
	{
		if (yRotRad > mYMinRad)
			bringBackRotation_y = QuaternionRotationAxis(yCross, mYMinRad - yRotRad);
	}
	//Constrix Y rotation--- 

	//Constrix Z rotation--- 
	auto zCross = Cross({ idleDir.x, idleDir.y , 0.0f }, { currentDir.x, currentDir.y , 0.0f });
	bool zRotDir = zCross.z > 0.0f;
	float zRotRad = acosf(Dot(Normalize({ idleDir.x, idleDir.y , 0.0f }), Normalize({ currentDir.x, currentDir.y , 0.0f })));
	NFGE::Math::Quaternion bringBackRotation_z;
	if (!zRotDir)
	{
		if (zRotRad > mZMaxRad)
			bringBackRotation_z = QuaternionRotationAxis(zCross, mZMaxRad - zRotRad);
	}
	else
	{
		if (zRotRad > mZMinRad)
			bringBackRotation_z = QuaternionRotationAxis(zCross, mZMinRad - zRotRad);
	}
	//Constrix Z rotation--- 

	currentDir = currentDir * MatrixRotationQuaternion(bringBackRotation_x * bringBackRotation_y /** bringBackRotation_z*/);
	currentDir = currentDir * toWorldMatrix;
	mSuppose_hori->mVertices.back()->SetPosition(mSuppose_hori->mVertices.front()->position + currentDir);

	idleDir = /*NFGE::Math::Normalize*/( mIdle_hori->mVertices.back()->position - mIdle_hori->mVertices.front()->position);
	currentDir = /*NFGE::Math::Normalize*/( mSuppose_hori->mVertices.back()->position - mSuppose_hori->mVertices.front()->position);
	return MatrixRotationQuaternion(QuaternionFromTo(idleDir, currentDir));
	//return MatrixRotationQuaternion(QuaternionLookRotation(currentDir_hori, Vector3::YAxis));
}

void NFGE::Physics::RigBone::CaculateRotationTransformAixes()
{
	idleDir_hori = /*NFGE::Math::Normalize*/(mIdle_hori->mVertices.back()->position - mIdle_hori->mVertices.front()->position);
	auto currentDir_hori = /*NFGE::Math::Normalize*/(mSuppose_hori->mVertices.back()->position - mSuppose_hori->mVertices.front()->position);
	idleDir_vert = /*NFGE::Math::Normalize*/(mIdle_vert->mVertices.back()->position - mIdle_vert->mVertices.front()->position);
	auto currentDir_vert = /*NFGE::Math::Normalize*/(mSuppose_vert->mVertices.back()->position - mSuppose_vert->mVertices.front()->position);

	auto toWorldRotMat = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(idleDir_hori, Vector3::YAxis));
	auto toLocalMatrix = NFGE::Math::Inverse(toWorldRotMat);

	idleDir_hori = idleDir_hori * toLocalMatrix;
	currentDir_hori = currentDir_hori * toLocalMatrix;
	idleDir_vert = idleDir_vert * toLocalMatrix;

	//Constrix X rotation--- 
	auto xCross = Cross({ 0.0f, idleDir_hori.y, idleDir_hori.z }, { 0.0f, currentDir_hori.y,currentDir_hori.z });
	bool xRotDir = xCross.x > 0.0f;
	float xRotRad = acosf(Dot(Normalize({ 0.0f,idleDir_hori.y, -1.0f }), Normalize({ 0.0f,currentDir_hori.y, -1.0f })));
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
	auto yCross = Cross({ idleDir_hori.x, 0.0f, idleDir_hori.z }, { currentDir_hori.x, 0.0f, currentDir_hori.z });
	bool yRotDir = yCross.y > 0.0f;
	float yRotRad = acosf(Dot(Normalize({ idleDir_hori.x, 0.0f, idleDir_hori.z }), Normalize({ currentDir_hori.x , 0.0f, currentDir_hori.z })));
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

	//auto ret = xRotation * yRotation;
	//mSuppose_hori->mVertices.back()->SetPosition(mSuppose_hori->mVertices.front()->position + idleDir_hori * xRotation * Matrix4::sRotationZ(Constants::Pi) * yRotation * toWorldMatrix);
	//mSuppose_vert->mVertices.back()->SetPosition(mSuppose_vert->mVertices.front()->position + idleDir_vert * xRotation * Matrix4::sRotationY(Constants::Pi) * yRotation * toWorldMatrix); // todo add zrotation

	//return ret;
}

NFGE::Math::Matrix4 NFGE::Physics::RigBone::GetRotationTransform() const
{
	return mXRotMat * mYRotMat * mZRotMat;
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

	mSuppose_hori->mVertices.back()->SetPosition(mSuppose_hori->mVertices.front()->position + (Vector3{0.0f,0.0f,-1.0f} * mXRotMat * mYRotMat * toWorldMat - Vector3::Zero() * toWorldMat));
	//mSuppose_hori->mVertices.back()->SetPosition(mSuppose_hori->mVertices.front()->position + idleDir_hori * mXRotMat * Matrix4::sRotationZ(Constants::Pi) * mYRotMat * toWorldMat); [OPTIMAS]
	mSuppose_vert->mVertices.back()->SetPosition(mSuppose_vert->mVertices.front()->position + (Vector3{ 0.0f,1.0f,0.0f } *mXRotMat * mYRotMat * toWorldMat - Vector3::Zero() * toWorldMat));
	//mSuppose_hori->mVertices.back()->SetPosition(mSuppose_hori->mVertices.front()->position + idleDir_hori * mXRotMat * Matrix4::sRotationZ(Constants::Pi) * mYRotMat * toWorldMat); [OPTIMAS]
	// todo add zrotation
}

void NFGE::Physics::RigBone::DebugDraw() const
{
	NFGE::Graphics::SimpleDraw::AddSphere(mIdle_hori->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Red, 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mIdle_vert->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Red, 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mSuppose_hori->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Yellow , 4, 2);
	NFGE::Graphics::SimpleDraw::AddSphere(mSuppose_vert->mVertices.back()->position, 0.2f, NFGE::Graphics::Colors::Pink, 4, 2);
}

NFGE::Math::Vector3 NFGE::Physics::RigBone::GetPosition() const
{
	return mSuppose_hori->mVertices.front()->position; 
}