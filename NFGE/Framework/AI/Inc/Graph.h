#ifndef INCLUDE_NFGE_AI_GRAPH_H
#define INCLUDE_NFGE_AI_GRAPH_H

#include "Common.h"


namespace NFGE::AI {
	constexpr size_t InvalidNode = std::numeric_limits<size_t>::max();

	struct Node
	{
		std::vector<size_t> neighbors;

	};

	template <typename NodeType>
	class Graph
	{
	public:
		void Initialize(size_t nodeCount);

		NodeType& GetNode(size_t index);
		const NodeType& GetNode(size_t index) const;
		size_t GetNodeCount() const;
	private:
		std::vector<NodeType> mNodes;
	};

	template<typename NodeType>
	inline void Graph<NodeType>::Initialize(size_t nodeCount)
	{
		mNodes.resize(nodeCount);
	}
	template<typename NodeType>
	inline NodeType & Graph<NodeType>::GetNode(size_t index)
	{
		return mNodes[index];
	}
	template<typename NodeType>
	inline const NodeType & Graph<NodeType>::GetNode(size_t index) const
	{
		return mNodes[index];
	}
	template<typename NodeType>
	inline size_t Graph<NodeType>::GetNodeCount() const
	{
		return mNodes.size();
	}
} // namespace NFGE::AI
#endif // !INCLUDE_NFGE_AI_GRAPH_H
