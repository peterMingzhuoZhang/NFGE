#include "RobotArm.h"

using namespace NFGE;
using namespace NFGE::Math;
using namespace NFGE::Input;

#define LOAD_SAFTY_CHECK(readResult) if (readResult == EOF)\
			{	LOG("[NFGE::HelloRobotArm::RobotArm::LoadFromFile()] --- Error: File currupted! Reach EOF while Loading.");\
				return false;}

namespace
{
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Standard_noTexture.fx";

	bool IsEndOf(const char* end, const char* string)
	{
		int endLength = static_cast<int>(strlen(end));
		int stringLength = static_cast<int>(strlen(string));
		for (int i = 0; i < endLength; i++)
		{
			if (end[endLength - 1 - i] != string[stringLength - 1 - i]) return false;
		}
		return true;
	}
}

void RobotArm::Load(int maxSeg)
{
	mGeometry.Load(MeshShaderFileName, MeshShaderFileName);
	OnLoad(maxSeg);
}

void RobotArm::UnLoad()
{
	mGeometry.UnLoad();
	for (auto& seg : mRobotArmSegments)
		seg.UnLoad();
	OnUnLoad();
}

void RobotArm::Update(float deltaTime)
{
	if (mRobotArmSegments.size() == 0)
		return;
	ControlSegment(deltaTime);
	ControlSelection();

	if (isAnimateing)
	{
		mAnimationTimeCounter += deltaTime;
		for (int i = 0; i < mRobotArmSegments.size(); ++i)
		{
			auto& seg = mRobotArmSegments[i];
			seg.mAnitmatePosition = seg.mAnimations[mCurrentAnimation]->GetValue_OnKeyframes(seg.mAnimations[mCurrentAnimation]->mPositionChannel.mKeyFrames, mAnimationTimeCounter);
			seg.mAnitmateRotation = seg.mAnimations[mCurrentAnimation]->GetValue_OnKeyframes(seg.mAnimations[mCurrentAnimation]->mRotationChannel.mKeyFrames, mAnimationTimeCounter);
			UpdateSegmentTransform(i, deltaTime);
		}

		if (mAnimationTimeCounter >= mRobotArmSegments[0].mAnimations[mCurrentAnimation]->GetLastFrameTime())
		{
			isAnimateing = false;
			if (isLoop)
			{
				isAnimateing = true;
				mAnimationTimeCounter = 0.0f;
			}
		}
	}

	OnUpdate(deltaTime);
}

void RobotArm::DebugUI()
{
	ImGui::Begin(GetTypeName().c_str());

	if (mRobotArmSegments.size() > 0)
	{
		auto& currentSeg = mRobotArmSegments[mCurrentSelectSegment];
		auto currentHeading = Normalize(Vector3::YAxis * currentSeg.mBone.offsetTransform * currentSeg.mBone.toParentTransform) * 5.0f;
		auto currentRight = Normalize(Cross(Vector3::YAxis, currentHeading)) * 5.0f;
		auto currentUp = Normalize(Cross(currentHeading, currentRight)) * 5.0f;
		auto currentPos =Vector3::Zero() *currentSeg.mBone.offsetTransform* currentSeg.mBone.toParentTransform;
		Matrix4 transform{
			currentRight.x, currentRight.y, currentRight.z, 0.0f,
			currentUp.x, currentUp.y, currentUp.z, 0.0f,
			currentHeading.x, currentHeading.y, currentHeading.z, 0.0f,
			currentPos.x,  currentPos.y,  currentPos.z, 1.0f,
		};
		SimpleDraw::AddTransform(transform);

		if (ImGui::CollapsingHeader("Animation"))
		{
			AnimationTabRender();
		}
	}
	if (ImGui::CollapsingHeader("Build"))
	{
		BuildTabRender();
	}

	if (ImGui::CollapsingHeader("Util"))
	{
		UtilTabRender();
	}
	
	OnDebugUI();
	
	ImGui::End();
}

void RobotArm::Render(const Camera & camera)
{
	OnRender(camera);
}

void RobotArm::AddAnimation()
{
	for (auto& seg : mRobotArmSegments)
	{
		seg.mAnimations.emplace_back();
	}
	mCurrentAnimation = static_cast<int>(mRobotArmSegments[0].mAnimations.size() - 1);
	AddKey(mCurrentAnimation, 0.0f);
}

void RobotArm::DeleteAnimation(int animationIndex)
{
	for (auto& seg : mRobotArmSegments)
	{
		seg.mAnimations.erase(seg.mAnimations.begin() + animationIndex);
	}
	mCurrentAnimation = static_cast<int>(mRobotArmSegments[0].mAnimations.size() - 1);
}

void RobotArm::PlayAniamtion(int animationIndex)
{
	isAnimateing = true;
	mAnimationTimeCounter = 0.0f;
	mCurrentAnimation = animationIndex;
}

void RobotArm::StopAniamtion()
{
	isAnimateing = false;
}

