#include "Zory_RobotArm.h"

using namespace NFGE;
using namespace NFGE::Input;

namespace
{
	const float RotationSpeed = 2.0f;

	void UpdateTransfrom(Graphics::Bone* bone, std::vector<Matrix4>& boneMatrix)
	{
		if (bone->parent)
			boneMatrix[bone->index] = bone->toParentTransform * boneMatrix[bone->parentIndex];
		else
			boneMatrix[bone->index] = bone->toParentTransform;
		for (auto child : bone->children)
			UpdateTransfrom(child, boneMatrix);
	}
}

void Zory_RobotArm::Load()
{
	mRobotArmMesh.Load();
	mRobotArmSegments.reserve(mMaxSeg);
	boneMatrixes.reserve(mMaxSeg);
	AddSegment();
	AddSegment();
	AddSegment();

	//TODO Add segment
}

void Zory_RobotArm::UnLoad()
{
	mRobotArmMesh.UnLoad();
}

void Zory_RobotArm::Update(float deltaTime)
{
	ControlSegment(deltaTime);
}

void Zory_RobotArm::Render(const Camera & camera)
{
	for (int i = 0; i < mRobotArmSegments.size(); ++i)
	{
		auto& robotArmSeg = mRobotArmSegments[i];
		//mRobotArmMesh.Render(boneMatrixes[robotArmSeg.index], camera);		// Step 2
		mRobotArmMesh.Render(robotArmSeg.toParentTransform, camera);			// Step 1
	}
}

void Zory_RobotArm::AddSegment()
{
	int newIndex = static_cast<int>(mRobotArmSegments.size());
	if (newIndex == mMaxSeg)
		return;

	if (newIndex == 0)
	{
		Bone bone{
			"",
			newIndex,
			nullptr,
			-1,
			std::vector<Bone*>(),
			std::vector<int>(),
			Math::Matrix4::sIdentity(),
			Math::Matrix4::sIdentity()
		};
		mRobotArmSegments.push_back(bone);
		boneMatrixes.push_back(Matrix4::sIdentity());
	}
	else
	{
		auto& currentRobotSeg = mRobotArmSegments.back();
		//Matrix4 toParent = Math::Matrix4::sTranslation(mRobotArmMesh.mChildSpwanPosition);	// Step 2
		Bone bone{
			"",
			newIndex,
			&(mRobotArmSegments.back()),
			mRobotArmSegments.back().index,
			std::vector<Bone*>(),
			std::vector<int>(),
			Math::Matrix4::sIdentity(),					// Step 1
			//toParent,										// Step 2
			Math::Matrix4::sIdentity()
		};
		mRobotArmSegments.push_back(bone);
		currentRobotSeg.children.push_back(&mRobotArmSegments.back());
		currentRobotSeg.childIndices.push_back(mRobotArmSegments.back().index);
		boneMatrixes.push_back(Matrix4::sIdentity());
		//UpdateTransfrom(&mRobotArmSegments.back(), boneMatrixes);			// Step 2
	}
}

void Zory_RobotArm::ControlSegment(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD1))
	{
		mRobotArmSegments[0].toParentTransform = MatrixRotationQuaternion(QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime)) * mRobotArmSegments[0].toParentTransform;
		//UpdateTransfrom(&mRobotArmSegments[0], boneMatrixes);				// Step 2
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD3))
	{
		mRobotArmSegments[0].toParentTransform = MatrixRotationQuaternion(QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime)) * mRobotArmSegments[0].toParentTransform;
		//UpdateTransfrom(&mRobotArmSegments[0], boneMatrixes);				// Step 2
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD4))
	{
		mRobotArmSegments[1].toParentTransform = MatrixRotationQuaternion(QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime)) * mRobotArmSegments[1].toParentTransform;
		//UpdateTransfrom(&mRobotArmSegments[1], boneMatrixes);				// Step 2
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD6))
	{
		mRobotArmSegments[1].toParentTransform = MatrixRotationQuaternion(QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime)) * mRobotArmSegments[1].toParentTransform;
		//UpdateTransfrom(&mRobotArmSegments[1], boneMatrixes);				// Step 2
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD7))
	{
		mRobotArmSegments[2].toParentTransform = MatrixRotationQuaternion(QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime)) * mRobotArmSegments[2].toParentTransform;
		//UpdateTransfrom(&mRobotArmSegments[2], boneMatrixes);				// Step 2
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD9))
	{
		mRobotArmSegments[2].toParentTransform = MatrixRotationQuaternion(QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime)) * mRobotArmSegments[2].toParentTransform;
		//UpdateTransfrom(&mRobotArmSegments[2], boneMatrixes);				// Step 2
	}
}
