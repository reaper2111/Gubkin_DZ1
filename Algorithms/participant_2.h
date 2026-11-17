#pragma once
#include "../Graph.h"
#include <vector>

inline long long participant_2(const Graph& graph, int source, int sink) {
    int n = graph.n;
    std::vector<std::vector<long long>> capacity(n, std::vector<long long>(n, 0));

    for (const auto& edge : graph.edges) {
        capacity[edge.from][edge.to] += edge.capacity;
    }

    return 0;
}
