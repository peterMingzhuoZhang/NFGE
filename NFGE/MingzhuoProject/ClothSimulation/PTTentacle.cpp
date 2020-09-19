#include "PTTentacle.h"

using namespace NFGE::Input;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

namespace
{
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Standard_Skining.fx";
	float RotationSpeed = 0.20f;
	Vector3 hookPosition = { 0.0f,30.0f,0.0f };
	float SupportRootDistance = 10.0f;
	float grassGrowingLevelDiff = 0.01f;

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

bool PTTentacle::IsMeshInit = false;
NFGE::Graphics::BoneMesh PTTentacle::mMesh;
NFGE::Graphics::MeshBuffer PTTentacle::mMeshBuffer;

void PTTentacle::Load(int totalSeg, float baseLength, const char* diffuseMap, const char* nomralMap, const char* specularMap, const char* displaceMentMap)
{
	//ASSERT(__assume(totalSeg < NFGE::Graphics::MaxBoneCount), "[HelloTentacle] try to initlize a tentacle that have more than MaxBoneCount of bone");

	// - Bone Initlize -----------------------------------------------------------
	mTotalSeg = totalSeg;
	BuildBoneSkelton(mTotalSeg, baseLength);
	// - Mesh Initlize -----------------------------------------------------------
	if (!IsMeshInit)
	{
		mMesh = NFGE::Graphics::MeshBuilder::CreateTentacle(mTotalSeg, 500, 40, 2.0f, baseLength, NFGE::Math::Ease::EaseNone);
		mMeshBuffer.Initialize(mMesh, true);
		mMeshBuffer.SetTopology();
		IsMeshInit = true;
	}

	mMatrices[0].Identity();
	mMatrices[1].Identity();
	mMatrices[2].Identity();

	mVertexShader.Initialize(MeshShaderFileName, NFGE::Graphics::BoneVertex::Format);
	mPixelShader.Initialize(MeshShaderFileName);

	mTransformBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mBoneTransferBuffer.Initialize();
	mOptionBuffer.Initialize();

	mMaterial.ambient = NFGE::Graphics::Colors::LightGray;
	mMaterial.diffuse = NFGE::Graphics::Colors::LightSlateGray;
	mMaterial.specular = { 0.8f, 0.8f, 0.8f, 1.0f };
	mMaterial.power = 10.0f;

	mDiffuseTexture = TextureManager::Get()->LoadTexture(diffuseMap);
	mNormalTexture = TextureManager::Get()->LoadTexture(nomralMap);
	mSpecularTexture = TextureManager::Get()->LoadTexture(specularMap);
	mDisplacementTexture = TextureManager::Get()->LoadTexture(displaceMentMap);

	mPosition = Vector3(0.0f, 0.0f, 0.0f);
	mRotation = Quaternion::Identity();
	mScale = Vector3{ 1.0f,1.0f,1.0f };
	// ---------------------------------------------------------------------------

}

void PTTentacle::InitPhysics(NFGE::Physics::PhysicsWorld& world)
{
	//mRootParticle = world.AddParticle(new NFGE::Physics::Particle());

	NFGE::Math::Vector3 lastSpwanPos = mPosition;
	
	auto newParticle_root = world.AddParticle(mGrassParticles.emplace_back(new NFGE::Physics::Particle()));
	newParticle_root->SetPosition(mPosition);
	newParticle_root->invMass = 1.0f;
	world.AddConstraint(new NFGE::Physics::Fixed(newParticle_root));

	NFGE::Physics::Particle* lastParticle;

	int index = 0;
	for (auto& seg : mSegs)
	{
		lastParticle = mGrassParticles.back();

		auto newParticle_grass = world.AddParticle(mGrassParticles.emplace_back(new NFGE::Physics::Particle()));
		newParticle_grass->SetPosition(seg.mChildSpwanPoint + lastSpwanPos + Vector3(0.0f, 0.0f, index * grassGrowingLevelDiff));
		newParticle_grass->invMass = 1.0f;
		lastSpwanPos = newParticle_grass->position;

		world.AddConstraint(new NFGE::Physics::Spring(lastParticle, newParticle_grass));
		lastParticle = newParticle_grass;

		AddSupport(world, Vector3(0.0f, 0.0f, SupportRootDistance));
		AddSupport(world, Vector3(0.0f, 0.0f, -SupportRootDistance));
		AddSupport(world, Vector3(SupportRootDistance, 0.0f, 0.0f));
		AddSupport(world, Vector3(-SupportRootDistance, 0.0f, 0.0f));

		//auto newParticle_support = world.AddParticle(new NFGE::Physics::Particle());
		//newParticle_support->SetPosition(newParticle_grass->position + Vector3(0.0f,0.0f,index * grassGrowingLevelDiff));
		//newParticle_support->invMass = 10.0f * (index + 1);
		//world.AddConstraint(new NFGE::Physics::Spring(newParticle_grass, newParticle_support));
		//world.AddConstraint(new NFGE::Physics::Fixed(newParticle_support));
		++index;
	}

	
}

void PTTentacle::Update(float deltaTime)
{
	//ControlSegment(deltaTime);
	UpdateSegmentTransform_FromPhysics();

}

void PTTentacle::Render(const Camera & camera)
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

