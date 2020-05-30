#include "PTZRobotArm.h"

using namespace NFGE;
using namespace NFGE::Math;
using namespace NFGE::Input;

namespace
{
	const float RotationSpeed = 2.10f;
}

void PTZRobotArm::OnLoad(int maxSeg)
{
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,7.0f,0.0f }, Quaternion(), Vector3{ 1.0f, 7.0f, 1.0f }, NFGE::Graphics::Colors::Red, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,3.0f,0.0f }, Quaternion(), Vector3{ 0.5f, 3.0f, 0.5f }, NFGE::Graphics::Colors::Green, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,4.0f,0.0f }, Quaternion(), Vector3{ 0.5f, 4.0f, 0.5f }, NFGE::Graphics::Colors::Blue, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,1.0f,0.0f }, Quaternion(), Vector3{ 1.0f, 1.0f, 1.0f }, NFGE::Graphics::Colors::Yellow, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,4.0f,0.0f }, Quaternion(), Vector3{ 2.0f, 4.0f, 2.0f }, NFGE::Graphics::Colors::Orange, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,10.0f,0.0f }, Quaternion(), Vector3{ 0.3f, 10.0f, 0.3f }, NFGE::Graphics::Colors::Cyan, NFGE::Graphics::Colors::Gray);

	mMaxSeg = maxSeg;
	mRobotArmSegments.reserve(mMaxSeg);
}

void PTZRobotArm::OnUnLoad()
{
}

void PTZRobotArm::OnUpdate(float deltaTime)
{
}

void PTZRobotArm::OnDebugUI()
{
	if (mRobotArmSegments.empty())
		return;
	ImGui::BeginGroup();
	ImGui::Text("Move Robot Arm");
	Vector3& newTranslation = mRobotArmSegments[0].mAnitmatePosition;

	ImGui::Text("Move X: "); ImGui::SameLine();
	if (ImGui::ArrowButton("Move X_l: ", ImGuiDir_Left)) newTranslation.x -= 5.0f; ImGui::SameLine();
	if (ImGui::ArrowButton("Move X_r: ", ImGuiDir_Right)) newTranslation.x += 5.0f;
	ImGui::Text("Move Y: "); ImGui::SameLine();
	if (ImGui::ArrowButton("Move Y_l: ", ImGuiDir_Left)) newTranslation.y -= 5.0f; ImGui::SameLine();
	if (ImGui::ArrowButton("Move Y_r: ", ImGuiDir_Right)) newTranslation.y += 5.0f;
	ImGui::Text("Move Z: "); ImGui::SameLine();
	if (ImGui::ArrowButton("Move Z_l: ", ImGuiDir_Left)) newTranslation.z -= 5.0f; ImGui::SameLine();
	if (ImGui::ArrowButton("Move Z_r: ", ImGuiDir_Right)) newTranslation.z += 5.0f;

	mRobotArmSegments[0].mBone.toParentTransform = mRobotArmSegments[0].mBone.toParentTransform * Matrix4::sTranslation(newTranslation);
	UpdateSegmentTransform(0, 0.0f);
	ImGui::EndGroup();

	Vector3 origin = Vector3::Zero() * mRobotArmSegments[0].boneMatrix;
	NFGE::Graphics::SimpleDraw::AddCircle_FaceY(origin.x, origin.y, origin.z, 10.0f, NFGE::Graphics::Colors::LightGreen);
}

void PTZRobotArm::OnRender(const Camera & camera)
{
	for (int i = 0; i < mRobotArmSegments.size(); ++i)
	{
		auto& robotArmSeg = mRobotArmSegments[i];
		robotArmSeg.mBone.toParentTransform.Identity();
		
		if (robotArmSeg.mBone.parent)
		{
			auto& parentBoneMatrix = mRobotArmSegments[robotArmSeg.mBone.parentIndex].boneMatrix;
			robotArmSeg.mBone.toParentTransform = (parentBoneMatrix * robotArmSeg.mBone.parent->toParentTransform);
		}
		mAllTypeSegaments[robotArmSeg.mArmSegmentType].Render(robotArmSeg.boneMatrix * robotArmSeg.mBone.toParentTransform , camera, mCurrentSelectSegment == i);
	}
}

