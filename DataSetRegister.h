#pragma once
#include "DataSets.h"
#include <stack>

inline std::stack<DataSet> GenerateDataSets() {
    return std::stack<DataSet>({
        {"single edge", {2, {{0, 1, 7}}}, 0, 1, 7},
        {"two paths", {4, {{0, 1, 3}, {0, 2, 4}, {1, 3, 3}, {2, 3, 4}}}, 0, 3, 7},
        {"bottleneck", {4, {{0, 1, 8}, {1, 2, 2}, {2, 3, 9}}}, 0, 3, 2},
        {"no path", {4, {{0, 1, 5}, {2, 3, 6}}}, 0, 3, 0},
        {"parallel edges", {2, {{0, 1, 2}, {0, 1, 5}}}, 0, 1, 7},
        {"reverse edge", {3, {{0, 1, 4}, {1, 0, 9}, {1, 2, 4}}}, 0, 2, 4},
        {"classic six vertices", {6, {{0, 1, 16}, {0, 2, 13}, {1, 2, 10},
            {2, 1, 4}, {1, 3, 12}, {3, 2, 9}, {2, 4, 14}, {4, 3, 7},
            {3, 5, 20}, {4, 5, 4}}}, 0, 5, 23},
    });
}

