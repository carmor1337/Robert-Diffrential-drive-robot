
#include "Node.h"

#include <cmath>  // for INFINITY
#include <functional>  // for std::hash

Node::Node(int x_, int y_) : x(x_), y(y_) {}

bool Node::operator==(const Node& other) const {
    return x == other.x && y == other.y;
}

std::size_t PairHash::operator()(const std::pair<int, int>& p) const {
    return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
}

bool CompareNode::operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
    return a->key > b->key;
}
