#include "Precompiled.h"
#include "Mesh.h"
#include "MeshIO.h"
#include <Network/Inc/MemoryStream.h>
#include <Network/Inc/StreamWriter.h>
#include <Network/Inc/StreamReader.h>

using namespace NFGE::Graphics;

bool SaveBianry(const BoneMesh& mesh, FILE * file);
bool SaveRV(const BoneMesh& mesh, FILE * file); // RV -> Readable Value;
bool LoadBianry(BoneMesh& mesh, FILE * file);
bool LoadRV(BoneMesh& mesh, FILE * file); // RV -> Readable Value;

bool NFGE::Graphics::MeshIO::Save(const BoneMesh& mesh, FILE * file, NFGE::Core::IOOption option)
{
	switch (option)
	{
	case NFGE::Core::IOOption::Binary:
		return SaveBianry(mesh, file);
		break;
	case NFGE::Core::IOOption::RV:
		return SaveRV(mesh, file);
		break;
	default:
		return SaveBianry(mesh, file);
		break;
	}
}

bool NFGE::Graphics::MeshIO::Load(BoneMesh& mesh, FILE * file, NFGE::Core::IOOption option)
{
	switch (option)
	{
	case NFGE::Core::IOOption::Binary:
		return LoadBianry(mesh, file);
		break;
	case NFGE::Core::IOOption::RV:
		return LoadRV(mesh, file);
		break;
	default:
		return LoadBianry(mesh, file);
		break;
	}
}

bool SaveBianry(const BoneMesh& mesh, FILE * file)
{
	uint32_t totalVertexes = static_cast<uint32_t>(mesh.vertices.size());
	uint32_t totalIndices = static_cast<uint32_t>(mesh.indices.size());

	uint32_t totalBytes = totalVertexes * sizeof(Vertex) + sizeof(uint32_t) * (3 + totalIndices);

	Network::MemoryStream ms(totalBytes);
	Network::StreamWriter sw(ms);

	// Write TotalBytesCount
	sw.Write<uint32_t>(totalBytes);

	// Write Vertexes
	sw.Write<uint32_t>(totalVertexes);
	for (size_t i = 0; i < totalVertexes; i++)
	{
		// Write position
		sw.Write<float>(mesh.vertices[i].position.x);
		sw.Write<float>(mesh.vertices[i].position.y);
		sw.Write<float>(mesh.vertices[i].position.z);
		// Write normal
		sw.Write<float>(mesh.vertices[i].normal.x);
		sw.Write<float>(mesh.vertices[i].normal.y);
		sw.Write<float>(mesh.vertices[i].normal.z);
		// Write tangent
		sw.Write<float>(mesh.vertices[i].tangent.x);
		sw.Write<float>(mesh.vertices[i].tangent.y);
		sw.Write<float>(mesh.vertices[i].tangent.z);
		// Write texcood
		sw.Write<float>(mesh.vertices[i].texcoord.x);
		sw.Write<float>(mesh.vertices[i].texcoord.y);
	}

	sw.Write<uint32_t>(totalIndices);
	// Write indices
	for (size_t i = 0; i < totalIndices; i++)
	{
		sw.Write<uint32_t>(mesh.indices[i]);
	}
	uint32_t savedElement = static_cast<uint32_t>(fwrite(ms.GetData(), ms.GetSize(), 1, file));

	return savedElement != 0;
}

bool SaveRV(const BoneMesh& mesh, FILE * file) // RV -> Space Separate Value
{
	int printResult = 0;

	int totalVertexes = static_cast<int>(mesh.vertices.size());
	int totalIndices = static_cast<int>(mesh.indices.size());

	printResult = fprintf(file, "Total_Vertices: %d Total_Indices: %d\n", totalVertexes, totalIndices);
	if (printResult < 0)
		return false;

	for (int i = 0; i < totalVertexes; i++)
	{
		printResult = fprintf(file, "Position: %f %f %f Normal: %f %f %f Tangent: %f %f %f Texcoord: %f %f BoneIndices: %d %d %d %d BoneWeight: %f %f %f %f\n",
			mesh.vertices[i].position.x, mesh.vertices[i].position.y, mesh.vertices[i].position.z,
			mesh.vertices[i].normal.x, mesh.vertices[i].normal.y, mesh.vertices[i].normal.z,
			mesh.vertices[i].tangent.x, mesh.vertices[i].tangent.y, mesh.vertices[i].tangent.z,
			mesh.vertices[i].texcoord.x, mesh.vertices[i].texcoord.y, 
			mesh.vertices[i].boneIndices[0], mesh.vertices[i].boneIndices[1], mesh.vertices[i].boneIndices[2], mesh.vertices[i].boneIndices[3],
			mesh.vertices[i].boneWeights[0], mesh.vertices[i].boneWeights[1], mesh.vertices[i].boneWeights[2], mesh.vertices[i].boneWeights[3]
		);

		if (printResult < 0)
			return false;
	}

	printResult = fprintf(file, "Indices: \n");
	if (printResult < 0)
		return false;

	for (int i = 0; i < totalIndices; i++)
	{
		printResult = fprintf(file, "%d ", mesh.indices[i]);
		if (printResult < 0)
			return false;
	}
	printResult = fprintf(file, "\n");
	if (printResult < 0)
		return false;

	return true;
}

