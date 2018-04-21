#include "bk_maxflow/adjacency_list_new_interface/graph.h"
#include <cassert>


template <typename node_t>
class MaxFlow
{
	typedef float value_t;

public:
	typedef Graph<value_t, value_t, value_t> graph_t;

public:
	MaxFlow(size_t numNodes) : graph(numNodes, 4 * numNodes) {}

	inline void addNode(node_t n) {
		node_t node_id = graph.add_node();

		assert(node_id == n);
	}

    inline void addTweights(node_t n, value_t source, value_t sink) {
        graph.add_tweights(n, source, sink);
    }

	inline void addEdge(node_t n1, node_t n2, value_t capacity, value_t reverseCapacity) {
		assert(capacity >= 0 && reverseCapacity >= 0);
		graph.add_edge(n1, n2, capacity, reverseCapacity);
	}

	value_t computeMaxFlow() {
		return graph.maxflow();
	}

	inline bool isNodeOnSrcSide(node_t n) {
		return (graph.what_segment(n) == graph_t::SOURCE);
	}

protected:
	graph_t graph;
};
