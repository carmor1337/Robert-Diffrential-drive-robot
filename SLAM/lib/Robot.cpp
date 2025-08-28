#include "SLAM_GUI.h"
#include "TrueMap.h"
#include "Circular_beam_sensor.h"
#include "Robot.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <unordered_map>
#include <math.h>
using coordinates = std::pair<int,int>;


// Constructor definition (optional)
Robot::Robot(const TrueMap& map) {
    robot_map = std::make_shared<TrueMap>(map); // Shared so components can read it
    //sensor = std::make_unique<Circular_beam_Sensor>(this, 12); // Pass pointer to self
    //std::cout << "Robot constructed\n";

}
    //sensor = new Circular_beam_Sensor(this, 10); // pass pointer to this


// Method to get coordinates of the robot
std::pair<int,int> Robot::get_coordinates()
{
    return std::make_pair(x, y);
}
void Robot::set_coordinates(int new_x, int new_y )
{
    x = new_x;
    y = new_y;
}
// Method to sense the environment using the sensor
void Robot::sense_gui()
{
    std::cout<< "in sense GUI, do i run?\n";
    
coordinates current_coordiantes = get_coordinates();
int x_curr = current_coordiantes.first;
int y_curr = current_coordiantes.second;
int reveal_size {5};
robot_map->reveal_map(*robot_map,x_curr,y_curr,reveal_size);

}
void  Robot::plan(){

}
void Robot::act(){

}
void Robot::move_GUI(unsigned int dx, unsigned int dy){
     // set current square to visited
     std::pair<int,int> coordinates = get_coordinates();
     int x = coordinates.first;
     int y = coordinates.second;
 
 
     x = x + dx;
     y = y + dy;
     if (robot_map->isInBoundsAndNotObstacle(x,y))
     {
        robot_map->setValueAt(coordinates.first,coordinates.second,2);
        set_coordinates(x,y);
        robot_map->setValueAt(x,y,100);
 
         
     }

}
void Robot::update(){
    
}

