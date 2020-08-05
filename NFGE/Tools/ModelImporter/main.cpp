#define _CRT_SECURE_NO_WARNINGS

#include <NFGE/Inc/NFGE.h>
#include <assimp/Importer.hpp>		// c++ importer interface
#include <assimp/scene.h>		// Output data
#include <assimp/postprocess.h>		// Post process flags
#include <cstdio>

const bool test = true;
const char* arg_1 = "../../Assets/Model/Original/IdleGunTurnLeft_45-.fbx"; 
const char* arg_2 = "../../Assets/Model/IdleGunTurnLeft_45.NFModelRV";
bool arg_3 = true;

using namespace NFGE::Graphics;

using BoneIndexLookup = std::map<std::string, int>;	// Used to look up bone by name
using NFGEModelTextureType = NFGE::Graphics::MeshTextureMaterial::ModelTextureType;

struct Argument
{
	const char* inputFileName = nullptr;
	const char* outputFileName = nullptr;
	bool isAnimationOnly = false;
};

struct TextureLoadInfo
{
	aiTextureType mAiType;
	const char* mNameFix;
	uint32_t mask;
};

const std::array<TextureLoadInfo, 18> AllTextureTypeLoadInfo = { 
	{
		{ aiTextureType::aiTextureType_DIFFUSE,				"_diffuse_",			NFGE::Graphics::MeshTextureMaterial::TextureType_DIFFUSE },
		{ aiTextureType::aiTextureType_SPECULAR,			"_specular_",			NFGE::Graphics::MeshTextureMaterial::TextureType_SPECULAR },
		{ aiTextureType::aiTextureType_AMBIENT,				"_ambient_",			NFGE::Graphics::MeshTextureMaterial::TextureType_AMBIENT },
		{ aiTextureType::aiTextureType_EMISSIVE,			"_emissive_",			NFGE::Graphics::MeshTextureMaterial::TextureType_EMISSIVE},
		{ aiTextureType::aiTextureType_HEIGHT,				"_height_",				NFGE::Graphics::MeshTextureMaterial::TextureType_HEIGHT },
		{ aiTextureType::aiTextureType_NORMALS,				"_normal_",				NFGE::Graphics::MeshTextureMaterial::TextureType_NORMALS },
		{ aiTextureType::aiTextureType_SHININESS,			"_shininess_",			NFGE::Graphics::MeshTextureMaterial::TextureType_SHININESS },
		{ aiTextureType::aiTextureType_OPACITY,				"_opacity_",			NFGE::Graphics::MeshTextureMaterial::TextureType_OPACITY },
		{ aiTextureType::aiTextureType_DISPLACEMENT,		"_displacement_",		NFGE::Graphics::MeshTextureMaterial::TextureType_DISPLACEMENT },
		{ aiTextureType::aiTextureType_LIGHTMAP,			"_lightmap_",			NFGE::Graphics::MeshTextureMaterial::TextureType_LIGHTMAP },
		{ aiTextureType::aiTextureType_REFLECTION,			"_reflection_",			NFGE::Graphics::MeshTextureMaterial::TextureType_REFLECTION},
		{ aiTextureType::aiTextureType_BASE_COLOR,			"_basecolor_",			NFGE::Graphics::MeshTextureMaterial::TextureType_BASE_COLOR },
		{ aiTextureType::aiTextureType_NORMAL_CAMERA,		"_normalcamera_",		NFGE::Graphics::MeshTextureMaterial::TextureType_NORMAL_CAMERA },
		{ aiTextureType::aiTextureType_EMISSION_COLOR,		"_emissioncolor_",		NFGE::Graphics::MeshTextureMaterial::TextureType_EMISSION_COLOR },
		{ aiTextureType::aiTextureType_METALNESS,			"_metalness_",			NFGE::Graphics::MeshTextureMaterial::TextureType_METALNESS },
		{ aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS,	"_diffuseroughness_",	NFGE::Graphics::MeshTextureMaterial::TextureType_DIFFUSE_ROUGHNESS },
		{ aiTextureType::aiTextureType_AMBIENT_OCCLUSION,	"_ambientocclusion_",	NFGE::Graphics::MeshTextureMaterial::TextureType_AMBIENT_OCCLUSION },
		{ aiTextureType::aiTextureType_UNKNOWN,				"_unknown_",			NFGE::Graphics::MeshTextureMaterial::TextureType_UNKNOWN },
	} 
};
//TODO :: create 18 of above as global;

