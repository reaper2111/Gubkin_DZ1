#pragma once
#include "../Graph.h"
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

// Алгоритм Диница: BFS строит слоистую сеть, DFS ищет в ней блокирующий поток.
// Повторяем, пока сток достижим из истока.

namespace participant_5_detail {

struct ResidualEdge {
    int to;
    long long cap;   // оставшаяся (остаточная) пропускная способность
};

class Dinic {
public:
    explicit Dinic(int n) : adj_(n), level_(n), it_(n) {}

    // Каждое ребро хранится парой: прямое (индекс i) и обратное (i ^ 1).
    void AddEdge(int from, int to, long long cap) {
        adj_[from].push_back(static_cast<int>(edges_.size()));
        edges_.push_back({to, cap});
        adj_[to].push_back(static_cast<int>(edges_.size()));
        edges_.push_back({from, 0});
    }

    long long MaxFlow(int s, int t) {
        if (s == t) return 0;
        long long flow = 0;
        while (Bfs(s, t)) {
            std::fill(it_.begin(), it_.end(), 0);
            while (long long pushed = Dfs(s, t))
                flow += pushed;
        }
        return flow;
    }

private:
    // Строим слоистую сеть. true — если сток достижим.
    bool Bfs(int s, int t) {
        std::fill(level_.begin(), level_.end(), -1);
        std::queue<int> q;
        level_[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int id : adj_[u]) {
                const ResidualEdge& e = edges_[id];
                if (e.cap > 0 && level_[e.to] == -1) {
                    level_[e.to] = level_[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level_[t] != -1;
    }

    // Ищем один путь s -> t в слоистой сети и пускаем по нему поток.
    // Обход без рекурсии (свой стек path_), чтобы на длинных путях
    // не переполнялся стек вызовов. Возвращает 0, если пути больше нет.
    long long Dfs(int s, int t) {
        path_.clear();
        int u = s;
        while (true) {
            if (u == t) {
                long long pushed = std::numeric_limits<long long>::max();
                for (int id : path_)
                    pushed = std::min(pushed, edges_[id].cap);
                for (int id : path_) {
                    edges_[id].cap -= pushed;
                    edges_[id ^ 1].cap += pushed;
                }
                return pushed;
            }
            bool advanced = false;
            for (int& i = it_[u]; i < static_cast<int>(adj_[u].size()); ++i) {
                int id = adj_[u][i];
                const ResidualEdge& e = edges_[id];
                if (e.cap > 0 && level_[e.to] == level_[u] + 1) {
                    path_.push_back(id);
                    u = e.to;
                    advanced = true;
                    break;
                }
            }
            if (advanced) continue;
            // тупик: из u до стока не дойти
            if (u == s) return 0;
            int id = path_.back();
            path_.pop_back();
            u = edges_[id ^ 1].to;   // возвращаемся в предыдущую вершину
            ++it_[u];                // и больше это ребро не пробуем
        }
    }

    std::vector<ResidualEdge> edges_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> level_;
    std::vector<int> it_;
    std::vector<int> path_;  // id рёбер текущего пути
};

}  // namespace participant_5_detail

inline long long participant_5(const Graph& graph, int source, int sink) {
    participant_5_detail::Dinic d(graph.n);
    for (const Edge& e : graph.edges)
        d.AddEdge(e.from, e.to, e.capacity);
    return d.MaxFlow(source, sink);
}
