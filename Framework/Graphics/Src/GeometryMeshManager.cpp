#include "Precompiled.h"
#include "GeometryMeshManager.h"
#include "BuildInGeometryMesh.h"

using namespace NFGE::Graphics;

namespace
{
	std::unique_ptr<GeometryMeshManager> sInstance = nullptr;
}


void NFGE::Graphics::GeometryMeshManager::StaticInitialize(DirectionalLight * light)
{
	ASSERT(sInstance == nullptr, "[GeometryManager] System already initlizlized!");
	sInstance = std::make_unique<GeometryMeshManager>();
	sInstance->Initialize(light);
}

void NFGE::Graphics::GeometryMeshManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

GeometryMeshManager * NFGE::Graphics::GeometryMeshManager::Get()
{
	ASSERT(sInstance != nullptr, "[GeometryManager] System already initlizlized!");
	return sInstance.get();
}

void NFGE::Graphics::GeometryMeshManager::Initialize(DirectionalLight *light)
{
	AddGeometryMesh<Geometry::PlaneMesh>("Plane", light);
	AddGeometryMesh<Geometry::SphereMesh>("Sphere", light);
	AddGeometryMesh<Geometry::ConeMesh>("Cone", light);
}

void NFGE::Graphics::GeometryMeshManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->UnLoad();
}

bool NFGE::Graphics::GeometryMeshManager::AddGeometryMesh(GeometryType geometryMesh, DirectionalLight * light)
{
	bool ret = false;

	switch (geometryMesh)
	{
	case NFGE::Graphics::GeometryType::Plane:
		ret = AddGeometryMesh<Geometry::PlaneMesh>("Plane", light);
		break;
	case NFGE::Graphics::GeometryType::Sphere:
		ret = AddGeometryMesh<Geometry::SphereMesh>("Sphere", light);
		break;
	case NFGE::Graphics::GeometryType::Cone:
		ret = AddGeometryMesh<Geometry::ConeMesh>("Cone", light);
		break;
	default:
		break;
	}

	return ret;
}

GeometryMesh * NFGE::Graphics::GeometryMeshManager::GetGeometryMesh(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

GeometryMesh * NFGE::Graphics::GeometryMeshManager::GetGeometryMesh(GeometryType geometryMesh)
{
	GeometryMesh* ret = nullptr;

	switch (geometryMesh)
	{
	case NFGE::Graphics::GeometryType::Plane:
		ret = GetGeometryMesh("Plane");
		break;
	case NFGE::Graphics::GeometryType::Sphere:
		ret = GetGeometryMesh("Sphere");
		break;
	case NFGE::Graphics::GeometryType::Cone:
		ret = GetGeometryMesh("Cone");
		break;
	default:
		break;
	}

	return ret;
}