bool IsEndOf(const char* end, const char* string)
{
	int endLength = strlen(end);
	int stringLength = strlen(string);
	for (int i = 0; i < endLength; i++)
	{
		if (end[endLength - 1 - i] != string[stringLength - 1 - i]) return false;
	}
	return true;
}

//old
//bool ParseArgs(int argc, char* argv[], Argument& args)
//{
//	return true;
//}

//new
std::optional<Argument> ParseArgs(int argc, char* argv[])
{
	// We need at least 4 arguments
	if (argc < 4)
		return std::nullopt;

	Argument args;
	args.inputFileName = argv[argc - 3];
	args.outputFileName = argv[argc - 2];
	args.isAnimationOnly = argv[argc - 1];

	return args;
}

bool LoadTexture(const aiScene* scene, aiMaterial* inputMaterial, NFGEModelTextureType textureType, NFGE::Graphics::MeshTextureMaterial::MeshTextureHolder& textureHolder, const Argument& args);

void PrintUsage()
{
	printf(
		"== ModelImporter Help == \n"
		"\n"
		"Usage:\n"
		"	ModelImporter.exe [Options] <InputFile> <OutputFile>\n"
		"\n"
		"Options:\n"
		"b--- Export Binary"
		"v--- Export Real Value"
		"\n"
		"	<none>\n"
		"\n"
	);
}

inline NFGE::Math::Vector3 Convert(const aiVector3D& v)
{
	return { v.x, v.y, v.z };
}

inline NFGE::Math::Quaternion Convert(const aiQuaternion& q)
{
	return { q.x, q.y, q.z, q.w };
}

inline NFGE::Math::Matrix4 Convert(const aiMatrix4x4& m)
{
	NFGE::Math::Matrix4 mat = *(reinterpret_cast<const NFGE::Math::Matrix4*>(&m));
	return NFGE::Math::Transpose(mat);
}

// Check if input Bone exists in skeleton, if so just return the index.
// Otherwise, add it to the skeleton. The aiBone must have a name!
int TryAddBoneIndex(const aiBone* inputBone, Skeleton& skeleton, BoneIndexLookup& boneIndexLoopup)
{
	std::string name = inputBone->mName.C_Str();
	ASSERT(!name.empty(), "Error: inputBone has no name!");
	auto iter = boneIndexLoopup.find(name);
	if (iter != boneIndexLoopup.end())
		return iter->second;

	//	Add a new bone in the skeleton for this
	auto& newBone = skeleton.bones.emplace_back(std::make_unique<Bone>());
	newBone->name = std::move(name);
	newBone->index = static_cast<int>(skeleton.bones.size()) - 1;
	newBone->offsetTransform = Convert(inputBone->mOffsetMatrix);

	// Cache the bone index
	boneIndexLoopup.emplace(newBone->name, newBone->index);
	return newBone->index;
}