bool LoadBianry(BoneMesh& mesh, FILE * file)
{
	int ret = 0;
	uint32_t totalBytes;
	unsigned char buffer[sizeof(uint32_t)];
	ret = static_cast<int>(fread(buffer, sizeof(uint32_t), 1, file));
	if (ret == 0)
		return true;

	memcpy(&totalBytes, buffer, sizeof(uint32_t));

	uint8_t *contentBuffer = (uint8_t*)malloc(totalBytes - 1 * sizeof(uint32_t));	//|--- Load the rest of data
	fread(contentBuffer, totalBytes - 1 * sizeof(uint32_t), 1, file);			//|

	Network::MemoryStream ms(contentBuffer, totalBytes - 1 * sizeof(uint32_t)); //Used up 1 bite already for [totalBytes]
	Network::StreamReader sr(ms);

	uint32_t totalVertexes;
	sr.Read<uint32_t>(totalVertexes);
	mesh.vertices.clear();
	mesh.vertices.reserve(totalVertexes);

	for (size_t i = 0; i < totalVertexes; i++)
	{
		BoneVertex vertex;
		sr.Read<float>(vertex.position.x);
		sr.Read<float>(vertex.position.y);
		sr.Read<float>(vertex.position.z);
		sr.Read<float>(vertex.normal.x);
		sr.Read<float>(vertex.normal.y);
		sr.Read<float>(vertex.normal.z);
		sr.Read<float>(vertex.tangent.x);
		sr.Read<float>(vertex.tangent.y);
		sr.Read<float>(vertex.tangent.z);
		sr.Read<float>(vertex.texcoord.x);
		sr.Read<float>(vertex.texcoord.y);
		mesh.vertices.push_back(vertex);
	}

	uint32_t totalIndices;
	sr.Read<uint32_t>(totalIndices);
	mesh.indices.clear();
	mesh.indices.reserve(totalIndices);

	for (size_t i = 0; i < totalIndices; i++)
	{
		uint32_t index;
		sr.Read<uint32_t>(index);
		mesh.indices.push_back(index);
	}

	return false;
}

bool LoadRV(BoneMesh& mesh, FILE * file) // RV -> Space Separate Value;
{
	int readResult = 0;

	int totalVertexes = 0;
	int totalIndices = 0;

	readResult = fscanf(file, "Total_Vertices: %d Total_Indices: %d\n", &totalVertexes, &totalIndices);
	if (readResult == EOF)
		return true;//

	mesh.vertices.clear();
	mesh.vertices.reserve(totalVertexes);
	for (int i = 0; i < totalVertexes; i++)
	{
		mesh.vertices.emplace_back();
		auto& currentVertex = mesh.vertices.back();
		fscanf(file, "Position: %f %f %f Normal: %f %f %f Tangent: %f %f %f Texcoord: %f %f BoneIndices: %d %d %d %d BoneWeight: %f %f %f %f\n",
			&currentVertex.position.x,	&currentVertex.position.y,	&currentVertex.position.z,
			&currentVertex.normal.x,	&currentVertex.normal.y,	&currentVertex.normal.z,
			&currentVertex.tangent.x,	&currentVertex.tangent.y,	&currentVertex.tangent.z,
			&currentVertex.texcoord.x,	&currentVertex.texcoord.y,
			&currentVertex.boneIndices[0], &currentVertex.boneIndices[1], &currentVertex.boneIndices[2], &currentVertex.boneIndices[3],
			&currentVertex.boneWeights[0], &currentVertex.boneWeights[1], &currentVertex.boneWeights[2], &currentVertex.boneWeights[3]

		);
	}

	mesh.indices.clear();
	mesh.indices.reserve(totalIndices);

	fscanf(file, "Indices: \n");
	for (int i = 0; i < totalIndices; i++)
	{
		uint32_t index;
		fscanf(file, "%d ", &index);
		mesh.indices.push_back(index);
	}
	fscanf(file, "\n");

	return false;
}