#ifndef SLAM_GUI_H
#define SLAM_GUI_H

#include "../include/TrueMap.h"
#include "../include/SLAM_GUI.h"
#include "../include/Circular_beam_sensor.h"
#include "../include/Robot.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <unordered_map>
#include <math.h>


class SLAM_GUI {
    public:
        SLAM_GUI();  // Constructor declaration
        //TrueMap truemap();
        float map_width = 10;
        float map_length = 10;
        float resolution = 0.5;
        int map_selected = 0;

        TrueMap truemap = TrueMap(map_width,map_length,resolution,2);
        
        Robot robert = Robot(truemap);

        int xStart {0};
        int yStart {0};
        int xGoal {truemap.size -1};
        int yGoal {truemap.size -1};
        
        void run();
        void renderMap();
        void fill_cell(int &value,sf::RectangleShape &cell);

        void initialize(int maptype);
        void set_start_position(int start_x,int start_y);
        //std::vector<std::pair<int,int>> run_path(int StartX,int StartY,int GoalX,int GoalY);
        //void produceAndRenderPath(std::vector<std::pair<int,int>> path);
        
        void move_robot(unsigned int add_x,unsigned int add_y);
        void move_robot_up();
        void move_robot_left();
        void move_robot_down();
        void move_robot_right();
        void move_robot_up_left();
        void move_robot_up_right();;
        void move_robot_down_right();
        void move_robot_down_left();




    private:
         //sf::Window window;  // SFML window
        sf::RenderWindow renderWindow;  // Used for drawing the shapes
        int cellSize;  // Size of each cell in the grid
    };


#endif



