#pragma once
#include <vector>

struct Edge {
    int from;
    int to; 
    long long capacity;
};

struct Graph {
    int n;
    std::vector<Edge> edges; 
}; 



