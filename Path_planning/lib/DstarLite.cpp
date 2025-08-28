#include "Robot.h"
#include <TrueMap.h>
#include "Node.h"
#include "DstarLite.h"


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

// ################################ Main functions #############################

// constructor
DstarLite::DstarLite(coordinates init_start, coordinates init_goal) 
    : map(10, 10, 1, 5), 
      robert(map), 
      goal_ints(init_goal)
    {    
        goal_node = std::make_shared<Node>(goal_ints.first,goal_ints.second);
        current_node_p = std::make_shared<Node>(init_start.first,init_start.second);

        initialize();
    }

    // initialize all the nodes to g=rhs=INFINITY for all cell in map, but NOT the keys
void DstarLite::initialize()
{
    key_m = 0.00;
    //std::cout<< robert.robot_map->size<<std::endl;
    for (int i = 0; i<robert.robot_map->size;i++)
    {

        for (int j = 0; j<robert.robot_map->size; j++)
        {   
            if (i == goal_node->x && j == goal_node->y)
            {   
                goal_node->rhs = 0;

                goal_node->key = calculate_key(goal_node);
                node_map[{goal_node->x, goal_node->y}] = goal_node;

                open_list.push(goal_node);
            }
            else if (i == current_node_p->x && j == current_node_p->y)
            {
                node_map[{current_node_p->x, current_node_p->y}] = current_node_p;
            }
            else 
            {   //creates a node and put it into the hashmap
                node_pointer node = std::make_shared<Node>(i,j);
                node_map[{node->x, node->y}] = node;
            }
        }
    }  
}

key DstarLite::calculate_key(node_pointer node_p)
    {   
        coordinates coords = robert.get_coordinates();
        int x = coords.first;
        int y = coords.second;      
        
        double h = dist(node_p, current_node_p);       
        double min_g_rhs = std::min(node_p->g, node_p->rhs); // or dist to s_start

        key out_key = {min_g_rhs + h + key_m, min_g_rhs};
        return out_key;
    }
   

void DstarLite::update_vertex(node_pointer node_p)
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

    // Computes the shortest valid path: optimized version
void DstarLite::compute_shortest_path()
    {   
        while (shortest_path_not_done())
        
        //for (int i = 0; i<11; i++)
        {   bool tmp = shortest_path_not_done();
            //std::cout << "\nShortest path done? " << tmp<< std::endl;

            
            //std::cout<< " Look at me gooo!\n";
            node_pointer node_p = pop_open_list();
            key key_old = node_p->key;
            key key_new = calculate_key(node_p);

            //std::cout<< "\nPopped nodes x "<< node_p->x << " and y "<< node_p->x <<std::endl;
            //std::cout<< "Popped nodes g "<< node_p->g << " and rhs "<< node_p->rhs <<std::endl;
            //std::cout << "open list size is "<< open_list.size()<<std::endl<<std::endl; 
            
            
            if (key_old < key_new)
            {
                push_open_list(node_p);
                //std::cout<< " Do i push to open list?!\n";

            }
            else if (node_p->g > node_p->rhs)
            {
                //std::cout<< " Do i try to find neighbours?!\n";

                node_p->g = node_p->rhs;
                // for all predecessors of node_p: update vertex
                auto neighbour_list = get_neighours(node_p);
                //std::cout<< "Neighbourlist size is "<< neighbour_list.size() <<std::endl;
                for (auto neighbour_p: neighbour_list)
                {
                    if (neighbour_p!= goal_node )
                    {
                    //std::cout<< "distance is  "<< dist(node_p,neighbour_p) + node_p->g<<std::endl;
                    neighbour_p->rhs = std::min(neighbour_p->rhs, dist(node_p,neighbour_p) + node_p->g);
                   // std::cout<< "neighbour (x,y), (rhs, g) ("<< neighbour_p->x<<", "<<neighbour_p->y<<"), (" <<neighbour_p->rhs <<", "<< neighbour_p->g<<")"<<std::endl;

                    }   
                    update_vertex(neighbour_p);
                }
            }
            else
            {
               // std::cout<< " Did i enter else?\n";

                double g_old = node_p->g;
                node_p->g = INFINITY;

                auto neighbour_list = get_neighours(node_p);
                neighbour_list.push_back(node_p);
                //std::cout<< "Neighbourlist size is "<< neighbour_list.size() <<std::endl;

                for (auto neighbour_p: neighbour_list)
                {
                    std::cout<< "In else neighbour rhs vs dist(neighbour,node) + g_old "<< neighbour_p->rhs << ", " << dist(neighbour_p,node_p) + g_old<< std::endl;

                    if (neighbour_p->rhs == dist(neighbour_p,node_p) + g_old)
                    {
                        if (neighbour_p != goal_node)
                        {   

                            auto neighbour_neighbours_list = get_neighours(neighbour_p);
                            //std::cout<< "Neighbour_neighbour_list size is "<< neighbour_neighbours_list.size() <<std::endl;

                            auto min_node = find_min_successor(neighbour_neighbours_list,neighbour_p);
                            //std::cout<< " Do i pass the find_min_successor?\n";
                            if (!min_node) {
                                std::cout << "min_node is nullptr — skipping this neighbor\n";
                                continue;  // or return, or safely skip the rest
                            }

                            //std::cout<< " min node is "<< min_node->x<< " "<< min_node->y<<std::endl;
                            
                            neighbour_p->rhs = dist(neighbour_p, min_node) + min_node->g;
                        }
                    }

                    update_vertex(neighbour_p);

                }
            }
        }
    }

