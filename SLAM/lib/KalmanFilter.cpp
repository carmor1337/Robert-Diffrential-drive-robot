
#include "../include/TrueMap.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <random>
#include <Eigen/Dense>






// use Eigen to set up the Kalman filter, i can atleast create a class and a header file to fill in later
// Use the simple gold example the ensure that things are ok


class KalmanFilter
{
    // do i assume input known? 
    // do i assume dimensions known?
    // do i assume element type?
    // tailord for my purpose so i think i do 
    
    public:
    float x {};     // in map coordinates
    float y {};     // in map coordinates
    float theta {}; // in map coordinates

    float mu {10};      // to be filled in as a matrix when eigen is installed
    float sigma {0.1};   // to be filled in as a matrix when eigen is installed
    float K {};

    float TrueValue {100};

    int processNoise {1};
    int measurementNoise {1};

    // preprocessing

    void motionModel(){}
    float measurementModel()
    {
        float val = 100; 
        val = val +rand();
        return val;
    }


    void predict(){};
    void calculateKalmanGain(){};
    void update()
    {
        
        
    };

    


    void estimate(){

        float z = measurementModel();
    };


    



    private:

    

};