void PTZRobotArm::AddSegment(int segmentType)
{
	int newIndex = static_cast<int>(mRobotArmSegments.size());
	if (newIndex == mMaxSeg)
		return;

	if (newIndex == 0)
	{
		auto& currentSegType = mAllTypeSegaments[segmentType];
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
		mRobotArmSegments.emplace_back( segmentType, bone, Matrix4::sIdentity(),Vector3::Zero(), Quaternion::Identity() );
		mCurrentSelectSegment = 0;
	}
	else
	{
		auto& currentRobotSeg = mRobotArmSegments[mCurrentSelectSegment];
		auto& currentSegType = mAllTypeSegaments[currentRobotSeg.mArmSegmentType];
		Matrix4 offsetMat = Math::Matrix4::sTranslation(currentSegType.GetChildSpwanPosition());
		Bone bone{
		"",
		newIndex,
		&(currentRobotSeg.mBone),
		currentRobotSeg.mBone.index,
		std::vector<Bone*>(),
		std::vector<int>(),
		Math::Matrix4::sIdentity(),
		offsetMat
		};
		mRobotArmSegments.emplace_back(segmentType, bone, offsetMat, currentSegType.GetChildSpwanPosition(), Quaternion::Identity());
		currentRobotSeg.mBone.children.push_back(&mRobotArmSegments.back().mBone);
		currentRobotSeg.mBone.childIndices.push_back(newIndex);
		mCurrentSelectSegment = newIndex;
		mCurrentChildBoneIndex = 0;
		for(int i = 0; i < mRobotArmSegments[0].mAnimations.size(); ++i)
			mRobotArmSegments[mCurrentSelectSegment].mAnimations.emplace_back();
	}
}

void PTZRobotArm::ControlSegment(float deltaTime)
{
	auto& currentRobotArmSegment = mRobotArmSegments[mCurrentSelectSegment];
	auto& currentBoneMatrix = currentRobotArmSegment.boneMatrix;
	
	// Careful: ToParentMatix is oppsite to what you want rotaion in world;
	auto inputSystem = InputSystem::Get();

	Vector3 lookVec;
	if (currentRobotArmSegment.mBone.parent)
	{
		Vector3 parentHead = Vector3::Zero() * currentRobotArmSegment.mBone.parent->toParentTransform;
		Vector3 parentButt = mAllTypeSegaments[currentRobotArmSegment.mArmSegmentType].GetChildSpwanPosition() * currentRobotArmSegment.mBone.parent->offsetTransform * currentRobotArmSegment.mBone.parent->toParentTransform;
		lookVec = NFGE::Math::Normalize(parentButt - parentHead);
	}
	else
		lookVec = Vector3::YAxis;

	if (inputSystem->IsKeyDown(KeyCode::NUMPAD4))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime); //* QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD6))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::NUMPAD2))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(Vector3::XAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(Vector3::XAxis, -RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD8))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(-Vector3::XAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(-Vector3::XAxis, -RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD7))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(lookVec, RotationSpeed * deltaTime);// *QuaternionRotationAxis(-lookVec, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD9))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(lookVec, -RotationSpeed * deltaTime);// *QuaternionRotationAxis(lookVec, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
}

void PTZRobotArm::UpdateSegmentTransform(int index, float deltaTime)
{
	auto& boneMatrix = mRobotArmSegments[index].boneMatrix;
	boneMatrix = Math::MatrixRotationQuaternion(mRobotArmSegments[index].mAnitmateRotation) * Matrix4::sTranslation(mRobotArmSegments[index].mAnitmatePosition);
}
