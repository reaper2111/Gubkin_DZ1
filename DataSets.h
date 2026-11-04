#pragma once
#include "Graph.h"
#include <string>

struct DataSet {
    std::string name;
    Graph graph;
    int source;
    int sink;
    long long expected;
};
