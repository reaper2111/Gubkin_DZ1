#pragma once

#include "../Graph.h"

#include <algorithm>
#include <climits>
#include <vector>

namespace participant_3_detail {

inline long long DijkstraWidest(
    int n,
    int source,
    int sink,
    const std::vector<std::vector<long long>>& residual,
    std::vector<int>& parent)
{
    std::vector<bool> visited(n, false);
    std::vector<long long> widest(n, 0);

    for (int vertex = 0; vertex < n; ++vertex) {
        widest[vertex] = residual[source][vertex];
        if (residual[source][vertex] > 0) {
            parent[vertex] = source;
        }
    }
    widest[source] = LLONG_MAX;
    visited[source] = true;

    for (int iteration = 0; iteration < n; ++iteration) {
        long long max_capacity = 0;
        int current = -1;

        for (int vertex = 0; vertex < n; ++vertex) {
            if (!visited[vertex] && widest[vertex] > max_capacity) {
                max_capacity = widest[vertex];
                current = vertex;
            }
        }

        if (current == -1) {
            break;
        }
        visited[current] = true;

        if (current == sink) {
            break;
        }

        for (int vertex = 0; vertex < n; ++vertex) {
            const long long candidate = std::min(
                widest[current], residual[current][vertex]);
            if (!visited[vertex] && candidate > widest[vertex]) {
                widest[vertex] = candidate;
                parent[vertex] = current;
            }
        }
    }

    return widest[sink];
}

} // namespace participant_3_detail

inline long long participant_3(const Graph& graph, int source, int sink)
{
    if (source == sink || graph.n <= 0) {
        return 0;
    }

    std::vector<std::vector<long long>> residual(
        graph.n, std::vector<long long>(graph.n, 0));

    for (const auto& edge : graph.edges) {
        if (edge.capacity > 0) {
            residual[edge.from][edge.to] += edge.capacity;
        }
    }

    long long max_flow = 0;
    std::vector<int> parent(graph.n, -1);

    while (true) {
        std::fill(parent.begin(), parent.end(), -1);
        const long long path_flow = participant_3_detail::DijkstraWidest(
            graph.n, source, sink, residual, parent);

        if (path_flow == 0) {
            break;
        }

        max_flow += path_flow;

        int current = sink;
        while (current != source) {
            const int previous = parent[current];
            residual[previous][current] -= path_flow;
            residual[current][previous] += path_flow;
            current = previous;
        }
    }

    return max_flow;
}
