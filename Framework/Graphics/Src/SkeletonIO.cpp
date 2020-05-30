#include "Precompiled.h"
#include "SkeletonIO.h"

using namespace NFGE::Graphics;

bool NFGE::Graphics::SkeletonIO::Save(const Skeleton & skeleton, FILE * file)
{
	uint32_t totalBones = static_cast<uint32_t>(skeleton.bones.size());

	fprintf_s(file, "Total_Bones: %d \n", totalBones);
	for (uint32_t i = 0; i < totalBones; ++i)
	{
		auto& bone = skeleton.bones[i];
		
		if (bone->parent)
			bone->parentIndex = bone->parent->index;
		else
			bone->parentIndex = -1;

		fprintf_s(file, "Bone_Name: %s\n", bone->name.c_str());
		fprintf_s(file, "Bone_Index: %d, Bone_ParentIndex: %d\n", bone->index, bone->parentIndex);
		
		uint32_t totalChildren = static_cast<uint32_t>(bone->childIndices.size());
		fprintf_s(file, "Bone_ChildrenIndices_Count: %d\n", totalChildren);
		fprintf_s(file, "Bone_ChildrenIndices: ");
		for (uint32_t j = 0; j < totalChildren; ++j)
		{
			bone->childIndices[j] = bone->children[j]->index;
			fprintf_s(file, "%d, ", bone->childIndices[j]);
		}
		
		auto& mat_toParent = bone->toParentTransform;
		//mat_toParent = NFGE::Math::Transpose(mat_toParent);
		fprintf_s(file, "\nBone_ToParentTransform: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,  \n",
			mat_toParent._11, mat_toParent._12, mat_toParent._13, mat_toParent._14,
			mat_toParent._21, mat_toParent._22, mat_toParent._23, mat_toParent._24,
			mat_toParent._31, mat_toParent._32, mat_toParent._33, mat_toParent._34,
			mat_toParent._41, mat_toParent._42, mat_toParent._43, mat_toParent._44
		);

		auto& mat_offset = bone->offsetTransform;
		//mat_offset = NFGE::Math::Transpose(mat_offset);
		fprintf_s(file, "Bone_OffsetTransform: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,  \n",
			mat_offset._11, mat_offset._12, mat_offset._13, mat_offset._14, 
			mat_offset._21, mat_offset._22, mat_offset._23, mat_offset._24,
			mat_offset._31, mat_offset._32, mat_offset._33, mat_offset._34,
			mat_offset._41, mat_offset._42, mat_offset._43, mat_offset._44
		);

	}

	return true;
}

bool NFGE::Graphics::SkeletonIO::Load(Skeleton & skeleton, FILE * file)
{
	uint32_t totalBones;
	fscanf_s(file, "Total_Bones: %d \n", &totalBones);

	skeleton.bones.reserve(totalBones);
	for (uint32_t i = 0; i < totalBones; ++i)
	{
		auto& newBone = skeleton.bones.emplace_back(std::make_unique<Bone>());
		
		//newBone->name = std::string(256, '\0');
		char buffer[256];
		fscanf(file, "Bone_Name: %s\n", buffer);
		newBone->name = buffer;
		 
		fscanf_s(file, "Bone_Index: %d, Bone_ParentIndex: %d\n", &newBone->index, &newBone->parentIndex);

		uint32_t totalChildren;
		fscanf_s(file, "Bone_ChildrenIndices_Count: %d\n", &totalChildren);
		newBone->childIndices.reserve(totalChildren);

		fscanf_s(file, "Bone_ChildrenIndices: ");
		for (uint32_t j = 0; j < totalChildren; ++j)
		{
			newBone->childIndices.emplace_back();
			fscanf_s(file, "%d, ", &newBone->childIndices[j]);
		}

		auto& mat_toParent = newBone->toParentTransform;
		fscanf_s(file, "\nBone_ToParentTransform: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,  \n",
			&mat_toParent._11, &mat_toParent._12, &mat_toParent._13, &mat_toParent._14,
			&mat_toParent._21, &mat_toParent._22, &mat_toParent._23, &mat_toParent._24,
			&mat_toParent._31, &mat_toParent._32, &mat_toParent._33, &mat_toParent._34,
			&mat_toParent._41, &mat_toParent._42, &mat_toParent._43, &mat_toParent._44
		);

		auto& mat_offset = newBone->offsetTransform;
		fscanf_s(file, "Bone_OffsetTransform: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,  \n",
			&mat_offset._11, &mat_offset._12, &mat_offset._13, &mat_offset._14,
			&mat_offset._21, &mat_offset._22, &mat_offset._23, &mat_offset._24,
			&mat_offset._31, &mat_offset._32, &mat_offset._33, &mat_offset._34,
			&mat_offset._41, &mat_offset._42, &mat_offset._43, &mat_offset._44
		);


	}

	for (uint32_t i = 0; i < totalBones; ++i)
	{
		auto& bone = skeleton.bones[i];
		if (bone->parentIndex == -1)
		{
			bone->parent = nullptr;
			skeleton.root = bone.get();
		}
		else
			bone->parent = skeleton.bones[bone->parentIndex].get();

		for (uint32_t j = 0; j < bone->childIndices.size(); ++j)
		{
			bone->children.emplace_back(nullptr);
			bone->children[j] = skeleton.bones[bone->childIndices[j]].get();
		}
	}



	return true;
}
