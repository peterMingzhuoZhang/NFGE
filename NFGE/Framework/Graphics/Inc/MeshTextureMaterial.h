#ifndef NFGE_GRAPHICS_MESHTEXTUREMATERIAL_H
#define NFGE_GRAPHICS_MESHTEXTUREMATERIAL_H

#include "TextureManager.h"

namespace NFGE::Graphics
{
	struct MeshTextureMaterial
	{
		static const uint32_t totalTypeCount = 18;
		static const uint32_t TextureType_DIFFUSE = 0x1 << 0;
		static const uint32_t TextureType_SPECULAR = 0x1 << 1;
		static const uint32_t TextureType_AMBIENT = 0x1 << 2;
		static const uint32_t TextureType_EMISSIVE = 0x1 << 3;
		static const uint32_t TextureType_HEIGHT = 0x1 << 4;
		static const uint32_t TextureType_NORMALS = 0x1 << 5;
		static const uint32_t TextureType_SHININESS = 0x1 << 6;
		static const uint32_t TextureType_OPACITY = 0x1 << 7;
		static const uint32_t TextureType_DISPLACEMENT = 0x1 << 8;
		static const uint32_t TextureType_LIGHTMAP = 0x1 << 9;
		static const uint32_t TextureType_REFLECTION = 0x1 << 10;
		static const uint32_t TextureType_BASE_COLOR = 0x1 << 11;
		static const uint32_t TextureType_NORMAL_CAMERA = 0x1 << 12;
		static const uint32_t TextureType_EMISSION_COLOR = 0x1 << 13;
		static const uint32_t TextureType_METALNESS = 0x1 << 14;
		static const uint32_t TextureType_DIFFUSE_ROUGHNESS = 0x1 << 15;
		static const uint32_t TextureType_AMBIENT_OCCLUSION = 0x1 << 16;
		static const uint32_t TextureType_UNKNOWN = 0x1 << 17;

		static const std::array<const char*, totalTypeCount> ModelTextureTypeName;
		
		enum ModelTextureType
		{
			DIFFUSE = 0,
			SPECULAR,
			AMBIENT,
			EMISSIVE,
			HEIGHT,
			NORMALS,
			SHININESS,
			OPACITY,
			DISPLACEMENT,
			LIGHTMAP,
			REFLECTION,
			BASE_COLOR,
			NORMAL_CAMERA,
			EMISSION_COLOR,
			MATALNESS,
			DIFFUSE_ROUGHNESS,
			AMBIENT_OCCLUSION,
			UNKNOWN
		};

		struct MeshTextureHolder
		{
			std::vector<std::string> mFilenames;
			std::vector<TextureId> mTextureIds;
			int mNumOfTextures;
			std::vector<int> mEachMeshCurrentUseTexture;
		};

		uint32_t mAvliableTextureType;							//--- It is the mask to determine which kind of texture it has
		std::array<MeshTextureHolder, 18> mTextureHolders;		//--- Store 18 types of texture holders

		void LoadTexture(ModelTextureType type, TextureId textureId)
		{
			switch (type)
			{
			case NFGE::Graphics::MeshTextureMaterial::DIFFUSE:
				mAvliableTextureType |= TextureType_DIFFUSE;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::SPECULAR:
				mAvliableTextureType |= TextureType_SPECULAR;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::AMBIENT:
				mAvliableTextureType |= TextureType_AMBIENT;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::EMISSIVE:
				mAvliableTextureType |= TextureType_EMISSIVE;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::HEIGHT:
				mAvliableTextureType |= TextureType_HEIGHT;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::NORMALS:
				mAvliableTextureType |= TextureType_NORMALS;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::SHININESS:
				mAvliableTextureType |= TextureType_SHININESS;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::OPACITY:
				mAvliableTextureType |= TextureType_OPACITY;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::DISPLACEMENT:
				mAvliableTextureType |= TextureType_DISPLACEMENT;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::LIGHTMAP:
				mAvliableTextureType |= TextureType_LIGHTMAP;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::REFLECTION:
				mAvliableTextureType |= TextureType_REFLECTION;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::BASE_COLOR:
				mAvliableTextureType |= TextureType_BASE_COLOR;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::NORMAL_CAMERA:
				mAvliableTextureType |= TextureType_NORMAL_CAMERA;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::EMISSION_COLOR:
				mAvliableTextureType |= TextureType_EMISSION_COLOR;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::MATALNESS:
				mAvliableTextureType |= TextureType_METALNESS;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::DIFFUSE_ROUGHNESS:
				mAvliableTextureType |= TextureType_DIFFUSE_ROUGHNESS;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::AMBIENT_OCCLUSION:
				mAvliableTextureType |= TextureType_AMBIENT_OCCLUSION;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			case NFGE::Graphics::MeshTextureMaterial::UNKNOWN:
				mAvliableTextureType |= TextureType_UNKNOWN;
				mTextureHolders[type].mTextureIds.push_back(textureId);
				break;
			default:
				break;
			}
		}

		TextureId GetTexture(ModelTextureType type, int index = 0)
		{
			if (mTextureHolders[type].mTextureIds.empty())
				return 0;
			return mTextureHolders[type].mTextureIds[index];
		}
	};
}

#endif // !NFGE_GRAPHICS_MESHTEXTUREMATERIAL_H
