#include "PTTentacle.h"

using namespace NFGE::Input;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

namespace
{
	float RotationSpeed = 0.20f;

	void UpdateTransfrom(Bone* bone, std::vector<PC::TentacleSegment>& armSegments)
	{
		if (bone->parent)
			armSegments[bone->index].boneMatrix = bone->toParentTransform * armSegments[bone->parentIndex].boneMatrix;
		else
			armSegments[bone->index].boneMatrix = bone->toParentTransform;
		for (auto child : bone->children)
			UpdateTransfrom(child, armSegments);
	}
}

void PCTentacle::Load(int totalSeg, float baseLength, const char* diffuseMap, const char* nomralMap, const char* specularMap, const char* displaceMentMap)
{
	//ASSERT(__assume(totalSeg < NFGE::Graphics::MaxBoneCount), "[HelloTentacle] try to initlize a tentacle that have more than MaxBoneCount of bone");

	// - Bone Initlize -----------------------------------------------------------
	mTotalSeg = totalSeg;
	mMesh = NFGE::Graphics::MeshBuilder::CreateTentacle(mTotalSeg, 500, 10, 2.0f, baseLength, NFGE::Math::Ease::EaseNone);
	mMeshBuffer.Initialize(mMesh, true);
	mMeshBuffer.SetTopology();
	
	BuildBoneSkelton(mTotalSeg, baseLength);

	mEffectContext.

	mDiffuseTexture = TextureManager::Get()->LoadTexture(diffuseMap);
	mNormalTexture = TextureManager::Get()->LoadTexture(nomralMap);
	mSpecularTexture = TextureManager::Get()->LoadTexture(specularMap);
	mDisplacementTexture = TextureManager::Get()->LoadTexture(displaceMentMap);

	mPosition = Vector3(0.0f, 0.0f, 0.0f);
	mRotation = Quaternion::Identity();
	mScale = Vector3{ 1.0f,1.0f,1.0f };
	// ---------------------------------------------------------------------------

}

void PCTentacle::Update(float deltaTime)
{
	ControlSegment(deltaTime);
}

