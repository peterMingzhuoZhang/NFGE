#ifndef INCLUDE_NFGE_GRAPHICS_GEOMETRYMESHMANAGER_H
#define INCLUDE_NFGE_GRAPHICS_GEOMETRYMESHMANAGER_H

#include "GeometryMesh.h"

namespace NFGE::Graphics
{
	struct DirectionalLight;
	enum class GeometryType
	{
		Plane,
		Sphere,
		Cone,
	};

	class GeometryMeshManager
	{
	public:
		static void StaticInitialize(DirectionalLight*);
		static void StaticTerminate();
		static GeometryMeshManager* Get();

	public:
		void Initialize(DirectionalLight*);
		void Terminate();

		template <class GeometryMeshType>
		bool AddGeometryMesh(std::string name, DirectionalLight* light)
		{
			auto[iter, success] = mInventory.try_emplace(name, nullptr);
			if (success)
			{
				auto effect = std::make_unique<GeometryMeshType>();
				effect->DefaultInitialize(light);
				iter->second = std::move(effect);
			}
			return success;
		}

		bool AddGeometryMesh(GeometryType effects, DirectionalLight* light);

		
		GeometryMesh* GetGeometryMesh(std::string name);
		GeometryMesh* GetGeometryMesh(GeometryType geometryType);
	private:
		std::unordered_map<std::string, std::unique_ptr<GeometryMesh>> mInventory;

	};
}

#endif // !INCLUDE_NFGE_GRAPHICS_GEOMETRYMESHMANAGER_H
