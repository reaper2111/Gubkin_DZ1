#pragma once
#include "../Graph.h"
#include <vector>
#include <queue>

inline long long participant_2(const Graph& graph, int source, int sink) {
    int n = graph.n;
    std::vector<std::vector<long long>> capacity(n, std::vector<long long>(n, 0));

    for (const auto& edge : graph.edges) {
        capacity[edge.from][edge.to] += edge.capacity;
    }

    std::vector<int> parent(n, -1);
    std::queue<int> q;

    q.push(source);
    parent[source] = source;

    while (!q.empty() && parent[sink] == -1) {
        int v = q.front();
        q.pop();

        for (int u = 0; u < n; u++) {
            if (parent[u] == -1 && capacity[v][u] > 0) {
                parent[u] = v;
                q.push(u);
            }
        }
    }

    return 0;
}
