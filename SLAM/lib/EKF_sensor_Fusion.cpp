#include "../include/TrueMap.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <Eigen/Dense>

/*
This class is a implementation of a 2D diffrential drive robot

mu = {x y theta}.T 
b = {v_left,v_right}.T
y = {x y theta}.T 

z = unknown, this is for sensor fusion


theta = arctan(ax/az)
theta(k) = theta(k-1) - w_y*delta_t

psi = atan(ay/az)
psi(k) = psi(k-1) - w_x*delta_t

*/

class EKF
{
    // the origin for this filter
    // https://sparxeng.com/blog/software/imu-signal-processing-with-kalman-filter
    // do i assume input known? 
    // do i assume dimensions known?
    // do i assume element type?
    // tailord for my purpose so i think i do 
    
    public:
    // Input ax,ay,az,wx,wy,wz

    //only need yaw so... 
    // Accelerometer measurments
    float a_x {};
    float a_y {};
    float a_z {};

    float w_x {};
    float w_y {};
    float w_z {};


    // state and covariance
    float theta {};
    float sigma_theta {};


    float theta_hat {};

    // noise 
    float const theta_Q {0.1};
    float const theta_R {0.1};

    float K {0};
    float const delta_t {0.1}; // needs to be set at the polling speed
    float innovation {};
   
    void getSensorData()
    {
        // gets the sensordata and puts it into the correct variables
    }
    float estimate_theta()
    {
        getSensorData();
        // predict step
        theta_hat = theta - w_y*delta_t; 
        sigma_theta = sigma_theta + theta_Q;

        //Kalman gain
        K = sigma_theta/(sigma_theta + theta_R);

        // prediction 
        innovation = atan2(a_x,a_z) - theta_hat; // might need to double check atan2 for which element is where
        theta = theta_hat + K * innovation;
        sigma_theta = sigma_theta + K * sigma_theta;
        return theta;
    }




    private:
    // prob half the functions should be here and just estimate be callable
    // also probably have an odometry class here. 
    // should i try to have test driven development here? 

};