// Recursively walk the aiScene tree and add/link bone as we find them to our skeleton.
Bone* BuildSkeleton(const aiNode& sceneNode, Bone* parent, Skeleton& skeleton, BoneIndexLookup& boneIndexLookUp)
{
	Bone* bone = nullptr;
	std::string name = sceneNode.mName.C_Str();
	auto iter = boneIndexLookUp.find(name);
	if (iter != boneIndexLookUp.end())
	{
		bone = skeleton.bones[iter->second].get();
	}
	else
	{
		bone = skeleton.bones.emplace_back(std::make_unique<Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		bone->offsetTransform = NFGE::Math::Matrix4::sIdentity(); // This was Matrix::Identity before, we will see what blows up
		if (name.empty())
			bone->name = "NoName" + std::to_string(bone->index);
		else
			bone->name = std::move(name);

		// Cache the bone index
		boneIndexLookUp.emplace(bone->name, bone->index);
	}

	bone->parent = parent;
	if (parent != nullptr)
		bone->parentIndex = parent->index;
	else
		bone->parentIndex = -1;

	bone->toParentTransform = Convert(sceneNode.mTransformation);
	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
	{
		Bone* child = BuildSkeleton(*sceneNode.mChildren[i], bone, skeleton, boneIndexLookUp);
		bone->children.push_back(child);
		bone->childIndices.push_back(child->index);
	} 
	return bone;
}

void SaveModel(const Argument& args, const Model& model)
{
	FILE* outFile;
	bool isOutPutBinary = IsEndOf("NFModel", args.outputFileName);
	if (isOutPutBinary)
		outFile = fopen(args.outputFileName, "wb");
	else
		outFile = fopen(args.outputFileName, "w");
	if (!model.SaveToFile(outFile, isOutPutBinary ? NFGE::Core::IOOption::Binary : NFGE::Core::IOOption::RV))
		printf("Error: %s\n", "Lost Mesh data when saving it");
	fclose(outFile);
}

void SaveSkeleton(const Argument& args, const Skeleton& skeleton)
{
	std::filesystem::path path = args.outputFileName;
	path.replace_extension("NFSkeleton");
	std::string fileName = path.u8string();
	FILE* outFile =  fopen(fileName.c_str(), "w");
	NFGE::Graphics::SkeletonIO::Save(skeleton, outFile);
	fclose(outFile);
}

void SaveAnimationClips(const Argument& args, const AnimationSet& aniamtionSet)
{
	std::filesystem::path path = args.outputFileName;
	path.replace_extension("NFAnimationSet");

	FILE* file = nullptr;
	fopen_s(&file, path.u8string().c_str(), "w");
	AnimationSetIO::Save(aniamtionSet, file);
	fclose(file);
}

int main(int argc, char* argv[])
{
	const auto argsOpt = ParseArgs(argc, argv);
	Argument args;

	if (test)
	{
		args.inputFileName = arg_1;
		args.outputFileName = arg_2;
		args.isAnimationOnly = arg_3;
	}
	else
	{
		if (!argsOpt.has_value())
		{
			PrintUsage();
			return -1;
		}
		args = argsOpt.value();
	}

	FILE* inFile = fopen(args.inputFileName, "r");
	

	if (inFile == nullptr)
	{
		printf("Error! Could not open file\n");
		fclose(inFile);
		//fclose(outFile);
		return -1;
	}

	// Create an instance of the importer class to do teh parseing for us.
	Assimp::Importer importer;

	// Try to import the model into a scene;
	const aiScene* scene = importer.ReadFile(args.inputFileName, aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		fclose(inFile);
		//fclose(outFile);
		return -1;
	}

	// Scene
	// +- mesh[_][_][_][_][_][_][_][_]...
	// +- material[_][_][_][_][_][_][_][_]...
	// +- animation[_][_][_][_][_][_][_][_]...
	// +- mRootNode
	//		+- Node
	//		+- Node
	//			+- Node
	//		...

	NFGE::Graphics::Model model;
	Skeleton skeleton;
	BoneIndexLookup boneIndexLookup;
	AnimationSet animationClips;
	// Look for mesh data.
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");

		const uint32_t numMeshes = scene->mNumMeshes;
		//model.meshes.reserve(numMeshes);
		//model.materialIndices.reserve(numMeshes);
		model.mMeshData.reserve(numMeshes);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			uint32_t numVertices = inputMesh->mNumVertices;
			uint32_t numIndices = inputMesh->mNumFaces * 3;

			printf("Reading vertices ...\n");

			std::vector<NFGE::Graphics::BoneVertex> vertices;
			std::vector<int> indices;

			const aiVector3D* position = inputMesh->mVertices;
			const aiVector3D* normal = inputMesh->mNormals;
			const aiVector3D* tangent = inputMesh->mTangents;
			const aiVector3D* texcood = inputMesh->HasTextureCoords(0) ? inputMesh->mTextureCoords[0] : nullptr;
			if (texcood == nullptr)
			{
				printf("Warning: %s\n", "Model dont have texcood");
				for (size_t i = 0; i < numVertices; ++i)
				{
					vertices.emplace_back(BoneVertex{
						NFGE::Math::Vector3{ position[i].x, position[i].y, position[i].z },
						NFGE::Math::Vector3{ normal[i].x, normal[i].y, normal[i].z },
						NFGE::Math::Vector3{ tangent[i].x, tangent[i].y, tangent[i].z },
						NFGE::Math::Vector2{ 0.0f, 0.0f},
						});
				}
			}
			else if (tangent == nullptr)
			{
				printf("Warning: %s\n", "Model dont have tangent");
				for (size_t i = 0; i < numVertices; ++i)
				{
					vertices.emplace_back(BoneVertex{
						NFGE::Math::Vector3{ position[i].x, position[i].y, position[i].z },
						NFGE::Math::Vector3{ normal[i].x, normal[i].y, normal[i].z },
						NFGE::Math::Vector3{ 1.0f,0.0f, 0.0f},
						NFGE::Math::Vector2{ texcood[i].x, texcood[i].y },
						});
				}
			}
			else
			{
				for (size_t i = 0; i < numVertices; ++i)
				{
					vertices.emplace_back(BoneVertex{
						NFGE::Math::Vector3{ position[i].x, position[i].y, position[i].z },
						NFGE::Math::Vector3{ normal[i].x, normal[i].y, normal[i].z },
						NFGE::Math::Vector3{ tangent[i].x, tangent[i].y, tangent[i].z },
						NFGE::Math::Vector2{ texcood[i].x, texcood[i].y },
						});
				}
			}

			

			const aiFace* faces = inputMesh->mFaces;

			for (size_t i = 0; i < inputMesh->mNumFaces; ++i)
			{
				for (size_t j = 0; j < faces[i].mNumIndices; j++)
				{
					auto& currentIndices = faces[i].mIndices;
					indices.push_back(currentIndices[j]);
				}
			}

			if (inputMesh->HasBones())
			{
				printf("Reading bone weights...\n");
				// Track how many weights have added to each vertex so far
				std::vector<int> numWeights(vertices.size(), 0);//-------------------------------------------[Create a counter of blend bones for every vertex of current mesh]
				for (uint32_t meshBoneIndex = 0; meshBoneIndex < inputMesh->mNumBones; ++meshBoneIndex)//----[Loop through current bones of current Mesh from .fbx]
				{
					aiBone* inputBone = inputMesh->mBones[meshBoneIndex];//----------------------------------[Get current bone from all bones for current mesh]
					int boneIndex = TryAddBoneIndex(inputBone, skeleton, boneIndexLookup);//-----------------[Add this bone to our skeleton and get its index]

					for (uint32_t weightIndex = 0; weightIndex < inputBone->mNumWeights; ++weightIndex)//----[Current bone store which vertex using it and how much weight,(which in our engine we store this Info in to vertex), loop through those weight]
					{
						const aiVertexWeight& weight = inputBone->mWeights[weightIndex];//-------------------[Get the <Weight> object that store current vertex index that is using current bone, and its blend weight]
						auto& vertex = vertices[weight.mVertexId];//-----------------------------------------[Cache the vertex in local scope]
						auto& count = numWeights[weight.mVertexId];//----------------------------------------[Cache the vertex weight counter in local scope]
						
						// Our vertices can hold at most up to 4 weights
						if (count < 4) 
						{
							vertex.boneIndices[count] = boneIndex;//-----------------------------------------[Store the boneIndex that we use in out skeleton for current bone]
							vertex.boneWeights[count] = weight.mWeight;//------------------------------------[Store the weight]
							++count;
							// TODO:: ------------------------POSSIBLE IMPROVEMENT---------------------------[Get every blend bone, we pick the most effect 4 bone and normalize the weight]]
						}
					}
				}
			}

			// Create NFGE::Mesh and pass in vertices/indices
			model.mMeshData.emplace_back();
			auto& currentMesh = model.mMeshData[meshIndex];
			currentMesh.mesh.vertices.insert(currentMesh.mesh.vertices.end(), std::begin(vertices), std::end(vertices));
			currentMesh.mesh.indices.insert(currentMesh.mesh.indices.end(), std::begin(indices), std::end(indices));

			currentMesh.materialIndex = inputMesh->mMaterialIndex;
			
			
		}
	}

	// Look for material data.
	if (!args.isAnimationOnly)
	{
		if (scene->HasMaterials())
		{
			printf("Reading materials...\n");

			model.mContext.modelMaterials.reserve(scene->mNumMaterials);
			for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
			{
				printf("Reading material %d...\n", i);
				aiMaterial* inputMaterial = scene->mMaterials[i];

				model.mContext.modelMaterials.emplace_back();
				for (int i = 0; i < static_cast<int>(AllTextureTypeLoadInfo.size()); i++)
				{
					if (LoadTexture(scene, inputMaterial, static_cast<NFGEModelTextureType>(i), model.mContext.modelMaterials.back().mTextureHolders[i], args))
						model.mContext.modelMaterials.back().mAvliableTextureType |= AllTextureTypeLoadInfo[i].mask;
				}
			}
		}
	}
	

	// Check if we have skeleton information
	if (!skeleton.bones.empty())
	{
		printf("Building skeleton...\n");
		BuildSkeleton(*scene->mRootNode, nullptr, skeleton, boneIndexLookup);
	}

	// Look for animation data
	if (scene->HasAnimations())
	{
		printf("Reading animations...\n");
		for (uint32_t animaIndex = 0; animaIndex < scene->mNumAnimations; ++animaIndex)
		{
			const aiAnimation* inputAnima = scene->mAnimations[animaIndex];
			auto& animClip = animationClips.boneAnimationClips.emplace_back(std::make_unique<AnimationClip>());

			if (inputAnima->mName.length > 0)
			{
				animClip->name = inputAnima->mName.C_Str();
			}
			else
			{
				animClip->name = "Anima" + std::to_string(animaIndex);
			}

			animClip->duration = static_cast<float>(inputAnima->mDuration);
			animClip->ticksPerSecond = static_cast<float>(inputAnima->mTicksPerSecond);

			printf("Reading bone animations for %s...\n", animClip->name.c_str());

			// Reserve space so we have one animation slot per bone, not that not
			// all bones will have maimtion so some slot will stay empty. However,
			// keeping them same size meas we can use bone index directly to look
			// up animations.
			animClip->boneAnimations.resize(skeleton.bones.size());
			for (uint32_t boneAnimIndex = 0; boneAnimIndex < inputAnima->mNumChannels; ++boneAnimIndex)
			{
				const aiNodeAnim* inputBoneAnim = inputAnima->mChannels[boneAnimIndex];
				int slotIndex = boneIndexLookup[inputBoneAnim->mNodeName.C_Str()];
				auto& boneAnim = animClip->boneAnimations[slotIndex];
				boneAnim = std::make_unique<BoneAnimation>();

				boneAnim->boneName = inputBoneAnim->mNodeName.C_Str();

				AnimationBuilder builder;
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumPositionKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumRotationKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mRotationKeys[keyIndex];
					builder.AddRotationKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumScalingKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mScalingKeys[keyIndex];
					builder.AddScaleKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}

				boneAnim->animation = builder.Build();
			}
		}
	}

	if (!args.isAnimationOnly)
	{
		SaveModel(args, model);
		SaveSkeleton(args, skeleton);
	}
	SaveAnimationClips(args, animationClips);

	fclose(inFile);

	printf("All Done!\n");
	system("pause");
	return 0;
}