void RobotArm::AddKey(int animationIndex, float time)
{
	for (auto& seg : mRobotArmSegments)
	{
		if (seg.mAnimations[animationIndex] == nullptr)
		{
			seg.mAnimationBuilder.AddKey(seg.mAnitmatePosition, seg.mAnitmateRotation, time);
			seg.mAnimations[animationIndex] = seg.mAnimationBuilder.Build();
		}
		else
		{
			seg.mAnimationBuilder.AdaptExistingAniamtion(seg.mAnimations[animationIndex]);
			seg.mAnimationBuilder.AddKey(seg.mAnitmatePosition, seg.mAnitmateRotation, time);
			seg.mAnimations[animationIndex] = seg.mAnimationBuilder.Build();
		}
	}
}


void RobotArm::ControlSelection()
{
	auto& currentRobotArmSegment = mRobotArmSegments[mCurrentSelectSegment];
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::UP) && currentRobotArmSegment.mBone.childIndices.size() > 0)
	{
		mCurrentSelectSegment = currentRobotArmSegment.mBone.childIndices[0];
		mCurrentChildBoneIndex = 0;
	}
	if (inputSystem->IsKeyPressed(KeyCode::DOWN))
	{
		mCurrentSelectSegment = currentRobotArmSegment.mBone.parentIndex;
		if (mCurrentSelectSegment == -1) // hit the root 
			mCurrentSelectSegment = 0;
		mCurrentChildBoneIndex = 0;
	}

	if (currentRobotArmSegment.mBone.parentIndex == -1)// Root seg dont have paralla child
		return;

	auto& currentChildrenGroup = currentRobotArmSegment.mBone.parent->childIndices;
	int childrenSize = static_cast<int>(currentChildrenGroup.size());
	if (inputSystem->IsKeyPressed(KeyCode::RIGHT) && currentChildrenGroup.size() > 0)
	{	
		mCurrentChildBoneIndex = (++mCurrentChildBoneIndex) % childrenSize;
		mCurrentSelectSegment = mRobotArmSegments[currentRobotArmSegment.mBone.parentIndex].mBone.childIndices[mCurrentChildBoneIndex];
	}

	if (inputSystem->IsKeyPressed(KeyCode::LEFT) && currentChildrenGroup.size() > 0)
	{
		mCurrentChildBoneIndex -= 1;
		if (mCurrentChildBoneIndex < 0)
			mCurrentChildBoneIndex = childrenSize - 1;
		mCurrentSelectSegment = mRobotArmSegments[currentRobotArmSegment.mBone.parentIndex].mBone.childIndices[mCurrentChildBoneIndex];
	}
}

void RobotArm::AnimationTabRender()
{
	ImGui::BeginGroup();
	for (int i = 0; i < mRobotArmSegments[0].mAnimations.size(); i++)
	{
		auto& currentAnimation = mRobotArmSegments[0].mAnimations[i];
		
		//if (currentAnimation != nullptr)

		char buffer[16];

		sprintf(buffer, "Animation %d:", i);
		ImGui::Text(buffer);

		sprintf(buffer, "scrolling%d", i);

		ImGui::BeginChild(buffer, ImVec2(0, ImGui::GetFrameHeightWithSpacing() + 30), true, ImGuiWindowFlags_HorizontalScrollbar);

		for (int n = 0; n < currentAnimation->mPositionChannel.mKeyFrames.size(); n++)
		{
			float frameTime = currentAnimation->mPositionChannel.mKeyFrames[n].time;
			if (n > 0) ImGui::SameLine();
			ImGui::PushID(n);
			char num_buf[16];
			sprintf(num_buf, "%fs", frameTime);
			const char* label = num_buf;
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.2f, 0.6f, 0.6f));
			if (ImGui::Button(label, ImVec2(40.0f, 0.0f)))
			{
				//Load Keyframe
				for (int j = 0; j < mRobotArmSegments.size(); ++j)
				{
					auto& seg = mRobotArmSegments[j];
					seg.mAnitmatePosition = seg.mAnimations[i]->GetValue_OnKeyframes(seg.mAnimations[i]->mPositionChannel.mKeyFrames, seg.mAnimations[i]->mPositionChannel.mKeyFrames[n].time);
					seg.mAnitmateRotation = seg.mAnimations[i]->GetValue_OnKeyframes(seg.mAnimations[i]->mRotationChannel.mKeyFrames, seg.mAnimations[i]->mPositionChannel.mKeyFrames[n].time);
					UpdateSegmentTransform(j, 0.0f);
				}
			}
			ImGui::PopStyleColor(1);
			ImGui::PopID();
		}

		float scroll_x = ImGui::GetScrollX();
		float scroll_max_x = ImGui::GetScrollMaxX();
		ImGui::EndChild();

		ImGui::PushID(i);
		ImGui::InputFloat("Next frame time: ", &nextFrameTime);

		if (ImGui::Button("Add Key"))
		{
			AddKey(i, nextFrameTime);
			nextFrameTime = currentAnimation->GetLastFrameTime() + 1.0f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Play"))
			PlayAniamtion(i);
		ImGui::PopID();
		ImGui::Separator();
	}
	ImGui::Checkbox("isLoop", &isLoop);

	if (ImGui::Button("Add Aniamtion"))
	{
		AddAnimation();
	}
	ImGui::EndGroup();
}

