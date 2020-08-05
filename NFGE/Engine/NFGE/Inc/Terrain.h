#ifndef INCLUDED_NFGE_TERRAIN_H
#define INCLUDED_NFGE_TERRAIN_H

#include "QuadTree.h"

namespace NFGE {

class Terrain
{
public:
	Terrain() = default;

	void Initialize(uint32_t numRows, uint32_t numCols, float scale);
	void Terminate();

	bool Intersect(NFGE::Math::Ray ray, float& distance) const;

	void Render(const Graphics::Camera& camera);

	void DrawEditorUI();

private:
	void GenerateVertices();
	void GenerateIndices();

	struct ConstantData
	{
		NFGE::Math::Matrix4 world;
		NFGE::Math::Matrix4 wvp;
		NFGE::Math::Vector4 viewPosition;
		NFGE::Graphics::DirectionalLight directionalLight;
	};

	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::VertexShader mTerrainVertexShader;
	Graphics::PixelShader mTerrainPixelShader;

	Graphics::Sampler mSampler;

	Graphics::Texture mGrassTexture;

	Graphics::Mesh mMesh;
	Graphics::MeshBuffer mMeshBuffer;

	QuadTree mQuadTree;

	uint32_t mNumRows{ 0 };
	uint32_t mNumCols{ 0 };
	uint32_t mNumCellsInCol{ 0 };
	uint32_t mNumCellsInRow{ 0 };
	uint32_t mNumCells{ 0 };

	float mScale{ 1.0f };
	float mFrequency0{ 5.0f };
	float mFrequency1{ 40.0f };
	float mWeight0{ 25.0f };
	float mWeight1{ 1.0f };
};

} // namespace NFGE

#endif // #ifndef INCLUDED_GAMENFGE_TERRAIN_H