void DstarLite::dstar_main()
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
                        else if (change.second == 0) {node_p->in_obstacle = false;}

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


    node_pointer DstarLite::find_min_successor(std::vector<node_pointer> neighbours,node_pointer current_node_p)
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
    std::vector<node_pointer> DstarLite::get_neighours(node_pointer current_node)
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
        
    node_pointer DstarLite::pop_open_list()
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

    void DstarLite::push_open_list(node_pointer node_p)
    {   
        // calculates the node key
        // set the node key inside
        // push onto the open list with the key value

        key new_key = calculate_key(node_p);
        node_p->key = new_key;
        if (node_p->in_queue == true)
        node_p->in_queue = true;
        open_list.push(node_p);
    }


    std::vector<std::pair<coordinates,int>>  DstarLite::scan_for_changes()
    {
        // gets a return message if anything in map has changed and where it has changed into what
        // should resonable only return with the obstacle value.
        std::vector<std::pair<std::pair<int,int>,int>> changes = robert.robot_map->reveal_map();
        
        
        return changes;
    }

    // checks the while loop condition for the shortest path while loop
    /*bool DstarLite::shortest_path_not_done()
    {
        if (open_list.empty()) return false; // edge case safety
    
        node_pointer top_node = open_list.top();
        return (top_node->key < calculate_key(current_node_p)) ||
                (current_node_p->rhs > current_node_p->g);
    }*/
    bool DstarLite::shortest_path_not_done()
    {
        if (open_list.empty()) return false;
    
        auto top_node = open_list.top();
        auto top_key = top_node->key;
        auto curr_key = calculate_key(current_node_p);
        auto rhs = current_node_p->rhs;
        auto g = current_node_p->g;
    
        //std::cout << "top_key: (" << top_key.first << ", " << top_key.second << ") ";
        //std::cout << "curr_key: (" << curr_key.first << ", " << curr_key.second << ") ";
        //std::cout << "rhs: " << rhs << ", g: " << g << std::endl;
    
        return (top_key < curr_key) || (rhs > g);
    }
    
    double DstarLite::dist(const node_pointer node1 , const node_pointer node2 )
        {
            if (node1->in_obstacle || node2->in_obstacle )
            {
                return INFINITY;

            }
    
            return std::sqrt((node1->x - node2->x) * (node1->x - node2->x) + 
                            (node1->y - node2->y) * (node1->y - node2->y));

        }

