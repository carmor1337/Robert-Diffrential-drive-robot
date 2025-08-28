#ifndef ODOM_H
#define ODOM_H

#include "../include/Odometry.h"

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <thread>

// TODO: figure out E_right and E_left
class Odometry
{
    public:
    const float r {0.033};      // in m 33 mm
    const float B {0.05};       // in m, half the distance between the wheels
    const float dt {0.1};       // in seconds
    const float hertz {10};      // in seconds Only here for reference
    const float K{};

    float E_right {};           //Encoder right  difference in encoder reading over a period of time dt
    float E_left  {};           //Encoder left   difference in encoder reading over a period of time dt
    
    float v_left {};            // sped of the left wheel
    float v_right {};           // sped of the right wheel
    float v_tot {};             // total heading speed
    float w {};                 // the angular velocity, counter clockwise
    



    float x{};
    float y{};
    float theta {};

    void calculate_v_w();

    void calculate_coordinates();


    void print_coordiantes();

    private:

};



#endif // MACRO
