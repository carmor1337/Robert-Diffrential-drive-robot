#include "../include/Odometry.h"
#include "../include/TrueMap.h"
 

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <thread>
#include <Eigen/Dense>

//using Eigen::Matrix2;
// TODO:
// Import eigen for the matrix operations

int main()
{   
    

    Eigen::Matrix2d mat ; 

    mat(0,0) = 0;
    mat(0,1) = 1;
    mat(1,0) = 2;
    mat(1,1) = 3;

    std::cout << mat<<"\n";
    
    /* this is basically a test function for truemap
    TrueMap true_map;
    TrueMap test_map;

    std::cout<< "the maps are generated\n";
    true_map.diagnoalSquaresMap();
    std::cout<< "the True map has generated obstacles\n";
    std::cout<< " value of 1,1 on true map: " << true_map.getValueAt(1,1) << "\n";

    std::cout<< " value of 1,1 on test map before map reveal: " << test_map.getValueAt(1,1) << "\n";

    test_map.reveal_map(true_map, 1,1,10);
    std::cout<< " value of 1,1 on test map after map reveal: " << test_map.getValueAt(1,1) << "\n";
    */
    return 0;
}