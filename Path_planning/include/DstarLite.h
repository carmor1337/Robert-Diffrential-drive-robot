#pragma once
#include "Node.h"
#include <Robot.h>
#include <TrueMap.h>

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <memory>
#include <SFML/Graphics.hpp>


// D*lite implementation unoptimized duet to not being able to acces priorityqueue
// priority Queue
// Nodes with sharepointers
// cosists of g an rhs
// key = cost_to_goal + heuristic
// C = cost between nodes
// Successors(s): All nodes that can be reached from node s (i.e., neighbors you can move to).
// Predecessors(s): All nodes that can move to s (i.e., neighbors that consider s a successor).

using key          = std::pair<double,double>;
using coordinates  = std::pair<int,int> ;
using node_pointer = std::shared_ptr<Node>;


class DstarLite
{

    private:
    // TEMP: ints to integrate easy with grid
    coordinates goal_ints; 

    // Used in order to have a simple goal node and integrate easily
    TrueMap map; // Needed to setup a map for Robert, might be discarded later


    // initialize all the nodes to g=rhs=INFINITY for all cell in map, but NOT the keys
    void initialize();

    public:
    // constructor
    DstarLite(coordinates init_start, coordinates init_goal) ;

    Robot robert; //Provides the map and the position in the map as well as the goal
    double key_m {0};
    node_pointer goal_node; 



    std::priority_queue<node_pointer, std::vector<node_pointer>, CompareNode> open_list;
    std::unordered_map<coordinates, node_pointer, PairHash> node_map;
    node_pointer current_node_p;
    node_pointer last_node_p;
    key calculate_key(node_pointer node_p);

    void update_vertex(node_pointer node_p);

    void compute_shortest_path_unoptimized();


    // Computes the shortest valid path: optimized version
    void compute_shortest_path();

    void dstar_main();


    // ###################################    Helper function  ###########################################

    // find the min successor of all neighbours and returns that node
    node_pointer find_min_successor(std::vector<node_pointer> neighbours,node_pointer current_node_p);

    // Returns the neighbours of the current node. Sucessor or predecessor is determined locally
    std::vector<node_pointer> get_neighours(node_pointer current_node);

    // Pop nodes until a valid one is presented
    node_pointer pop_open_list();
    
    // pushes a node onto the priorityqueue
    void push_open_list(node_pointer node_p);


    std::vector<std::pair<coordinates,int>>  scan_for_changes();

    // checks the while loop condition for the shortest path while loop
    bool shortest_path_not_done();

    // Calculates the dist between tẃo nodes, with abs value
    double dist(const node_pointer node1 , const node_pointer node2);
};

