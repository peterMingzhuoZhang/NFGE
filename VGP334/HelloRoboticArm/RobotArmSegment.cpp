#include "RobotArmSegment.h"

RobotArmSegment::RobotArmSegment(int type, const Bone & bone, const Matrix4 & boneMat, const Vector3 & animaPos, const Quaternion & animaRot)
	: mArmSegmentType(type)
	, mBone(bone)
	, boneMatrix(boneMat)
	, mAnitmatePosition(animaPos)
	, mAnitmateRotation(animaRot)
{
}

RobotArmSegment::RobotArmSegment(const RobotArmSegment & other)
	: mArmSegmentType(other.mArmSegmentType)
	, mBone(other.mBone)
	, boneMatrix(other.boneMatrix)
	, mAnitmatePosition(other.mAnitmatePosition)
	, mAnitmateRotation(other.mAnitmateRotation)
{
	for (int i = 0; i < other.mAnimations.size(); ++i)
	{
		mAnimations.emplace_back(std::make_unique<Animation>(*other.mAnimations[i].get()));
	}
}

void RobotArmSegment::UnLoad()
{
	for (auto& animation : mAnimations)
	{
		animation.release();
		animation.reset();
	}
}

void RobotArmSegment::LoadFromFile(FILE * inputFile)
{
	fscanf(inputFile, "segType: %d\n", &mArmSegmentType);
	fscanf(inputFile, "bone_index: %d\n", &mBone.index);
	fscanf(inputFile, "bone_parentIndex: %d\n", &mBone.parentIndex);
	int childrenCount = 0;
	fscanf(inputFile, "bone_childrenCound: %d\n", &childrenCount);
	mBone.children.clear();
	mBone.children.reserve(childrenCount);
	mBone.childIndices.clear();
	mBone.childIndices.reserve(childrenCount);
	for (size_t i = 0; i < childrenCount; ++i)
	{
		mBone.childIndices.emplace_back();
		fscanf(inputFile, "childIndex: %d\n", &(mBone.childIndices.back()));
	}
	fscanf(inputFile, "toParentTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		&mBone.toParentTransform._11, &mBone.toParentTransform._12, &mBone.toParentTransform._13, &mBone.toParentTransform._14,
		&mBone.toParentTransform._21, &mBone.toParentTransform._22, &mBone.toParentTransform._23, &mBone.toParentTransform._24,
		&mBone.toParentTransform._31, &mBone.toParentTransform._32, &mBone.toParentTransform._33, &mBone.toParentTransform._34,
		&mBone.toParentTransform._41, &mBone.toParentTransform._42, &mBone.toParentTransform._43, &mBone.toParentTransform._44
	);
	fscanf(inputFile, "offsetTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		&mBone.offsetTransform._11, &mBone.offsetTransform._12, &mBone.offsetTransform._13, &mBone.offsetTransform._14,
		&mBone.offsetTransform._21, &mBone.offsetTransform._22, &mBone.offsetTransform._23, &mBone.offsetTransform._24,
		&mBone.offsetTransform._31, &mBone.offsetTransform._32, &mBone.offsetTransform._33, &mBone.offsetTransform._34,
		&mBone.offsetTransform._41, &mBone.offsetTransform._42, &mBone.offsetTransform._43, &mBone.offsetTransform._44
	);
	fscanf(inputFile, "boneMatrix: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		&boneMatrix._11, &boneMatrix._12, &boneMatrix._13, &boneMatrix._14,
		&boneMatrix._21, &boneMatrix._22, &boneMatrix._23, &boneMatrix._24,
		&boneMatrix._31, &boneMatrix._32, &boneMatrix._33, &boneMatrix._34,
		&boneMatrix._41, &boneMatrix._42, &boneMatrix._43, &boneMatrix._44
	);
	fscanf(inputFile, "animatePos: %f %f %f\n", &mAnitmatePosition.x, &mAnitmatePosition.y, &mAnitmatePosition.z);
	fscanf(inputFile, "animateRot: %f %f %f %f\n", &mAnitmateRotation.x, &mAnitmateRotation.y, &mAnitmateRotation.z, &mAnitmateRotation.w);

	UnLoad();
	int animationCount = 0;
	fscanf(inputFile, "aniamtionCount: %d\n", &animationCount);
	mAnimations.reserve(animationCount);
	for (int i = 0; i < animationCount; ++i)
	{
		mAnimations.emplace_back(std::make_unique<Animation>());

		int keyFramesCount = 0;
		fscanf(inputFile, "keyframesCount: %d\n", &keyFramesCount);

		for (int j = 0; j < keyFramesCount; ++j)
		{
			Keyframe<Vector3> posKey;
			Keyframe<Quaternion> rotKey;
			fscanf(inputFile, "pos: %f %f %f %f rot: %f %f %f %f %f\n",
				&posKey.value.x, &posKey.value.y, &posKey.value.z, &posKey.time,
				&rotKey.value.x, &rotKey.value.y, &rotKey.value.z, &rotKey.value.w, &rotKey.time
			);
			mAnimationBuilder.AddKey(posKey.value, rotKey.value, posKey.time);
		}
		mAnimations.back() = mAnimationBuilder.Build();
	}

}

