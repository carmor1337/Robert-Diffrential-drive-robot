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

*/

class EKF
{
    // do i assume input known? 
    // do i assume dimensions known?
    // do i assume element type?
    // tailord for my purpose so i think i do 
    
    public:
    //coordinates
    float x {};     // in map coordinates
    float y {};     // in map coordinates
    float theta {}; // in map coordinates

    Eigen::Vector3f mu {};      
    Eigen::Matrix3f sigma {};   
    float K {};

    Eigen::Matrix3f processNoise {};
    Eigen::Matrix2f measurementNoise {};

    float innovation {}; // might not be needed, but being on the safe side here

    // preprocessing
    void getAndUpdateSensorData() // might be a call to the sensor fusion class. should atleast get the change in heading
    {
        // get odometrydata in order to sync with IMU
        // might just need to call odometry when IMU is ready to give data
        // or it might be the other way around
        // might need a kalman filter to ensure that these values are ok
        // currently only localiszation, but might need to integrate range finder and Lidar scans in the future

    }; 

    void measurementModel()  // to be for a classic 2 wheeled robot
    {
        // might need to call for sensordata here? or do i just have a buffer to access? 
        // currently only to localize
        // Will need to improve once the Lidar is in play and the EKF slam is real
    }; 
    void jacobian()
    {

    }; 

    void sensorModel()
    {

    }; // IMU data and encoder data to be used together

    void predict(){};
    void calculateKalmanGain(){};
    void update(){};
    

    


    void estimate()
    {
        // Get sensor data i guess
        // do the updates step
        // do the kalman gain calculation
        // do the update step
        // return the new x,y,theta to the main function
    };



    private:
    // prob half the functions should be here and just estimate be callable
    // also probably have an odometry class here. 
    // should i try to have test driven development here? 

};
