#ifndef ROBOT_H
#define ROBOT_H

class TrueMap;
//class Circular_beam_Sensor; // ✅ Forward declaration
//#include "Robot.h"
#include <Circular_beam_sensor.h>

#include <memory>
#include <utility>

class Robot
{
private:
    // Member variables
    // temporarily ints to integrate into the GUI
    int x {0};                     // Position x
    int y {0};                     // Position y
    float controller {};             // Controller state
    float SLAM {};                   // SLAM state
    float behaviour_tree {};         // Behaviour Tree state
    float path_manager {};           // Path manager state
    
    //std::unique_ptr<Circular_beam_Sensor> sensor; // ✅ Smart pointer to component

public:
    // Constructor (optional, if needed)
    Robot(const TrueMap& map); // ✅ Use const-ref to avoid copy

    std::shared_ptr<TrueMap> robot_map; // ✅ Shared if others access it too
    //Circular_beam_Sensor* sensor = nullptr; // pointer, initialized in constructor
    // Member functions
    void set_coordinates(int new_x, int new_y);
    std::pair<int,int > get_coordinates();


    void sense_gui();  // Sense the environment using the sensor
    void plan();
    void act();
    void move_GUI(unsigned int dx, unsigned int dy);
    void update();
    


};
#endif // MACRO