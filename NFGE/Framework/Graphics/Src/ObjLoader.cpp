#include "Precompiled.h"
#include "ObjLoader.h"

using namespace NFGE::Graphics;

void ObjLoader::Load(const std::filesystem::path& filePath, float scale, Mesh& mesh)
{
	std::vector<uint32_t> positionIndices, normalIndices, uvIndices;
	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector3> normals;
	std::vector<Math::Vector2> uvs;

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	ASSERT(file, "[ModelLoader] Failed to open file %s", filePath.u8string().c_str());

	while (true)
	{
		char buffer[128];
		int res = fscanf_s(file, "%s", buffer, std::size(buffer));
		if (res == EOF)
			break;

		if (strcmp(buffer, "v") == 0)
		{
			Math::Vector3 position;
			fscanf_s(file, "%f %f %f", &position.x, &position.y, &position.z);
			positions.push_back(position * scale);
		}
		else if (strcmp(buffer, "vt") == 0)
		{
			Math::Vector2 uv;
			fscanf_s(file, "%f %f", &uv.x, &uv.y);
			uvs.push_back({ uv.x, 1.0f - uv.y });
		}
		else if (strcmp(buffer, "vn") == 0)
		{
			Math::Vector3 normal;
			fscanf_s(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(buffer, "f") == 0)
		{
			uint32_t v[4], t[4], n[4];
			fgets(buffer, std::size(buffer), file);
			if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1], 
				&v[2], &t[2], &n[2], 
				&v[3], &t[3], &n[3]) == 12)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);

				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[2]);
				positionIndices.push_back(v[3]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[2]);
				uvIndices.push_back(t[3]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[2]);
				normalIndices.push_back(n[3]);
			}
			else if(sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2]) == 9)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);
			}
			else
			{
				ASSERT(false, "[ObjLoader] Unsipported OBJ format. File: %s", filePath.u8string().c_str());
			}
		}
	}
	fclose(file);

	// Reconstruct vertices
	std::vector<Vertex> vertices;
	vertices.reserve(positionIndices.size());
	for (size_t i = 0; i < positionIndices.size(); ++i)
	{
		Vertex vertex;
		vertex.position = positions[positionIndices[i] - 1];
		vertex.normal = normals[normalIndices[i] - 1];
		vertex.texcoord = uvs[uvIndices[i] - 1];
		vertices.emplace_back(vertex);
	}
	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);	//fill 0, 1, 2, 3, ....

	//Initialize mesh
	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);
}