void RobotArm::BuildTabRender()
{
	for (int i = 0; i < mAllTypeSegaments.size(); ++i)
	{
		ImGui::PushID(i);
		auto& currentSegType = mAllTypeSegaments[i];
		ImGui::BeginGroup();
		ImGui::Text("Arm Segment %d:", i);
		Vector4 currentAmbient = currentSegType.GetAmbientColor();
		ImGui::ColorEdit4("Segment Ambient: ##", &currentAmbient.r);
		currentSegType.SetAmbientColor(currentAmbient);

		if (ImGui::Button("Add Segment"))
		{
			AddSegment(i);
		}
		ImGui::EndGroup();
		ImGui::PopID();
	}
}

void RobotArm::UtilTabRender()
{
	ImGui::InputText("FileName: ", mUtilFileName, 256);
	
	if(ImGui::Button("Load"))
	{
		mUtilResult = LoadFromFile(mUtilFileName);
	}
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		mUtilResult = SaveToFile(mUtilFileName);
	}

	char buffer[256];
	if (mUtilResult)
		sprintf(buffer, "File action success!");
	else
		sprintf(buffer, "No file action processed.");
	ImGui::Text(buffer);
}

bool RobotArm::LoadFromFile(const char * fileName)
{
	if (!(IsEndOf(".RobotArmPTC", fileName) || IsEndOf(".RobotArmPTZ", fileName)))
	{
		return false;
	}
	FILE* inputFile;

	inputFile = fopen(fileName, "r");
	
	// Load [mAllTypeSegments]
	int numOfSegmentTypes;
	LOAD_SAFTY_CHECK(fscanf(inputFile, "Number Of SegType: %d\n", &numOfSegmentTypes));
	mAllTypeSegaments.clear();
	mAllTypeSegaments.reserve(numOfSegmentTypes);
	for (int i = 0; i < numOfSegmentTypes; ++i)
	{
		mAllTypeSegaments.emplace_back(mGeometry);
		mAllTypeSegaments.back().LoadFromFile(inputFile);
	}

	// Load [mCurrentSelectSegment]
	LOAD_SAFTY_CHECK(fscanf(inputFile, "currentSelectSegment: %d\n", &mCurrentSelectSegment));

	// Load [mCurrentChildBoneIndex]
	LOAD_SAFTY_CHECK(fscanf(inputFile, "currentChildBoneIndex: %d\n", &mCurrentChildBoneIndex));

	// Load [mMaxSeg]
	LOAD_SAFTY_CHECK(fscanf(inputFile, "maxSeg: %d\n", &mMaxSeg));
	mRobotArmSegments.clear();
	mRobotArmSegments.reserve(mMaxSeg);

	// Load [mRobotArmSegments]
	int numOfSeg;
	LOAD_SAFTY_CHECK(fscanf(inputFile, "Number Of Seg: %d\n", &numOfSeg));
	for (int i = 0; i < numOfSeg; ++i)
	{
		mRobotArmSegments.emplace_back();
		mRobotArmSegments.back().LoadFromFile(inputFile);
	}
	for (int i = 0; i < numOfSeg; ++i)
	{
		if (i != 0)
			mRobotArmSegments[i].mBone.parent = &mRobotArmSegments[mRobotArmSegments[i].mBone.parentIndex].mBone;

		for (auto& childIndex : mRobotArmSegments[i].mBone.childIndices)
			mRobotArmSegments[i].mBone.children.emplace_back(&mRobotArmSegments[childIndex].mBone);
	}

	mCurrentAnimation = 0;
	mAnimationTimeCounter = 0.0f;
	isAnimateing = false;

	fclose(inputFile);
	return true;
}

bool RobotArm::SaveToFile(const char * fileName)
{
	if (!(IsEndOf(".RobotArmPTC", fileName) || IsEndOf(".RobotArmPTZ", fileName)))
	{
		return false;
	}
	FILE* outputFile;

	outputFile = fopen(fileName, "w");

	// ave [mAllTypeSegments]
	int numOfSegmentTypes = static_cast<int>(mAllTypeSegaments.size());
	fprintf(outputFile, "Number Of SegType: %d\n", numOfSegmentTypes);
	for (int i = 0; i < numOfSegmentTypes; ++i)
	{
		mAllTypeSegaments[i].SaveToFile(outputFile);
	}

	// Load [mCurrentSelectSegment]
	fprintf(outputFile, "currentSelectSegment: %d\n", mCurrentSelectSegment);

	// Load [mCurrentChildBoneIndex]
	fprintf(outputFile, "currentChildBoneIndex: %d\n", mCurrentChildBoneIndex);

	// Load [mMaxSeg]
	fprintf(outputFile, "maxSeg: %d\n", mMaxSeg);

	// Load [mRobotArmSegments]
	int numOfSeg = static_cast<int>(mRobotArmSegments.size());
	fprintf(outputFile, "Number Of Seg: %d\n", numOfSeg);
	for (int i = 0; i < numOfSeg; ++i)
	{
		mRobotArmSegments[i].SaveToFile(outputFile);
	}

	fclose(outputFile);
	return true;

}
