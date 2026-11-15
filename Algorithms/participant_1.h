#pragma once
#include "../Graph.h"
#include <algorithm>
#include <limits>
#include <vector>

namespace participant_1_detail {
struct Arc {
    int to;
    long long remaining;
};

struct Network {
    std::vector<Arc> arcs;
    std::vector<std::vector<int>> outgoing;

    explicit Network(const Graph& graph) : outgoing(graph.n) {
        for (const auto& edge : graph.edges) {
            int id = static_cast<int>(arcs.size());
            outgoing[edge.from].push_back(id);
            outgoing[edge.to].push_back(id + 1);
            // Каждому исходному ребру соответствует своё обратное ребро.
            arcs.push_back({edge.to, edge.capacity});
            arcs.push_back({edge.from, 0});
        }
    }
};
} // namespace participant_1_detail

long long participant_1(const Graph& graph, int source, int sink);
