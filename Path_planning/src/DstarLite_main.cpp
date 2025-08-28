#include <Robot.h>
#include <TrueMap.h>

#include "Node.h"


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

using coordinates = std::pair<int,int> ;
using node_pointer = std::shared_ptr<Node>;
using key = std::pair<double,double>;

class DstarLite
{

    private:
    // TEMP: ints to integrate easy with grid
    coordinates goal_ints; 

    // Used in order to have a simple goal node and integrate easily
    node_pointer goal_node; 
    TrueMap map; // Needed to setup a map for Robert, might be discarded later
    Robot robert; //Provides the map and the position in the map as well as the goal

    double key_m {0};

    // initialize all the nodes to g=rhs=INFINITY for all cell in map, but NOT the keys
    void initialize()
    {
        key_m = 0.00;
        for (int i; i<robert.robot_map->size;i++)
        {
            for (int j; j<robert.robot_map->size; j++)
            {   
                if (i == goal_node->x && j == goal_node->y)
                {   
                    goal_node->rhs = 0;
                    goal_node->key = calculate_key(goal_node);

                    node_map[{goal_node->x, goal_node->y}] = goal_node;
                    open_list.push(goal_node);
                }
                else 
                {   //creates a node and put it into the hashmap
                    node_pointer node = std::make_shared<Node>(i,j);
                    node_map[{node->x, node->y}] = node;
                }
            }
        }  
    }

    public:
    // constructor
    DstarLite(std::pair<int,int> start_goal) 
    : map(10, 10, 1, 0), 
      robert(map), 
      goal_ints(start_goal)
    {    
        goal_node = std::make_shared<Node>(goal_ints.first,goal_ints.second);
    }

    std::priority_queue<node_pointer, std::vector<node_pointer>, CompareNode> open_list;
    std::unordered_map<coordinates, node_pointer, PairHash> node_map;
    node_pointer current_node_p;
    node_pointer last_node_p;

    key calculate_key(node_pointer node_p)
    {   
        coordinates coords = robert.get_coordinates();
        int x = coords.first;
        int y = coords.second;            

        double h = dist(node_p, current_node_p);        // or dist to s_start
        double min_g_rhs = std::min(node_p->g, node_p->rhs);
        return {min_g_rhs + h + key_m, min_g_rhs};
    }
   

    void update_vertex(node_pointer node_p)
    {
        // optimized update vertex
        // using lazy insetion means that 
        // update = recalculate and then push the new updated node
        // Insert is as normal
        // remove does nothing
        if (node_p->g != node_p->rhs) // and u in priorityqueue
        {
            push_open_list(node_p);
        }
        //else if (node_p->g != node_p->rhs) // and u not in priorityqueue
        //{ do nothing due to lazy instert}
    }

    void compute_shortest_path_unoptimized()
    {
        while (shortest_path_not_done())
        {
            node_pointer node_p = pop_open_list();
            key key_old = node_p->key;
            key key_new = calculate_key(node_p);

            if (key_old < key_new)
            {
                //node_p->key = key_new; done inside push_open_list
                push_open_list(node_p);
            }
            else if (node_p->g > node_p->rhs)
            {
                // for all predecessors of node_p: update vertex
                auto neighbour_list = get_neighours(node_p);
                for (auto neighbour_p: neighbour_list)
                {
                    update_vertex(neighbour_p);

                }
            }
            else
            {
                update_vertex(node_p);
                auto neighbour_list = get_neighours(node_p);
                for (auto neighbour_p: neighbour_list)
                {
                    update_vertex(neighbour_p);

                }
            }
        }
    }


    // Computes the shortest valid path: optimized version
    void compute_shortest_path()
    {
        while (shortest_path_not_done())
        {
            node_pointer node_p = pop_open_list();
            key key_old = node_p->key;
            key key_new = calculate_key(node_p);

            if (key_old < key_new)
            {
                push_open_list(node_p);
            }
            else if (node_p->g > node_p->rhs)
            {
                node_p->g = node_p->rhs;
                // for all predecessors of node_p: update vertex
                auto neighbour_list = get_neighours(node_p);
                for (auto neighbour_p: neighbour_list)
                {
                    update_vertex(neighbour_p);

                }
            }
            else
            {
                double g_old = node_p->g;
                node_p->g = INFINITY;

                auto neighbour_list = get_neighours(node_p);
                neighbour_list.push_back(node_p);
                
                for (auto neighbour_p: neighbour_list)
                {
                    if (neighbour_p->rhs == dist(neighbour_p,node_p) + g_old)
                    {
                        if (neighbour_p != goal_node)
                        {
                            auto neighbour_neighbours_list = get_neighours(neighbour_p);
                            auto min_node = find_min_successor(neighbour_neighbours_list,neighbour_p);
                            neighbour_p->rhs = dist(neighbour_p, min_node) + min_node->g;
                        }
                    }

                    update_vertex(neighbour_p);

                }
            }
        }
    }

