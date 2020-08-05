#ifndef INCLUDE_NFGE_QUADTREE_H
#define INCLUDE_NFGE_QUADTREE_H


namespace NFGE
{
	class QuadTree
	{
	public:
		struct Node
		{
			std::array<std::unique_ptr<Node>, 4> children;
			std::vector<uint32_t> indces;
			NFGE::Math::AABB aabb;
			bool isLeaf = false;

			void DrawNode() const
			{
				auto center = aabb.center;
				auto max = Math::Max(Math::Abs(center.x), Math::Max(Math::Abs(center.y), Math::Abs(center.z)));

				Graphics::SimpleDraw::AddAABB(aabb, NFGE::Math::Vector4{ (center.x / max),(center.y / max), (center.z / max), 1.0f });
			};
		};

		using Visitor = std::function<bool(Node*)>;

		void Initialize(const Graphics::Mesh*, uint32_t levels);

		void Visit(Visitor& visitor);

		bool Intersect(const NFGE::Math::Ray& ray, float& distance) const;

	private:
		enum Quadrant { BL, BR, TL, TR };
		
		void GenerateNodeRecursive(std::unique_ptr<Node>& node,const Math::AABB& aabb, uint32_t levels);
		void VisitRecursive(Node* node, Visitor& visitor);
		bool IntersectRecursive(const Node* node, const NFGE::Math::Ray& ray, float& distance) const;

		const Graphics::Mesh* mMesh = nullptr;
		std::unique_ptr<Node> mRoot = nullptr;
	};
}

#endif // !INCLUDE_NFGE_QUADTREE_H
