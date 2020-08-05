#ifndef INCLUDE_NFGE_TERRAINSERVICE_H
#define INCLUDE_NFGE_TERRAINSERVICE_H

#include "Service.h"
#include "Terrain.h"

namespace NFGE
{
	class CameraService;

	class TerrainService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Terminate() override;

		void Render() override;
		void DebugUI() override;


	private:
		CameraService * mCameraService = nullptr;

		Terrain mTerrain;
		int mNumRows = 200;
		int mNumCols = 200;
		float mCellSize = 1.0f;
	};
}

#endif // !INCLUDE_NFGE_TERRAINSERVICE_H