	auto rs = NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid");
	rs->Set();
	mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
	mMeshBuffer.Render();
	rs->Clear();
}

void PTTentacle::DebugUI()
{
	// - Imgui control -------------------------------------------------------------------------------------------------------------------------------
	ImGui::Begin("Control PTTentacle");

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

void PTTentacle::UnLoad()
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

void PTTentacle::BuildBoneSkelton(int totalBone, float baseLength)
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



void PTTentacle::UpdateSegmentTransform(int index)
{
	auto& currentToParent = mSegs[index].mBone.toParentTransform;

	currentToParent = NFGE::Math::MatrixRotationQuaternion(mSegs[index].mAnitmateRotation) * Matrix4::sTranslation(mSegs[index].mAnitmatePosition);
	UpdateTransfrom(&mSegs[index].mBone, mSegs);
}

void PTTentacle::UpdateSegmentTransform_FromPhysics()
{

	// mGrassParticle has two element more than mSeg, because of root and hook
	NFGE::Math::Vector3 lastDirection = NFGE::Math::Vector3::YAxis; // assume all grass initially faceing up
	for (size_t i = 0; i < mSegs.size(); i++)
	{
		NFGE::Math::Vector3 direction = NFGE::Math::Normalize(mGrassParticles[i + 1]->position - mGrassParticles[i]->position);
		
		if (lastDirection == direction)
		{
			mSegs[i].mAnitmateRotation = NFGE::Math::Quaternion::Identity();
		}
		else
		{
			mSegs[i].mAnitmateRotation = NFGE::Math::QuaternionFromTo(lastDirection, direction);
		}

		lastDirection = direction;
		if (i == 0)
			mSegs[i].mAnitmatePosition = NFGE::Math::Vector3::Zero();
		else
			mSegs[i].mAnitmatePosition = mGrassParticles[i + 1]->position - mGrassParticles[i]->position;

		auto& currentToParent = mSegs[i].mBone.toParentTransform;
		currentToParent = NFGE::Math::MatrixRotationQuaternion(mSegs[i].mAnitmateRotation) * Matrix4::sTranslation(mSegs[i].mAnitmatePosition);
	}

	
	UpdateTransfrom(&mSegs[0].mBone, mSegs);
}

void PTTentacle::AddSupport(NFGE::Physics::PhysicsWorld& world, Vector3 displacement)
{
	auto newParticle_supportRoot00 = world.AddParticle(new NFGE::Physics::Particle());
	newParticle_supportRoot00->SetPosition(mPosition + displacement);
	newParticle_supportRoot00->invMass = 10.0f;
	world.AddConstraint(new NFGE::Physics::Spring(mGrassParticles.back(), newParticle_supportRoot00));
	world.AddConstraint(new NFGE::Physics::Fixed(newParticle_supportRoot00));
}

