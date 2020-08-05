#include "Precompiled.h"
#include "TerrainService.h"

#include "CameraService.h"
#include "World.h"

using namespace NFGE;

META_DERIVED_BEGIN(TerrainService, Service)
META_FIELD_BEGIN
META_FIELD(mNumRows, "NumRows")
META_FIELD(mNumCols, "NumCols")
META_FIELD(mCellSize, "CellSize")
META_FIELD_END
META_CLASS_END;


void TerrainService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();

	mTerrain.Initialize(mNumRows, mNumCols, mCellSize);
}

void TerrainService::Terminate()
{
	mTerrain.Terminate();
}

void TerrainService::Render()
{
	auto camera = mCameraService->GetActiveCamera();
	mTerrain.Render(camera);
}

void TerrainService::DebugUI()
{
	mTerrain.DrawEditorUI();
}

