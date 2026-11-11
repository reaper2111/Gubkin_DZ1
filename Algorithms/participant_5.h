#pragma once
#include "../Graph.h"
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

// Участник 5: Артём Бабичев — алгоритм Диница (Dinic, 1970).
//
// Идея:
//   1. BFS из истока по остаточной сети строит "слоистую сеть":
//      level[v] = длина кратчайшего пути (в рёбрах) от истока до v.
//   2. Если сток недостижим — поток максимален, конец.
//   3. DFS ищет "блокирующий поток": пускаем поток только по рёбрам
//      u -> v с level[v] == level[u] + 1. Массив it[] запоминает, какое
//      ребро вершины проверять следующим, чтобы не перебирать заново
//      уже "мёртвые" рёбра.
//   4. Повторяем с шага 1.
//
// Сложность: O(V^2 * E) в общем случае, O(E * sqrt(V)) на единичных сетях.

namespace dinic_detail {

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
            while (long long pushed = Dfs(s, t, std::numeric_limits<long long>::max()))
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

    // Проталкиваем поток не больше limit из u в t по слоистой сети.
    long long Dfs(int u, int t, long long limit) {
        if (u == t) return limit;
        for (int& i = it_[u]; i < static_cast<int>(adj_[u].size()); ++i) {
            int id = adj_[u][i];
            ResidualEdge& e = edges_[id];
            if (e.cap > 0 && level_[e.to] == level_[u] + 1) {
                long long pushed = Dfs(e.to, t, std::min(limit, e.cap));
                if (pushed > 0) {
                    e.cap -= pushed;
                    edges_[id ^ 1].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;  // из u больше ничего не протолкнуть
    }

    std::vector<ResidualEdge> edges_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> level_;
    std::vector<int> it_;
};

}  // namespace dinic_detail

inline long long participant_5(const Graph& graph, int source, int sink) {
    dinic_detail::Dinic d(graph.n);
    for (const Edge& e : graph.edges)
        d.AddEdge(e.from, e.to, e.capacity);
    return d.MaxFlow(source, sink);
}
