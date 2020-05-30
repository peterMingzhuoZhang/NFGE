#include "PTSFSkeleton.h"

namespace
{
	void UpdateTransfrom(Bone* bone, std::vector<Matrix4>& boneMatrix)
	{
		if (bone->parent)
			boneMatrix[bone->index] = bone->toParentTransform * boneMatrix[bone->parentIndex];
		else
			boneMatrix[bone->index]= bone->toParentTransform;
		for (auto child : bone->children)
			UpdateTransfrom(child, boneMatrix);
	}
}

void PTSFSkeleton::Load(const char * fileName, DirectionalLight* light)
{
	mBoneMesh.Load(NFGE::Graphics::Colors::Gray, NFGE::Graphics::Colors::GreenYellow, light);

	FILE* infile = fopen(fileName, "r");
	SkeletonIO::Load(mSkeleton, infile);
	fclose(infile);

	for (auto& bone : mSkeleton.bones)
		boneMatrix.emplace_back();
	//TODO :: init the <mCurrentTran> & <mCurrentRotation> in each DynamicToParent 
	UpdateTransfrom(mSkeleton.root, boneMatrix);
}

void PTSFSkeleton::Update(float deltaTime)
{

}

void PTSFSkeleton::Render(const Camera & camera)
{
	RenderSkeleton(mSkeleton.root, camera);
}

void PTSFSkeleton::DebugUI()
{
}

void PTSFSkeleton::UnLoad()
{
}

void PTSFSkeleton::RenderSkeleton(Bone * bone, const Camera& camera)
{
	mBoneMesh.Render(bone->toParentTransform, boneMatrix[bone->index], camera, false);
	for (auto child : bone->children)
	{
		RenderSkeleton(child, camera);
	}
}

void PTSFSkeleton::UpdateToParentTransform(int index)
{
	auto& currentToParent = mSkeleton.bones[index]->toParentTransform;
	//currentToParent = //Math::MatrixRotationQuaternion(mDynamicToParentTransform.mCurrentRotation) * Matrix4::sTranslation(mDynamicToParentTransform.mCurrentTranslate);
	UpdateTransfrom(mSkeleton.bones[index].get(), boneMatrix);
}