    void dstar_main()
    {
        initialize();
        coordinates start_coords = robert.get_coordinates();
        if (!robert.robot_map->isInBoundsAndNotObstacle(start_coords.first , start_coords.second))
        {
            std::cout<< "Not valid starting Coordinates\n";
            return;
        }
        auto it = node_map.find(start_coords);
        current_node_p = it->second;
        last_node_p = current_node_p;

        compute_shortest_path();
        while (current_node_p != goal_node)
        {
            if (current_node_p->g == INFINITY)
            {
                std::cout << "No path to goal could be found.\n";
                return;
            }
            
            // start_node = min cost of successors based on cost(start,successor)+ g(successor)
            // next node = min(get_neighbours)
            std::vector<node_pointer> neighbours = get_neighours(current_node_p);
            node_pointer next_node_p = find_min_successor(neighbours,current_node_p);
            
            // move to start_node (better to call it current)
            
            robert.set_coordinates(next_node_p->x,next_node_p->y);
            current_node_p = next_node_p;
            // scan for changes
            std::vector<std::pair<coordinates,int>> changes_with_coords = scan_for_changes();

            // if any edge cost has changed
            if (!changes_with_coords.empty()) // meaning changes happened
            {
                key_m = key_m + dist( last_node_p, current_node_p);
                last_node_p = current_node_p;
                for (auto change: changes_with_coords)
                {
                    
                    coordinates node_coords = change.first;
                    auto node_p = node_map.at(node_coords);

                    auto neighbour_list = get_neighours(node_p);
                    for (auto neighbour: neighbour_list)
                    {
                        double old_cost = dist(node_p,neighbour);

                        if (change.second == 1) {node_p->in_obstacle = true;}
                        else {node_p->in_obstacle = false;}

                        double new_cost = dist(node_p,neighbour);
                        if (old_cost> new_cost)
                        {
                            if (node_p !=goal_node)
                            {
                                node_p->rhs = std::min(node_p->rhs,new_cost+ neighbour->g);
                            }
                        }
                        else if (node_p->rhs == old_cost + neighbour->g)
                        {
                            if (node_p !=goal_node)
                            {   
                                auto neighbour_list_2 = get_neighours(node_p);
                                auto min_node = find_min_successor(neighbour_list_2,node_p);
                                node_p->rhs = dist(node_p,min_node) + min_node->g;
                            }

                        }
                    }
                    update_vertex(node_p);
                }
                compute_shortest_path();
            }
        }
    }


    // ###################################    Helper function  ###########################################


    node_pointer find_min_successor(std::vector<node_pointer> neighbours,node_pointer current_node_p)
    {   
        if (neighbours.empty())
        {
            return nullptr;
        }
        double min_value = INFINITY;
        node_pointer best_neighbour = nullptr;
        for (auto node_p: neighbours)
        {
            double value = dist(current_node_p,node_p) + node_p->g;
            if (value < min_value)
            {
                min_value = value;
                best_neighbour = node_p;
            }
        }
        return best_neighbour;


    }

    // Returns the neighbours of the current node. Sucessor or predecessor is determined locally
    std::vector<node_pointer> get_neighours(node_pointer current_node)
    {
        //std::cout << "Entering get neighbours \n";
        std::vector<node_pointer> neighbours; 

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {   
                if (dx == 0 && dy == 0) // Skip self
                    continue;
    
                if (!robert.robot_map->isInBounds(current_node->x + dx, current_node->y + dy))
                {    
                continue;
                };
                coordinates id(current_node->x + dx,current_node->y+dy);
                auto it = node_map.find(id);
                if (it != node_map.end()) 
                {
                    node_pointer neighbour_node_p = it->second;
                    neighbours.push_back(neighbour_node_p); // Avoid copy
                }
                
                
            }
        }

    return neighbours; // Copying happens, but now references remain valid

    }
        
    node_pointer pop_open_list()
    {
        while (!open_list.empty()) {
            node_pointer node_p = open_list.top();
            open_list.pop();
    
            if (node_p->key == calculate_key(node_p)) {
                // Valid (non-stale) entry
                return node_p; 
            }
            // Stale node, to be skipped
        }
    
        return nullptr;  // no valid node found 
    }

    void push_open_list(node_pointer node_p)
    {   
        // calculates the node key
        // set the node key inside
        // push onto the open list with the key value

        key new_key = calculate_key(node_p);
        node_p->key = new_key;
        //node_p->in_queue = true;
        open_list.push(node_p);
    }


    std::vector<std::pair<coordinates,int>>  scan_for_changes()
    {
        // gets a return message if anything in map has changed and where it has changed into what
        // should resonable only return with the obstacle value.
        std::vector<std::pair<coordinates,int>> new_obstacle_coordinate; 
        
        
        return new_obstacle_coordinate ;
    }

    // checks the while loop condition for the shortest path while loop
    bool shortest_path_not_done()
    {
        if (open_list.empty()) return false; // edge case safety
    
        node_pointer top_node = open_list.top();
        return (top_node->key < calculate_key(current_node_p)) ||
                (current_node_p->g != current_node_p->rhs);
    }

    double dist(const node_pointer node1 , const node_pointer node2 )
        {
            if (node1->in_obstacle || node2->in_obstacle )
            {
                return INFINITY;

            }
    
        return std::abs(node1->x - node2->x) + std::abs(node1->y- node2->y);

        }
};



int main()
{

    std::cout <<"Dstarlite is now running\n";
    coordinates goal {10,10};
    DstarLite path_planner{goal};
    Node node1{1,1};
    node_pointer nodepointer_test = std::make_shared<Node>(Node{0, 0});
    int map_size {20*20*20};

    std::cout << "Size of node is " << sizeof(node1) << " bytes\n";    
    std::cout << "For a 20m x 20m with 0.05m in resolution it takes "<< sizeof(node1)*map_size/1024<<" Kbytes in node memory\n";

    
    std::cout << "Size of shared node pointer is is " << sizeof(nodepointer_test) << " bytes\n";    
    std::cout << "For a 20m x 20m with 0.05m in resolution it takes "<< sizeof(nodepointer_test)*map_size/1024<<" Kbytes in node pointer memory\n";

    std::cout << "For a 20m x 20m with 0.05m in resolution the total memory use is "<< (sizeof(nodepointer_test)*map_size + sizeof(nodepointer_test)*map_size)/1024 <<" Kbytes\n";

    
    return 0;
}