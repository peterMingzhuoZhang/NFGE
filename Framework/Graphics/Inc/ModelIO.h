#ifndef INCLUDDE_NFGE_GRAPHICS_MODELIO_H
#define INCLUDDE_NFGE_GRAPHICS_MODELIO_H

#include "AnimationSet.h"
#include "MeshIO.h"

#define LOAD_SAFTY_CHECK(readResult) if (readResult == EOF)\
			{	LOG("[NFGE::Graphics::Model::LoadRV()] --- Error: File currupted! Reach EOF while Loading.");\
				return false;}

namespace NFGE::Graphics
{
	namespace ModelIO
	{
		bool IsEndOf(const char* end, const char* string);
		template<class T>
		bool SaveBianry(const T& model, FILE * file)
		{
			return false;
		}

		template<class T>
		bool SaveRV(const T& model, FILE * file) // RV -> Readable Value;
		{
			bool ret;

			int numOfMeshes = static_cast<int>(model.mMeshData.size());
			int numOfMaterialIndices = numOfMeshes;
			int numOfMaterials = static_cast<int>(model.mContext.modelMaterials.size());

			// Save Meshes data
			fprintf(file, "Number Of Meshes: %d\n", numOfMeshes);
			if (numOfMeshes != numOfMaterialIndices)
			{
				LOG("[NFGE::Graphic::Model::SaveModel()] Model data currupted, numOfMesh not equal numOfMaterialIndex! <Save Model Cancel!>");
				return false;
			}

			for (int i = 0; i < numOfMeshes; ++i)
			{
				ret = NFGE::Graphics::MeshIO::Save(model.mMeshData[i].mesh, file, NFGE::Core::IOOption::RV);
				fprintf(file, "Material Index: %d\n", model.mMeshData[i].materialIndex);
			}

			// Save Materials data
			fprintf(file, "Number Of Materials: %d\n", numOfMaterials);
			for (int i = 0; i < numOfMaterials; ++i)
			{
				fprintf(file, "Material %d: \nContain Texture Types Mask: %d\n", i, model.mContext.modelMaterials[i].mAvliableTextureType);
				auto& currentMaterial = model.mContext.modelMaterials[i];
				uint32_t checker = 0;
				for (int j = 0; j < 9; ++j)
				{
					auto& currentTextureHolder = currentMaterial.mTextureHolders[j];
					fprintf(file, "%s Texture Count: %d\n", NFGE::Graphics::MeshTextureMaterial::ModelTextureTypeName[j], currentTextureHolder.mNumOfTextures);
					if (currentTextureHolder.mNumOfTextures > 0)
						checker += 0x1 << j;

					for (int k = 0; k < currentTextureHolder.mNumOfTextures; ++k)
					{
						fprintf(file, "texture: %s\n", currentTextureHolder.mFilenames[k].c_str());
					}
				}
				ASSERT(checker == model.mContext.modelMaterials[i].mAvliableTextureType, "[NFGE::Model::SaveModel()] Mask not match!");
			}
			fprintf(file, "EOF\n");

			return true;
		}

		template<class T>
		bool LoadBianry(T& model, FILE * file)
		{
			return false;
		}

		template<class T>
		bool LoadRV(T& model, FILE * file) // RV -> Readable Value;
		{
			// Load mesh data
			int numOfMeshes;
			LOAD_SAFTY_CHECK(fscanf(file, "Number Of Meshes: %d\n", &numOfMeshes));

			model.mMeshData.reserve(numOfMeshes);

			for (int i = 0; i < numOfMeshes; ++i)
			{
				model.mMeshData.emplace_back();
				NFGE::Graphics::MeshIO::Load(model.mMeshData.back().mesh, file, NFGE::Core::IOOption::RV);
				LOAD_SAFTY_CHECK(fscanf(file, "Material Index: %d\n", &(model.mMeshData.back().materialIndex)));
			}

			// Load material data
			int numOfMaterials;
			LOAD_SAFTY_CHECK(fscanf(file, "Number Of Materials: %d\n", &numOfMaterials));

			for (int i = 0; i < numOfMaterials; ++i)
			{
				model.mContext.modelMaterials.emplace_back();
				auto& currentMaterial = model.mContext.modelMaterials.back();
				int currentMaterialIndex;
				LOAD_SAFTY_CHECK(fscanf(file, "Material %d: \nContain Texture Types Mask: %d\n", &currentMaterialIndex, &(currentMaterial.mAvliableTextureType)));
				uint32_t checker = 0;
				for (int j = 0; j < 9; ++j)
				{
					auto& currentTextureHolder = currentMaterial.mTextureHolders[j];
					char buffer[256];
					LOAD_SAFTY_CHECK(fscanf(file, "%s Texture Count: %d\n", buffer, &(currentTextureHolder.mNumOfTextures)));
					bool cmpResult = std::strcmp(NFGE::Graphics::MeshTextureMaterial::ModelTextureTypeName[j], buffer) != 0;
					ASSERT(!cmpResult, "[NFGE::Graphics::Model::LoadRV()] --- Error: File currupted! Texture store order doesn't match with NFGE Model store stander.");

					if (currentTextureHolder.mNumOfTextures > 0)
						checker += 0x1 << j;

					for (int k = 0; k < currentTextureHolder.mNumOfTextures; ++k)
					{
						char buffer[256];
						LOAD_SAFTY_CHECK(fscanf(file, "texture: %[^\n]", buffer));
						currentTextureHolder.mFilenames.emplace_back(buffer);
					}
				}

				ASSERT(checker == model.mContext.modelMaterials[i].mAvliableTextureType, "[NFGE::Model::LoadRV()] Error: File currupted! <Material contain texture Mask> not match with contains textures!");
			}

			return true;
		}
	}
}

#endif // !NFGE_GRAPHICS_ANIMATIONSETIO_H
