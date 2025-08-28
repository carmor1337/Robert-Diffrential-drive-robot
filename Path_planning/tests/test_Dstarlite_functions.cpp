#include <gtest/gtest.h>

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

// will prob need a testsuite for this 

#include "DstarLite.h"  // Your implementation header
#include "Robot.h"
#include <TrueMap.h>
#include "Node.h"

using coordinates = std::pair<int,int> ;
using node_pointer = std::shared_ptr<Node>;
using key = std::pair<double,double>;

class DStarTest : public ::testing::Test {
    protected:
    float map_size = 10;
    float resolution = 1;
    int map_select = 0;
    double goal_x = 9;
    double goal_y = 9;
    std::pair<int,int> goal{9,9};
    std::pair<int,int> start{0,0};


    DstarLite* planner = nullptr;
    TrueMap* map_true = nullptr;

    void SetUp() override {
        map_true = new TrueMap(map_size, map_size, resolution, map_select);
        planner = new DstarLite(start,goal);
    }


    void TearDown() 
     {
        // Clean up if necessary
    }
};
// TestSuite
// decide size of testmap
// select goal node in map
// initialize object
// 

TEST_F(DStarTest, CalculateKeysTest) 
{
    node_pointer test_node = std::make_shared<Node>(0,0);
    ASSERT_EQ(test_node->x,0);
    ASSERT_EQ(test_node->y,0);
    ASSERT_EQ(test_node->g,INFINITY);
    ASSERT_EQ(test_node->rhs,INFINITY);
    key test_key = planner->calculate_key(test_node);
    ASSERT_EQ(test_key.first,INFINITY);
    ASSERT_EQ(test_key.second,INFINITY);
    
    // will need to test with all the diffrent types of RHS values and it it is consistent

}

TEST_F(DStarTest, InitializeTest) 
{   
    // assert variables which should be set
    ASSERT_EQ(planner->key_m, 0);
    ASSERT_EQ(planner->node_map.size(), 100); // (x*y)/resolution 
    ASSERT_EQ(planner->open_list.size(),1);

    node_pointer popped = planner->pop_open_list();
    ASSERT_EQ(popped,planner->goal_node);
    ASSERT_EQ(popped->rhs,0);   

}


TEST_F(DStarTest, UpdateVertexTest) {
    node_pointer test_node1 = std::make_shared<Node>(0,0);

    // test correct behaviour of push when rhs!=g
    test_node1->rhs = 10.0;
    planner->update_vertex(test_node1);
    ASSERT_EQ(planner->open_list.size(),2);
    test_node1->g = 10.0;
    // test correct behaviour of not push when rhs==g
    planner->update_vertex(test_node1);
    ASSERT_NE(planner->open_list.size(),3);


}

TEST_F(DStarTest, ComputeShortestPathTest) {
    
    planner->compute_shortest_path();
    // Need to figure out what and how to test it
}

/*
TEST_F(DStarTest, MainOneStepTest) {
    int i{0};
}
TEST_F(DStarTest, ScanSurroundingTest) {
    int i{0};
}

//////////////////   Helper funtion tests   //////////////////////////////

TEST(DstarliteHelper, GetNeighboursTest) {
    int i{0};
}

TEST(DstarliteHelper, PopOpenListTest ) {
    auto node1 = std::make_shared<Node>(Node{0, 0});
    auto node2 = std::make_shared<Node>(Node{1, 2});

    // setup keys
    std::pair<double, double> key1 {1.00,1.00};
    std::pair<double, double> key2 {2.00,2.00};
    
    // assignes keys to the nodes
    node1->key = key1;
    node2->key = key2;
    

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> open_list;
    open_list.push(node1);
    open_list.push(node2);

    auto pop1 = 1; // dstarlite

}

*/