void PCTentacle::Render(const Camera & camera)
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	NFGE::Math::Matrix4 rotMat = NFGE::Math::MatrixRotationQuaternion(mRotation);
	mMatrices[0].Identity();
	NFGE::Math::Matrix4 scl = NFGE::Math::Matrix4::sScaling(mScale);
	mMatrices[0] = (scl * rotMat * mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));
	mMatrices[1] = (camera.GetViewMatrix());
	mMatrices[2] = (camera.GetPerspectiveMatrix());

	PC::TransformData transformData;
	transformData.world = Transpose(mMatrices[0]);
	transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
	transformData.viewPositon = camera.GetPosition();
	mTransformBuffer.Load(transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mMaterialBuffer.Load(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	Matrix4 boneTransformMatrix[128];
	for (int i = 0; i < mSegs.size(); i++)
	{
		boneTransformMatrix[i] = Transpose(mSegs[i].boneMatrix);
	}
	mBoneTransferBuffer.Load(boneTransformMatrix);
	mBoneTransferBuffer.BindVS(4);

	PC::OptionsData optData;
	optData.displacementWeight = mBumpWeight;
	mOptionBuffer.Load(optData);
	mOptionBuffer.BindVS(3);

	TextureManager::Get()->GetTexture(mDiffuseTexture)->BindPS();
	TextureManager::Get()->GetTexture(mSpecularTexture)->BindPS(1);
	TextureManager::Get()->GetTexture(mDisplacementTexture)->BindVS(2);
	TextureManager::Get()->GetTexture(mNormalTexture)->BindPS(3);
	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
	SamplerManager::Get()->GetSampler("LinearWrap")->BindVS();

	auto rs = NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Wireframe");
	rs->Set();
	mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
	mMeshBuffer.Render();
	rs->Clear();
}

void PCTentacle::DebugUI()
{
	// - Imgui control -------------------------------------------------------------------------------------------------------------------------------
	ImGui::Begin("Control PCTentacle");

	ImGui::Text("Control Individual Segment");

	ImGui::BeginChild("HorizongtalScrollingView", ImVec2(0, ImGui::GetFrameHeightWithSpacing() + 30), true, ImGuiWindowFlags_HorizontalScrollbar);

	for (int n = 0; n < mSegs.size(); n++)
	{
		if (n > 0) ImGui::SameLine();
		ImGui::PushID(n);
		char num_buf[16];
		sprintf_s(num_buf, sizeof(num_buf), "%d", n);
		const char* label = num_buf;
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
		if (ImGui::Button(label, ImVec2(40.0f, 0.0f)))
		{
			mCurrentControlSeg = n;
		}
		ImGui::PopStyleColor(1);
		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::End();
	// -----------------------------------------------------------------------------------------------------------------------------------------------

	const int colorArraySize = 3;
	std::array<NFGE::Graphics::Color, colorArraySize> mColor = { NFGE::Graphics::Colors::Red, NFGE::Graphics::Colors::Green, NFGE::Graphics::Colors::Blue };

	for (int i = 0; i < mSegs.size(); i++)
	{
		Vector3 bone_pos_0 = Vector3{ 0.0f,0.0f,0.0f } *mSegs[i].boneMatrix;
		Vector3 bone_pos_1 = Vector3{ 0.0f,0.0f,0.0f } *Matrix4::sTranslation(mSegs[i].mChildSpwanPoint)* mSegs[i].boneMatrix;
		NFGE::sApp.DrawLine(bone_pos_0, bone_pos_1, mColor[i % colorArraySize]);
	}


	ImGui::Begin("Debug Mesh Vertex");
	{
		ImGui::InputInt("Vertex Index", &mCurrentControlVertex);
		mCurrentControlVertex %= mMesh.vertices.size();

		auto& currentVertex = mMesh.vertices[mCurrentControlVertex];

		char buffer[128];
		sprintf_s(buffer, sizeof(buffer), "bone 0 Indice: %d", currentVertex.boneIndices[0]);
		ImGui::Text(buffer);
		sprintf_s(buffer, sizeof(buffer), "bone 1 Indice: %d", currentVertex.boneIndices[1]);
		ImGui::Text(buffer);
		ImGui::InputFloat("bone 0 weight", &currentVertex.boneWeights[0], 0.01f, 0.01f, 4);
		ImGui::InputFloat("bone 1 weight", &currentVertex.boneWeights[1], 0.01f, 0.01f, 4);

		auto currentPos = currentVertex.position * ((mSegs[currentVertex.boneIndices[1]].boneMatrix * currentVertex.boneWeights[1]) + (mSegs[currentVertex.boneIndices[0]].boneMatrix * currentVertex.boneWeights[0]));
		NFGE::sApp.DrawSphere(currentPos, 0.05f, Colors::Yellow);

	}
	ImGui::End();

}

void PCTentacle::UnLoad()
{
	// - Mesh Initlize -----------------------------------------------------------
	mMeshBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mBoneTransferBuffer.Terminate();
	mOptionBuffer.Terminate();
	// ---------------------------------------------------------------------------
}

void PCTentacle::BuildBoneSkelton(int totalBone, float baseLength)
{
	mSegs.reserve(totalBone);

	auto reverseGoldenRatio = 1.0f / NFGE::Math::Constants::GoldenRatio;

	for (int i = 0; i < totalBone; i++)
	{
		Bone bone{
			"",
			i,
			nullptr,
			-1,
			std::vector<Bone*>(),
			std::vector<int>(),
			NFGE::Math::Matrix4::sIdentity(),
			NFGE::Math::Matrix4::sIdentity()
		};

		if (i != 0)
		{
			auto& lastBone = mSegs.back().mBone;
			//mSegs.emplace_back(bone, NFGE::Math::Matrix4::sIdentity(), Vector3{ 0.0f, mSegs[i - 1].mAnitmatePosition.y * reverseGoldenRatio ,0.0f }, Vector3{ 0.0f,0.0f,0.0f }, Quaternion::Identity());
			mSegs.emplace_back(bone, NFGE::Math::Matrix4::sIdentity(), Vector3{ 0.0f, mSegs[i - 1].mChildSpwanPoint.y,0.0f }, Vector3{ 0.0f,0.0f,0.0f }, Quaternion::Identity());
			auto& currentBone = mSegs.back().mBone;
			currentBone.parent = &lastBone;
			currentBone.parentIndex = lastBone.index;
			lastBone.children.push_back(&currentBone);
			lastBone.childIndices.push_back(currentBone.index);

			mSegs[i].mAnitmatePosition = mSegs[i - 1].mChildSpwanPoint;
		}
		else
		{
			mSegs.emplace_back(bone, NFGE::Math::Matrix4::sIdentity(), Vector3{ 0.0f,baseLength,0.0f }, Vector3{ 0.0f,0.0f,0.0f }, Quaternion::Identity());
		}
		UpdateSegmentTransform(i);
	}
	mCurrentControlSeg = 0;
}

void PCTentacle::ControlSegment(float deltaTime)
{

	// Button Control --------------------------------------------------------------------------------------------------------------------------------
	auto& currentSegment = mSegs[mCurrentControlSeg];
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyDown(KeyCode::NUMPAD4))
	{
		currentSegment.mAnitmateRotation = currentSegment.mAnitmateRotation * QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentControlSeg);
	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD6))
	{
		currentSegment.mAnitmateRotation = currentSegment.mAnitmateRotation * QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime);
		UpdateSegmentTransform(mCurrentControlSeg);
	}

	if (inputSystem->IsKeyDown(KeyCode::NUMPAD7))
	{
		for (int i = 0; i < mSegs.size(); i++)
		{
			mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(Vector3::ZAxis, RotationSpeed * deltaTime);
			mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(Vector3::XAxis, RotationSpeed * deltaTime);
			mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(Vector3::YAxis, RotationSpeed * deltaTime);
			UpdateSegmentTransform(i);
		}

	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD9))
	{
		for (int i = 0; i < mSegs.size(); i++)
		{
			mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(-Vector3::ZAxis, RotationSpeed * deltaTime);
			mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(-Vector3::XAxis, RotationSpeed * deltaTime);
			mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(-Vector3::YAxis, RotationSpeed * deltaTime);
			UpdateSegmentTransform(i);
		}
	}

	if (inputSystem->IsKeyDown(KeyCode::NUMPAD1))
	{
		for (int i = 1; i < mSegs.size(); i++)
		{
			float direction = i % 2 == 0 ? 1.0f : -1.0f;
			mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(Vector3::ZAxis * direction, RotationSpeed * deltaTime);
			//mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(Vector3::XAxis * direction, RotationSpeed * deltaTime);
			//mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(Vector3::YAxis * direction, RotationSpeed * deltaTime);
			UpdateSegmentTransform(i);
		}

	}
	if (inputSystem->IsKeyDown(KeyCode::NUMPAD3))
	{
		for (int i = 1; i < mSegs.size(); i++)
		{
			float direction = i % 2 == 0 ? 1.0f : -1.0f;
			mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(-Vector3::ZAxis* direction, RotationSpeed * deltaTime);
			//mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(-Vector3::XAxis* direction, RotationSpeed * deltaTime);
			//mSegs[i].mAnitmateRotation = mSegs[i].mAnitmateRotation * QuaternionRotationAxis(-Vector3::YAxis* direction, RotationSpeed * deltaTime);
			UpdateSegmentTransform(i);
		}
	}
	// -----------------------------------------------------------------------------------------------------------------------------------------------
}

void PCTentacle::UpdateSegmentTransform(int index)
{
	auto& currentToParent = mSegs[index].mBone.toParentTransform;

	currentToParent = NFGE::Math::MatrixRotationQuaternion(mSegs[index].mAnitmateRotation) * Matrix4::sTranslation(mSegs[index].mAnitmatePosition);
	UpdateTransfrom(&mSegs[index].mBone, mSegs);
}
