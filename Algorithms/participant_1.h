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

inline long long dfs(Network& net, int vertex, int sink, long long limit,
                     std::vector<bool>& visited) {
    if (vertex == sink) return limit;
    visited[vertex] = true;

    for (int id : net.outgoing[vertex]) {
        Arc& arc = net.arcs[id];
        if (visited[arc.to] || arc.remaining == 0) continue;

        long long sent = dfs(net, arc.to, sink, std::min(limit, arc.remaining), visited);
        if (sent > 0) {
            arc.remaining -= sent;
            // Номера рёбер в паре отличаются младшим битом.
            net.arcs[id ^ 1].remaining += sent;
            return sent;
        }
    }
    return 0;
}
} // namespace participant_1_detail

inline long long participant_1(const Graph& graph, int source, int sink) {
    if (source == sink) return 0;
    participant_1_detail::Network net(graph);
    long long flow = 0;
    while (true) {
        std::vector<bool> visited(graph.n, false);
        long long sent = participant_1_detail::dfs(
            net, source, sink, std::numeric_limits<long long>::max(), visited);
        if (sent == 0) return flow;
        flow += sent;
    }
}
