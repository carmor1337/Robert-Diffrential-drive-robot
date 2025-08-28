#ifndef POINT_TO_POINT
#define POINT_TO_POINT


#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <memory>

#include "Node.h"

using node_pointer = std::shared_ptr<Node>;
using coordinate  = std::pair<int,int> ;

struct Velocities
{
    float linear_velocity {};
    float angular_velocity {};
};

class Point_to_Point
{
private:
    std::vector<node_pointer> path {};
    coordinate current_position {}; 

    node_pointer get_next_point();
    Velocities calculate_velocities(coordinate new_point);

public:
    Point_to_Point(/* args */);
    ~Point_to_Point();

    void follow_path();
    coordinate get_currrent_position();

};

Point_to_Point::Point_to_Point(/* args */)
{
}

Point_to_Point::~Point_to_Point()
{
}

#endif