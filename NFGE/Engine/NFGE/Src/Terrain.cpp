#include "Precompiled.h"
#include "Terrain.h"

#include <ImGui/Inc/imgui.h>

using namespace NFGE;

void Terrain::Initialize(uint32_t numRows, uint32_t numCols, float scale)
{
	mNumRows = numRows;
	mNumCols = numCols;
	mNumCellsInCol = mNumRows - 1;
	mNumCellsInRow = mNumCols - 1;
	mNumCells = mNumCellsInCol * mNumCellsInRow;
	mScale = scale;

	mConstantBuffer.Initialize();
	mTerrainVertexShader.Initialize(L"../../Assets/Shaders/Terrain.fx", Graphics::Vertex::Format);
	mTerrainPixelShader.Initialize(L"../../Assets/Shaders/Terrain.fx");

	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mGrassTexture.Initialize("../../Assets/Images/grass_2048.jpg");

	mMesh.Allocate(numRows * numCols, mNumCells * 6);
	GenerateIndices();
	GenerateVertices();

	mMeshBuffer.Initialize(mMesh, true);

	mQuadTree.Initialize(&mMesh, 5);
}

void Terrain::Terminate()
{
	mMeshBuffer.Terminate();
	mGrassTexture.Terminate();
	mSampler.Terminate();

	mTerrainPixelShader.Terminate();
	mTerrainVertexShader.Terminate();
	mConstantBuffer.Terminate();
}

bool Terrain::Intersect(NFGE::Math::Ray ray, float& distance) const
{
	return mQuadTree.Intersect(ray, distance);
}

void Terrain::Render(const Graphics::Camera& camera)
{
	static bool showQuadTree = false;
	if (showQuadTree)
	{
		QuadTree::Visitor visitor = [](QuadTree::Node* node) {
			node->DrawNode();
			return true;
		};
		mQuadTree.Visit(visitor);
	}

	auto world = Math::Matrix4::sIdentity();
	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();

	ConstantData data;
	data.world = Math::Transpose(world);
	data.wvp = Math::Transpose(world * view * projection);
	data.viewPosition = { camera.GetPosition(), 1.0f };
	data.directionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	data.directionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	data.directionalLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	data.directionalLight.specular = { 0.1f, 0.1f, 0.1f, 1.0f };
	//data.directionalLight.power = 10.0f;
	mConstantBuffer.Load(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mSampler.BindPS(0);
	mGrassTexture.BindPS(0);

	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();

	mMeshBuffer.Render();
}

void Terrain::DrawEditorUI()
{
	if (ImGui::CollapsingHeader("Terrain Editor"))
	{
		if (ImGui::SliderFloat("Frequency 0", &mFrequency0, 0.0f, 100.0f) ||
			ImGui::SliderFloat("Frequency 1", &mFrequency1, 0.0f, 100.0f) ||
			ImGui::SliderFloat("Weight 0", &mWeight0, 0.0f, 50.0f) ||
			ImGui::SliderFloat("Weight 1", &mWeight1, 0.0f, 50.0f))
		{
			GenerateVertices();
			mMeshBuffer.Update(mMesh.GetVertices().data(), mMesh.GetVertexCount());
		}
	}
}

void Terrain::GenerateVertices()
{
	// Generate mesh for the terrain
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	//std::unique_ptr<float[]> heightValues = std::make_unique<float[]>(numRows * numCols);
	//
	//FILE* file = nullptr;
	//fopen_s(&file, "../Assets/Heightmaps/heightmap_200x200.raw", "rb");
	//for (uint32_t i = 0; i < numRows * numCols; ++i)
	//{
	//	heightValues[i] = fgetc(file) / 255.0f * 10.0f;
	//}
	//fclose(file);

	Math::PerlinNoise noise;
	for (uint32_t z = 0; z < mNumRows; ++z)
	{
		for (uint32_t x = 0; x < mNumCols; ++x)
		{
			uint32_t index = x + (z * mNumCols);
			auto& vertex = mMesh.GetVertex(index);
	
			float normX = (float)x / mNumCols;
			float normZ = (float)z / mNumRows;
			float noise0 = mWeight0 * noise.Get(mFrequency0 * normX, 0.0f, mFrequency0 * normZ);
			float noise1 = mWeight1 * noise.Get(mFrequency1 * normX, 0.0f, mFrequency1 * normZ);
	
			vertex.position =
			{
				(float)x * mScale,
				//heightValues[index],
				vertex.position.y = (noise0 + noise1),
				(float)z * mScale
			};
			vertex.normal = { 0.0f, 1.0f, 0.0f };
			vertex.tangent = { 1.0f, 0.0f, 0.0f };
			vertex.texcoord.x = normX;
			vertex.texcoord.y = normZ;
		}
	}

	Graphics::MeshBuilder::ComputeNormals(mMesh);
}

void Terrain::GenerateIndices()
{
	uint32_t i = 0;
	for (uint32_t z = 0; z < mNumCellsInCol; ++z)
	{
		for (uint32_t x = 0; x < mNumCellsInRow; ++x)
		{
			uint32_t lb = x + (z * mNumCols);
			uint32_t lt = x + ((z + 1) * mNumCols);
			uint32_t rb = (x + 1) + (z * mNumCols);
			uint32_t rt = (x + 1) + ((z + 1) * mNumCols);

			//mMesh.GetIndex(i++) = lb;
			//mMesh.GetIndex(i++) = lt;
			//mMesh.GetIndex(i++) = rb;
			//
			//mMesh.GetIndex(i++) = lt;
			//mMesh.GetIndex(i++) = rt;
			//mMesh.GetIndex(i++) = rb;
		}
	}
}