void RobotArmSegment::SaveToFile(FILE * outputFile)
{
	fprintf(outputFile, "segType: %d\n", mArmSegmentType);
	fprintf(outputFile, "bone_index: %d\n", mBone.index);
	fprintf(outputFile, "bone_parentIndex: %d\n", mBone.parentIndex);

	int childrenCount = static_cast<int>(mBone.childIndices.size());
	fprintf(outputFile, "bone_childrenCound: %d\n", childrenCount);
	for (size_t i = 0; i < childrenCount; ++i)
	{
		fprintf(outputFile, "childIndex: %d\n", (mBone.childIndices[i]));
	}
	fprintf(outputFile, "toParentTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		mBone.toParentTransform._11, mBone.toParentTransform._12, mBone.toParentTransform._13, mBone.toParentTransform._14,
		mBone.toParentTransform._21, mBone.toParentTransform._22, mBone.toParentTransform._23, mBone.toParentTransform._24,
		mBone.toParentTransform._31, mBone.toParentTransform._32, mBone.toParentTransform._33, mBone.toParentTransform._34,
		mBone.toParentTransform._41, mBone.toParentTransform._42, mBone.toParentTransform._43, mBone.toParentTransform._44
	);
	fprintf(outputFile, "offsetTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		mBone.offsetTransform._11, mBone.offsetTransform._12, mBone.offsetTransform._13, mBone.offsetTransform._14,
		mBone.offsetTransform._21, mBone.offsetTransform._22, mBone.offsetTransform._23, mBone.offsetTransform._24,
		mBone.offsetTransform._31, mBone.offsetTransform._32, mBone.offsetTransform._33, mBone.offsetTransform._34,
		mBone.offsetTransform._41, mBone.offsetTransform._42, mBone.offsetTransform._43, mBone.offsetTransform._44
	);
	fprintf(outputFile, "boneMatrix: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		boneMatrix._11, boneMatrix._12, boneMatrix._13, boneMatrix._14,
		boneMatrix._21, boneMatrix._22, boneMatrix._23, boneMatrix._24,
		boneMatrix._31, boneMatrix._32, boneMatrix._33, boneMatrix._34,
		boneMatrix._41, boneMatrix._42, boneMatrix._43, boneMatrix._44
	);
	fprintf(outputFile, "animatePos: %f %f %f\n", mAnitmatePosition.x, mAnitmatePosition.y, mAnitmatePosition.z);
	fprintf(outputFile, "animateRot: %f %f %f %f\n", mAnitmateRotation.x, mAnitmateRotation.y, mAnitmateRotation.z, mAnitmateRotation.w);

	int animationCount = static_cast<int>(mAnimations.size());
	fprintf(outputFile, "aniamtionCount: %d\n", animationCount);
	for (int i = 0; i < animationCount; ++i)
	{
		auto& positionChannel = mAnimations[i].get()->mPositionChannel;
		auto& rotationChannel = mAnimations[i].get()->mRotationChannel;
		auto& scaleChannel = mAnimations[i].get()->mScaleChannel;
		int keyFramesCount = static_cast<int>(positionChannel.mKeyFrames.size());
		fprintf(outputFile, "keyframesCount: %d\n", keyFramesCount);
		for (int j = 0; j < keyFramesCount; ++j)
		{
			fprintf(outputFile, "pos: %f %f %f %f rot: %f %f %f %f %f\n",
				positionChannel.mKeyFrames[j].value.x, positionChannel.mKeyFrames[j].value.y, positionChannel.mKeyFrames[j].value.z, positionChannel.mKeyFrames[j].time,
				rotationChannel.mKeyFrames[j].value.x, rotationChannel.mKeyFrames[j].value.y, rotationChannel.mKeyFrames[j].value.z, rotationChannel.mKeyFrames[j].value.w, rotationChannel.mKeyFrames[j].time
			);
		}
	}
}