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
Inputs:
mu = [x ,y ,theta].T 
Sigma = 3 X 3
u = [x y theta].T 
z = the measurements [range,angle,]
c = corresponcance variables
m  = map
F = 3N+3 X 3

Output:
mu
sigma
p = likelihood of the fature observations
*/
class Landmark
{
    public:
    int x;
    int y;
};

class EKF_localization
{
private:
    /* data */
public:
    EKF_localization(/* args */);
    ~EKF_localization();

    // mu separated for now
    float x {0}; 
    float y {0};
    float theta {0};

    float sigma {} ;// 3 X 3 


    float v {};
    float w {};
    float correspondence {0};

    const float Q {0.1};
    const float R {0.1};
    const float dt {0.1};


    bool measurement_flag {false}; // True if a new measurement is in.

    float G()
    {
    return 0.0;
    }
    float V()
    {
        return 0.0;

    }
    float M()
    {
    return 0.0;
        
    }
    float g()
    {
        return 0.0;

    }


    std::pair<float,float> update()
    {   float mu = mu;
        float curr_G =G() ;
        float curr_V = v;
        float curr_g = g();
        float curr_M = M();
        float movment_constant = v/w;
        x = x +(-v/w)*sin(theta) + (v/w)*sin(theta+w*dt);
        y = y + (v/w)*cos(theta) - (v/w)*cos(theta+w*dt);
        theta = theta + w*dt;

        //sigma = curr_G*sigma*curr_G.T + curr_V*curr_M*curr_V.T;
        return std::pair(mu,sigma);
        

    }

    std::vector<float> localize()
    {
        std::pair<float,float> updated_values = update();
        if (measurement_flag == true)
        {

        }
    }

};

EKF_localization::EKF_localization(/* args */)
{
}

EKF_localization::~EKF_localization()
{
}
