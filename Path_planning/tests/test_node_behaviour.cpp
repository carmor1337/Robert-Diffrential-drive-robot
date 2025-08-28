#include <gtest/gtest.h>
#include "Node.h"  // your Dstarlite implementation header

#include <utility>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <memory>
#include <SFML/Graphics.hpp>


TEST(NodeBehaviourTest, NodeHashTest) {
    // Create an instance or setup your algorithm
    auto node1 = std::make_shared<Node>(Node{0, 0});
    auto node2 = std::make_shared<Node>(Node{1, 2});

    std::unordered_map<std::pair<int, int>, std::shared_ptr<Node>, PairHash> node_map;

     // Insert nodes into map
     node_map[{node1->x, node1->y}] = node1;
     node_map[{node2->x, node2->y}] = node2;
 
     // Check if nodes can be found by their keys
     auto it1 = node_map.find({0,0});
     auto it2 = node_map.find({1,2});
     auto it3 = node_map.find({3,3});  // Not inserted
 
     ASSERT_NE(it1, node_map.end());
     EXPECT_EQ(it1->second->x, 0);
     EXPECT_EQ(it1->second->y, 0);
 
     ASSERT_NE(it2, node_map.end());
     EXPECT_EQ(it2->second->x, 1);
     EXPECT_EQ(it2->second->y, 2);
 
     EXPECT_EQ(it3, node_map.end());
}

TEST(NodeBehaviourTest, NodePriorityQueueTest) {

    auto node1 = std::make_shared<Node>(Node{0, 0});
    auto node2 = std::make_shared<Node>(Node{1, 2});

    // setup keys
    std::pair<double, double> key1 {1.00,1.00};
    std::pair<double, double> key2 {2.00,2.00};
    
    // assignes keys to the nodes
    node1->key = key1;
    node2->key = key2;
    

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> open_list;
    open_list.push(node2);
    open_list.push(node1);

    ASSERT_EQ(open_list.empty(), false);
    ASSERT_EQ(open_list.size(), 2);

    std::shared_ptr<Node> pop1 = open_list.top(); 
    open_list.pop();

    std::shared_ptr<Node> pop2 = open_list.top(); 
    open_list.pop();
    ASSERT_EQ(open_list.size(), 0);

    ASSERT_EQ(node1, pop1);
    ASSERT_EQ(node2, pop2);
    
}

