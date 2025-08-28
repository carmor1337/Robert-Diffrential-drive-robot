#pragma once

#include <utility>
#include <memory>
#include <cmath>


struct Node {
    int x, y;
    double g = INFINITY;
    double rhs = INFINITY;
    std::pair<double, double> key;
    bool in_queue {false};
    bool in_obstacle {false};

    Node(int x_, int y_);

    bool operator==(const Node& other) const;
};

// For unordered_map with pair<int, int> as key
struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const;
};

// For priority queue
struct CompareNode {
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const;
};
