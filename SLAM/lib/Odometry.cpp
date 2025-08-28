#include "../include/Odometry.h"
 
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <thread>


// w(R+d = vr
//w(R-D) = vl

//w = (vr-vl)/2d
//R = d(vr+vl)/vr-vl
//V = wR
//vw_i = 2pi*r*f*DeltaEncoder/tics_per_rev


// sole task of odometry class is to hold robot specific values and give odometry data at a resonable pace




void Odometry::calculate_v_w()
    {
        v_tot =(r*K)/2 * (E_right+E_left);
        w =(r*K)/B * (E_right-E_left);
    }

void Odometry::calculate_coordinates()
    {
        x = x + v_tot * dt * cos(theta);
        y = y + v_tot * dt * sin(theta);
        theta = theta + w*dt;
    }


void Odometry::print_coordiantes()
    { 
        std::cout << "\nPrinting coordinates (x,y,theta): (" << x <<", " << y <<", " << theta << ")\n";

    }


