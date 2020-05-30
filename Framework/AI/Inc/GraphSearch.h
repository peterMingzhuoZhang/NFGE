#ifndef INCLUDE_NFGE_AI_GRAPHSEARCH_H
#define INCLUDE_NFGE_AI_GRAPHSEARCH_H

#include "Graph.h"

namespace NFGE::AI::GraphSearch
{
	struct Context
	{
		size_t nodeCount;
		std::list<size_t> openList;
		std::list<size_t> closedList;
		std::vector<size_t> parent;
		std::vector<bool> open;
		std::vector<bool> closed;
		std::vector<float> g;
		std::vector<float> h;
		size_t start = InvalidNode;
		size_t end = InvalidNode;
		bool found = false;

		void Init(size_t nodes)
		{
			nodeCount = nodes;
		}

		void Reset()
		{
			openList.clear();
			closedList.clear();
			parent.clear();
			open.clear();
			closed.clear();
			g.clear();
			h.clear();

			parent.resize(nodeCount, InvalidNode);
			open.resize(nodeCount, false);
			closed.resize(nodeCount, false);
			g.resize(nodeCount, 0.0f);
			h.resize(nodeCount, 0.0f);

			size_t start = InvalidNode;
			size_t end = InvalidNode;

			found = false;
		}
	};

	template <typename GraphType, typename OpenPolicy, typename NextPolicy, typename ValidationPolicy>
	struct BaseSearch
	{
		BaseSearch(const GraphType& graph)
			:graph(graph)
		{}

		void Execute(Context& context)
		{
			if (!context.open[context.start])
			{
				Open(context, context.start, AI::InvalidNode);
			}
			while (!context.found && !context.openList.empty())
			{
				size_t nodeIndex = Next(context);

				if (nodeIndex == context.end)
				{
					context.found = true;
				}
				else
				{
					auto& node = graph.GetNode(nodeIndex);
					for (auto& n : node.neighbors)
					{
						if (IsValid(context, n))
						{
							Open(context, n, nodeIndex);
						}
					}
				}

				context.closedList.push_back(nodeIndex);
				context.closed[nodeIndex] = true;
			}
		}

		const GraphType& graph;
		OpenPolicy Open;
		NextPolicy Next;
		ValidationPolicy IsValid;
	};

	struct PushBackPolicy
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			context.openList.push_back(nodeIndex);
			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
		}
	};

	template<typename CostPolicy>
	struct InsertSortedPolicy
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			float g = Cost(context, parentIndex, nodeIndex);

			if ((context.open[nodeIndex]) && (context.g[nodeIndex] > g))
			{
				context.openList.remove(nodeIndex);
				context.open[nodeIndex] = false;
			}
			else if (context.open[nodeIndex])
			{
				return;
			}

			context.g[nodeIndex] = g;

			bool hasInsert = false;
			std::list<size_t>::iterator it;
			for (it = context.openList.begin(); it != context.openList.end(); ++it)
			{
				if (g >= context.g[*it])
				{
					context.openList.insert(it, nodeIndex);
					hasInsert = true;
					break;
				}
			}

			if (hasInsert == false)
				context.openList.push_back(nodeIndex);

			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
		}

		CostPolicy Cost;
	};

	template<typename G_CostPolicy, typename H_CostPolicy>
	struct InsertSortedPolicy_A_Star
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			float g = gCost(context, parentIndex, nodeIndex);
			float h = hCost(context, nodeIndex);

			if ((context.open[nodeIndex]) && (context.g[nodeIndex] + context.h[nodeIndex] > g + h))		//|
			{																							//|
				context.openList.remove(nodeIndex);														//|--- kick out that node, because it is too costy
				context.open[nodeIndex] = false;														//|
			}																							//|
			else if (context.open[nodeIndex])
			{
				return;
			}
			else
			{
				context.h[nodeIndex] = h;		// Only set h cost when first added
			}

			context.g[nodeIndex] = g;

			bool hasInsert = false;
			std::list<size_t>::iterator it;
			for (it = context.openList.begin(); it != context.openList.end(); ++it)
			{
				if (g + h >= context.g[*it] + context.h[*it])
				{
					context.openList.insert(it, nodeIndex);
					hasInsert = true;
					break;
				}
			}

			if (hasInsert == false)
				context.openList.push_back(nodeIndex);

			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
		}

		G_CostPolicy gCost;
		H_CostPolicy hCost;
	};

	struct PushFrontPolicy
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			context.openList.push_front(nodeIndex);
			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
		}
	};

	struct PopFrontPolicy
	{
		size_t operator()(Context& context)
		{
			auto next = context.openList.front();
			context.openList.pop_front();
			return next;
		}
	};

	struct PopBackPolicy
	{
		size_t operator()(Context& context)
		{
			auto next = context.openList.back();
			context.openList.pop_back();
			return next;
		}
	};

	template<typename GraphType, typename ValidationPolicy>
	using BFS = BaseSearch<GraphType, PushBackPolicy, PopFrontPolicy, ValidationPolicy>;

	template<typename GraphType, typename ValidationPolicy>
	using DFS = BaseSearch<GraphType, PushFrontPolicy, PopFrontPolicy, ValidationPolicy>;

	template<typename GraphType, typename ValidationPolicy, typename CostPolicy>
	using Dijkstra = BaseSearch<GraphType, InsertSortedPolicy<CostPolicy>, PopBackPolicy, ValidationPolicy>;

	template<typename GraphType, typename ValidationPolicy, typename G_CostPolicy, typename H_CostPolicy>
	using A_Star = BaseSearch<GraphType, InsertSortedPolicy_A_Star<G_CostPolicy, H_CostPolicy>, PopBackPolicy, ValidationPolicy>;
} // namespace NFGE::AI::GraphSearch

#endif // !INCLUDE_NFGE_AI_GRAPHSEARCH_H
