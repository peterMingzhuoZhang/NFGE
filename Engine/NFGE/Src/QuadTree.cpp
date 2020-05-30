#include "Precompiled.h"
#include "QuadTree.h"

using namespace NFGE;
namespace
{
}

void NFGE::QuadTree::Initialize(const Graphics::Mesh * mesh, uint32_t levels)
{
	mMesh = mesh;

	// Get the bound of the mesh. 
	Math::AABB bound = Graphics::MeshBuilder::ComputeBound(*mesh);
	//bound.center.y = 0.0f;
	//bound.extend.y = 50.0f;

	// Generate nodes up to number of levels.
	GenerateNodeRecursive(mRoot, bound, levels);

	// Assign triangles
	auto& vertices = mesh->vertices;
	auto& indices = mesh->indices;
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		auto& a = vertices[indices[i + 0]].position;
		auto& b = vertices[indices[i + 1]].position;
		auto& c = vertices[indices[i + 2]].position;

		const float minX = Math::Min(Math::Min(a.x, b.x), c.x);
		const float maxX = Math::Max(Math::Max(a.x, b.x), c.x);
		const float minY = Math::Min(Math::Min(a.y, b.y), c.y);
		const float maxY = Math::Max(Math::Max(a.y, b.y), c.y);
		const float minZ = Math::Min(Math::Min(a.z, b.z), c.z);
		const float maxZ = Math::Max(Math::Max(a.z, b.z), c.z);

		const Math::AABB aabb = Math::AABB::FromMinMax({ minX,minY,minZ }, { maxX, maxY, maxZ });

		Visitor visitor = [a, b, c, aabb, i](Node* node)
		{
			if (!Math::Intersect(aabb, node->aabb))
				return false;
			if (!node->isLeaf)
				return true;


			if (Math::Intersect(a, node->aabb) || 
				Math::Intersect(b, node->aabb) ||
				Math::Intersect(c, node->aabb) ||
				Math::Intersect((a + b)* 0.5f, node->aabb) ||
				Math::Intersect((a + c)* 0.5f, node->aabb) ||
				Math::Intersect((b + c)* 0.5f, node->aabb))
			{
				node->indces.push_back(i);
			}
			return false;
		};
		Visit(visitor);
	}
}

void NFGE::QuadTree::Visit(Visitor & visitor)
{
	VisitRecursive(mRoot.get(), visitor);
}

bool NFGE::QuadTree::Intersect(const NFGE::Math::Ray & ray, float & distance) const
{
	if (mRoot == nullptr)
		return false;

	distance = std::numeric_limits<float>::max();
	return IntersectRecursive(mRoot.get(), ray, distance);
}

void NFGE::QuadTree::GenerateNodeRecursive(std::unique_ptr<Node>& node, const Math::AABB & aabb, uint32_t levels)
{
	node = std::make_unique<Node>();
	node->aabb = aabb;

	if (--levels == 0)
	{
		node->isLeaf = true;
	}
	else
	{
		const Math::Vector3& center = aabb.center;
		const Math::Vector3& half = { aabb.extend.x * 0.5f, aabb.extend.y, aabb.extend.z * 0.5f };
		Math::AABB bl = { {center.x - half.x, center.y , center.z - half.z}, half };
		Math::AABB br = { {center.x + half.x, center.y , center.z - half.z}, half };
		Math::AABB tl = { {center.x - half.x, center.y , center.z + half.z}, half };
		Math::AABB tr = { {center.x + half.x, center.y , center.z + half.z}, half };

		GenerateNodeRecursive(node->children[BL], bl, levels);
		GenerateNodeRecursive(node->children[BR], br, levels);
		GenerateNodeRecursive(node->children[TL], tl, levels);
		GenerateNodeRecursive(node->children[TR], tr, levels);
	}
}

void NFGE::QuadTree::VisitRecursive(Node * node, Visitor & visitor)
{
	if (visitor(node) && !node->isLeaf)
	{
		for (auto& child : node->children)
			VisitRecursive(child.get(), visitor);
	}
}

bool NFGE::QuadTree::IntersectRecursive(const Node * node, const NFGE::Math::Ray & ray, float & distance) const
{
	float distEntry, distExit;
	if (!Math::Intersect(ray, node->aabb, distEntry, distExit))
		return false;

	if (!node->isLeaf)
	{
		return
			IntersectRecursive(node->children[0].get(), ray, distance) ||
			IntersectRecursive(node->children[1].get(), ray, distance) ||
			IntersectRecursive(node->children[2].get(), ray, distance) ||
			IntersectRecursive(node->children[3].get(), ray, distance);
	}

	bool intersect = false;
	for (size_t i = 0; i < node->indces.size(); ++i)
	{
		uint32_t faceIndex = node->indces[i];
		auto& a = mMesh->vertices[mMesh->indices[faceIndex + 0]];
		auto& b = mMesh->vertices[mMesh->indices[faceIndex + 1]];
		auto& c = mMesh->vertices[mMesh->indices[faceIndex + 2]];

		float d = 0.0f;
		if (Math::Intersect(ray, a.position, b.position, c.position, d))
		{
			distance = Math::Min(distance, d);
			intersect = true;
		}
	}

	return intersect;
}
