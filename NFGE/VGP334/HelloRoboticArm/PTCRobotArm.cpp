#include "PTCRobotArm.h"

using namespace NFGE;
using namespace NFGE::Math;
using namespace NFGE::Input;

namespace
{
	const float RotationSpeed = 2.0f;

	void UpdateTransfrom(Graphics::Bone* bone, std::vector<RobotArmSegment>& armSegments)
	{
		if (bone->parent)
			armSegments[bone->index].boneMatrix = bone->toParentTransform * armSegments[bone->parentIndex].boneMatrix;
		else
			armSegments[bone->index].boneMatrix = bone->toParentTransform;
		for (auto child : bone->children)
			UpdateTransfrom(child, armSegments);
	}
}

void PTCRobotArm::OnLoad(int maxSeg)
{
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,7.0f,0.0f }, Quaternion(), Vector3{ 1.0f, 7.0f, 1.0f }, NFGE::Graphics::Colors::Red, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,3.0f,0.0f }, Quaternion(), Vector3{ 0.5f, 3.0f, 0.5f }, NFGE::Graphics::Colors::Green, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,5.0f,0.0f }, Quaternion(), Vector3{ 0.5f, 5.0f, 0.5f }, NFGE::Graphics::Colors::Blue, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,0.0f,0.0f }, Quaternion(), Vector3{ 1.0f, 1.0f, 1.0f }, NFGE::Graphics::Colors::Yellow, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,1.0f,0.0f }, Quaternion(), Vector3{ 1.0f, 1.0f, 1.0f }, NFGE::Graphics::Colors::OrangeRed, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,2.0f,0.0f }, Quaternion(), Vector3{ 1.0f, 2.0f, 1.0f }, NFGE::Graphics::Colors::CadetBlue, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,0.5f,0.0f }, Quaternion(), Vector3{ 0.5f, 0.5f, 0.5f }, NFGE::Graphics::Colors::IndianRed, NFGE::Graphics::Colors::Gray);
	mAllTypeSegaments.emplace_back(mGeometry, Vector3{ 0.0f,0.5f,0.0f }, Quaternion(), Vector3{ 3.0f, 2.0f, 0.3f }, NFGE::Graphics::Colors::Green, NFGE::Graphics::Colors::Gray);

	mMaxSeg = maxSeg;
	mRobotArmSegments.reserve(mMaxSeg);
}

void PTCRobotArm::OnUnLoad()
{
}

void PTCRobotArm::OnUpdate(float deltaTime)
{
	
}

void PTCRobotArm::OnDebugUI()
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

	//mRobotArmSegments[0].mBone.toParentTransform = mRobotArmSegments[0].mBone.toParentTransform * Matrix4::sTranslation(newTranslation);
	UpdateSegmentTransform(0, 0.0f);

	ImGui::EndGroup();

	Vector3 origin = Vector3::Zero() * mRobotArmSegments[0].mBone.toParentTransform;
	NFGE::Graphics::SimpleDraw::AddCircle_FaceY(origin.x, origin.y, origin.z, 10.0f, NFGE::Graphics::Colors::LightGreen);
}

void PTCRobotArm::OnRender(const Camera & camera)
{
	for (int i = 0; i < mRobotArmSegments.size(); ++i)
	{
		auto& robotArmSeg = mRobotArmSegments[i];
		mAllTypeSegaments[robotArmSeg.mArmSegmentType].Render(robotArmSeg.boneMatrix, camera, mCurrentSelectSegment == i);
	}
}

void PTCRobotArm::AddSegment(int segmentType)
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
		mRobotArmSegments.emplace_back( segmentType, bone, Matrix4::sIdentity(), Vector3::Zero(), Quaternion::Identity());
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
			offsetMat,
			Math::Matrix4::sIdentity()
		};
		mRobotArmSegments.emplace_back( segmentType, bone, Math::Matrix4::sIdentity(), currentSegType.GetChildSpwanPosition(), Quaternion::Identity());
		currentRobotSeg.mBone.children.push_back(&mRobotArmSegments.back().mBone);
		currentRobotSeg.mBone.childIndices.push_back(newIndex);
		mCurrentSelectSegment = newIndex;
		mCurrentChildBoneIndex = 0;
		UpdateTransfrom(&mRobotArmSegments[mCurrentSelectSegment].mBone, mRobotArmSegments);
		for (int i = 0; i < mRobotArmSegments[0].mAnimations.size(); ++i)
			mRobotArmSegments[mCurrentSelectSegment].mAnimations.emplace_back();
	}
}

void PTCRobotArm::ControlSegment(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	auto& currentRobotArmSegment = mRobotArmSegments[mCurrentSelectSegment];

	if (inputSystem->IsKeyDown(KeyCode::NUMPAD4))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD6))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime);;
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD8))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(Vector3::XAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD2))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(-Vector3::XAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime);;
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD9))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(Vector3::YAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD7))
	{
		currentRobotArmSegment.mAnitmateRotation = currentRobotArmSegment.mAnitmateRotation * QuaternionRotationAxis(-Vector3::YAxis, RotationSpeed * deltaTime);// *QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime);;
		UpdateSegmentTransform(mCurrentSelectSegment, deltaTime);
	}
}

void PTCRobotArm::UpdateSegmentTransform(int index, float deltaTime)
{
	auto& currentToParent = mRobotArmSegments[index].mBone.toParentTransform;
	currentToParent = Math::MatrixRotationQuaternion(mRobotArmSegments[index].mAnitmateRotation) * Matrix4::sTranslation(mRobotArmSegments[index].mAnitmatePosition);
	UpdateTransfrom(&mRobotArmSegments[index].mBone, mRobotArmSegments);
}
