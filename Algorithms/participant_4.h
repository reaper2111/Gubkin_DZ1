#pragma once
#include "../Graph.h"
#include <algorithm>
#include <cstddef>
#include <vector>

namespace participant_4_detail {

struct Arc {
    int to;
    long long remaining;
};

class ScalingNetwork {
public:
    explicit ScalingNetwork(const Graph& graph)
        : outgoing_(graph.n), parent_arc_(graph.n),
          next_(graph.n), visited_(graph.n) {
        for (const Edge& edge : graph.edges) {
            long long capacity = edge.capacity > 0 ? edge.capacity : 0;
            int id = static_cast<int>(arcs_.size());
            outgoing_[edge.from].push_back(id);
            outgoing_[edge.to].push_back(id + 1);
            arcs_.push_back({edge.to, capacity});
            arcs_.push_back({edge.from, 0});
            if (capacity > max_capacity_) max_capacity_ = capacity;
        }
    }

    long long MaxFlow(int source, int sink) {
        if (max_capacity_ == 0) return 0;

        long long delta = 1;
        while (delta <= max_capacity_ / 2) delta *= 2;

        long long flow = 0;
        for (; delta >= 1; delta /= 2) {
            while (FindPath(source, sink, delta))
                flow += Augment(source, sink);
        }
        return flow;
    }

private:
    bool FindPath(int source, int sink, long long delta) {
        std::fill(visited_.begin(), visited_.end(), false);
        std::fill(next_.begin(), next_.end(), 0);

        stack_.clear();
        stack_.push_back(source);
        visited_[source] = true;

        while (!stack_.empty()) {
            int vertex = stack_.back();
            if (vertex == sink) return true;

            bool advanced = false;
            while (next_[vertex] < outgoing_[vertex].size()) {
                int id = outgoing_[vertex][next_[vertex]++];
                const Arc& arc = arcs_[id];
                if (!visited_[arc.to] && arc.remaining >= delta) {
                    visited_[arc.to] = true;
                    parent_arc_[arc.to] = id;
                    stack_.push_back(arc.to);
                    advanced = true;
                    break;
                }
            }
            if (!advanced) stack_.pop_back();
        }
        return false;
    }

    long long Augment(int source, int sink) {
        long long pushed = -1;
        for (int v = sink; v != source; v = arcs_[parent_arc_[v] ^ 1].to) {
            long long remaining = arcs_[parent_arc_[v]].remaining;
            if (pushed < 0 || remaining < pushed) pushed = remaining;
        }
        for (int v = sink; v != source; v = arcs_[parent_arc_[v] ^ 1].to) {
            int id = parent_arc_[v];
            arcs_[id].remaining -= pushed;
            arcs_[id ^ 1].remaining += pushed;
        }
        return pushed;
    }

    std::vector<Arc> arcs_;
    std::vector<std::vector<int>> outgoing_;
    std::vector<int> parent_arc_;
    std::vector<std::size_t> next_;
    std::vector<bool> visited_;
    std::vector<int> stack_;
    long long max_capacity_ = 0;
};

}

inline long long participant_4(const Graph& graph, int source, int sink) {
    if (graph.n <= 0 || source == sink) return 0;
    if (source < 0 || source >= graph.n || sink < 0 || sink >= graph.n) return 0;
    participant_4_detail::ScalingNetwork network(graph);
    return network.MaxFlow(source, sink);
}
