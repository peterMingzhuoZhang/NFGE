#ifndef NFGE_GRAPHIC_MODEL_H
#define NFGE_GRAPHIC_MODEL_H

#include "Mesh.h"
#include "Effect.h"
#include "MeshBuffer.h"
#include "Light.h"

namespace NFGE::Graphics
{
	class Model
	{
	public:
		//Peter chan suggest
		struct MeshData
		{
			BoneMesh mesh;
			uint32_t materialIndex;
			MeshBuffer meshBuffer;
		};
	public:
		bool isRenderWithEffect = false;;

		std::vector<MeshData> mMeshData;
		
		Effects::ModelMeshEffect::ModelMeshEffectContext mContext;					// New added feature for organization purpose. 2020-02-25
	public:
		void Load(const char* modelFileName, DirectionalLight* directionLight = nullptr);
		void Load(const std::filesystem::path& fileName, DirectionalLight* directionLight = nullptr);
		void UnLoad();
		void Update(float deltaTime);
		void OldRender(const NFGE::Graphics::Camera& camera);
		void Render(const NFGE::Graphics::Camera& camera);
		void DebugUI();
		
		bool SaveToFile(FILE* file, NFGE::Core::IOOption option = NFGE::Core::IOOption::Binary) const;
	private:
		bool LoadFromFile(FILE* file, NFGE::Core::IOOption option = NFGE::Core::IOOption::Binary);

	};
}

#endif // !NFGE_GRAPHIC_MODEL_H