bool LoadTexture(const aiScene* scene, aiMaterial* inputMaterial, NFGEModelTextureType textureType, NFGE::Graphics::MeshTextureMaterial::MeshTextureHolder& textureHolder, const Argument& args)
{
	// For new we are only intersted in first diffuse maps
	bool ret = false;
	auto& currentLoadingTextureType = AllTextureTypeLoadInfo[textureType];
	const uint32_t textureCount = inputMaterial->GetTextureCount(currentLoadingTextureType.mAiType);
	
	textureHolder.mNumOfTextures = textureCount;
	textureHolder.mFilenames.reserve(textureCount);
	for(size_t i = 0; i < textureCount; ++i)
	{
		
		aiString texturePath;
		//Link: github.com/assimp/assimp/issues/1830
		if (inputMaterial->GetTexture(currentLoadingTextureType.mAiType, i, &texturePath) == aiReturn_SUCCESS)
		{
			// If texture path starts with *,the the texture is embeded;
			const aiTexture* inputTexture = scene->GetEmbeddedTexture(texturePath.C_Str());
			if (inputTexture)
			{
				std::string fileName = args.inputFileName;
				fileName.erase(fileName.length() - 4); // remove .fbx extension
				fileName += inputMaterial->GetName().C_Str();
				fileName += currentLoadingTextureType.mNameFix;
				//fileName += texturePath.C_Str()[1];
				char indexStringbuffer[256];
				_itoa(i, indexStringbuffer, 10);
				fileName += indexStringbuffer;

				ASSERT(scene->HasTextures(), "Error: No embeded texture found!");

				int textureIndex = atoi(texturePath.C_Str() + 1);
				ASSERT(textureIndex < (int)scene->mNumTextures, "Error: Invalid texture index! ");


				ASSERT(inputTexture->mHeight == 0, "Error: Uncompressed texture found!");

				if (inputTexture->CheckFormat("jpg"))
					fileName += ".jpg";
				else if (inputTexture->CheckFormat("png"))
					fileName += ".png";
				else
					ASSERT(inputTexture->mHeight == 0, "Error: Unrecogized texture format. Skipping...\n");

				printf("Extrating embeded texture %s\n", fileName.c_str());

				//std::string fullFileName = args.outputFileName;
				//fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
				//fullFileName += fileName;

				FILE* file = nullptr;
				fopen_s(&file, fileName.c_str(), "wb");
				size_t written = fwrite(inputTexture->pcData, 1, inputTexture->mWidth, file);
				ASSERT(written == inputTexture->mWidth, "Error: Failed to extract embedded texture!");
				fclose(file);

				printf("Adding embeded texture %s\n", fileName.c_str());
				
				textureHolder.mFilenames.emplace_back(std::move(fileName));
			}
			else
			{
				std::filesystem::path filePath = texturePath.C_Str();
				std::string fileName = filePath.filename().u8string();

				std::string fullFileName = args.inputFileName;
				fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
				fullFileName += fileName;
				printf("Adding excluded texture %s\n", fullFileName.c_str());
				textureHolder.mFilenames.emplace_back(std::move(fullFileName));
			}
		}
		ret = true;
	}
	
	if (textureCount < 1)
	{
		printf("Current mesh doesn't have %s texture. Skipping...\n", currentLoadingTextureType.mNameFix);
		ret = false;
	}

	return ret;
}