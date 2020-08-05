#pragma once
#include "Mesh.h"

namespace NFGE::Graphics
{
	class ObjLoader
	{
	public:
		static void Load(const std::filesystem::path& filePath, float scale, Mesh& mesh);
	};
}