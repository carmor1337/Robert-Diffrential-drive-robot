#include "../include/TrueMap.h"
#include "../include/SLAM_GUI.h"
#include "../include/Circular_beam_sensor.h"
#include "../include/Robot.h"

#include <iostream>
#include <vector>
#include <assert.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <unordered_map>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


SLAM_GUI::SLAM_GUI() : 
    //window(sf::VideoMode(800, 600), "PahFinding Window"), 
    renderWindow(sf::VideoMode(1000, 800), "SLAM Visualization"),
    cellSize(2)
    {
        std::cout<< "SLAM GUI class created\n";
    }

void SLAM_GUI::run() {
    initialize(map_selected); // initalized the diagonal map
    // create a window based on the size of the cells

    
    unsigned int windowWidth = robert.robot_map->map_data[0].size() * cellSize;
    unsigned int windowHeight = robert.robot_map->map_data.size() * cellSize;

    renderWindow.create(sf::VideoMode(windowWidth, windowHeight), "SLAM Visualization");
    std::cout<< "SLAM GUI window created\n";


    while (renderWindow.isOpen()) {
        sf::Event event;
        // Process events
        while (renderWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderWindow.close();  // Close the window when the user closes it
            }

            // Needs to come first 
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Q)
                ||
                (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W) && (event.key.code == sf::Keyboard::A)  
            )
            {
                move_robot_up_left();

            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::E)
            ||
            (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W) && (event.key.code == sf::Keyboard::D)  
        )
            {
                move_robot_up_right();

            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::C)
            ||
            (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S) && (event.key.code == sf::Keyboard::D)  
        )
            {
                move_robot_down_right();

            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Z)
            ||
            (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S) && (event.key.code == sf::Keyboard::A)  
        )
            {
                move_robot_down_left();

            }

            // Regular button press
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W))
            {
                move_robot_up();

            }
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A))
            {
                move_robot_left();

            }
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
            {
                move_robot_down();

            }
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::D))
            {
                move_robot_right();

            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Q)
                ||
                (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W) && (event.key.code == sf::Keyboard::A)  
            )
            {
                move_robot_up_left();
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::E)
            ||
            (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W) && (event.key.code == sf::Keyboard::D)  
        )
            {
                move_robot_up_right();

            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::C)
            ||
            (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S) && (event.key.code == sf::Keyboard::D)  
        )
            {
                move_robot_down_right();

            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Z)
            ||
            (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S) && (event.key.code == sf::Keyboard::A)  
        )
            {
                move_robot_down_left();

            }

            // if event is move one step we move one step
        }
        
        auto coordinates = robert.get_coordinates();
        int x = coordinates.first;
        int y = coordinates.second;

        auto map_changes = robert.robot_map->reveal_map(truemap,x,y,4);
        //window.clear();  // Clear the window
        renderWindow.clear(sf::Color::Black);
        
        renderMap();

        renderWindow.display();

        //abort();

    }
}

void SLAM_GUI::initialize(int maptype)
{   int start_x = 0;
    int start_y = 0;
    
    set_start_position(start_x,start_y);

    // 0 is a map with diagonal type of obstacles
    if (maptype == 0)
    {
        //robert.robot_map.diagnoalSquaresMap();
        truemap.diagnoalSquaresMap();
        
        std::cout << "Map initalized to diagonal\n";
        
    }
    // 1 is a map similar to maze or labyrinth
    if (maptype == 1)
    {
        robert.robot_map->labyrintMap();
        //truemap.labyrintMap();
        std::cout << "Map initalized to labyrinth\n";

    }
    if (maptype == 2)
    {
        robert.robot_map->LmiddleObstacle();
        //truemap.labyrintMap();
        std::cout << "Map initalized to L obstacle\n";

    }
    std::cout << "GUI initialize goes here\n";

}
   
void SLAM_GUI::renderMap() 
{   // renderorder has to be 
    // 1. Free space
    // 2. Path
    // 3. reveal
    // 4. obstacle
    // 5. robot
    for (size_t i = 0; i < robert.robot_map->size; ++i) {

        for (size_t j = 0; j < robert.robot_map->map_data.size(); ++j) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(j * cellSize, i * cellSize);

            /* Set color based on the map value:
            0 for walkable,
            1 for obstacle)
            2 for visited
            3 for path
            10 for goal
            20 for start
            100 for robot
            
           sf::Color Grey(192, 192, 192);
           sf::Color pathYellow ( 204,102,0);
           */
           int robot_value = robert.robot_map->map_data[i][j];
           int true_value = truemap.getValueAt(i,j);
           fill_cell(robot_value, cell);
           //fill_cell(true_value,cell);
            renderWindow.draw(cell);  // Draw the cell
        }
    }

}

void SLAM_GUI::fill_cell(int &value,sf::RectangleShape &cell)
{   
 // might be better to initialize them all in a hash table 
 // and then just query the table. 
 // might be a later optimization


 // Draw operation is
 // Robot
 // Obstacle
 // Start
 // Goal
 // Path
 // map reveal
 // visited
 // Free space


    /* Set color based on the map value:
            0 for walkable,
            1 for obstacle)
            2 for visited
            3 for path
            5 for map reveal
            10 for goal
            20 for start
            100 for robot
            */
    sf::Color BackgroundGrey(192, 192, 192);
    sf::Color Hidden_obstacle_grey (96,96,96);
    sf::Color pathYellow ( 204,102,0);
    sf::Color mapVisableOrange(204,102,0);
    if (value == 0) 
    {
        cell.setFillColor(BackgroundGrey);  // Walkable area
    } 
    else if (value == 1) 
    {
        cell.setFillColor(sf::Color::Black);    // Obstacle area
    }
    else if (value == 2) 
    {
        cell.setFillColor(sf::Color::Magenta);    // Visited
    }
    else if (value == 3) 
    {
        cell.setFillColor(pathYellow);    // Path
    }
    else if (value == 5) 
    {
        cell.setFillColor(mapVisableOrange);    // map_reveal size
    }
    else if (value == 10) 
    {
        cell.setFillColor(sf::Color::Green);    // Goal
    }
    else if (value == 20) 
    {
        cell.setFillColor(sf::Color::Blue);    // Start
    }
    else if (value == 100) 
    {
        cell.setFillColor(sf::Color::Red);    // Robot
    }
    else {
        abort();
    }
} 
 
void SLAM_GUI::set_start_position(int start_x,int start_y)
{
    robert.set_coordinates(start_x,start_y); 
}


void SLAM_GUI::move_robot(unsigned int dx,unsigned int dy)
{
    robert.move_GUI(dx,dy);
    auto coordinates = robert.get_coordinates();
    int x = coordinates.first;
    int y = coordinates.second;

    robert.robot_map->reveal_map(truemap,x,y,4);
    //else {robert.robot_map.setValueAt(x,y,100);}

}
void SLAM_GUI::move_robot_up()
{
   move_robot(0,-1);

}
void SLAM_GUI::move_robot_left()
{
   move_robot(-1,0);

}
void SLAM_GUI::move_robot_down()
{
   move_robot(0,1);

}
void SLAM_GUI::move_robot_right()
{
   move_robot(1,0);

}
void SLAM_GUI::move_robot_up_left()
{
   move_robot(-1,-1);

}
void SLAM_GUI::move_robot_up_right()
{
   move_robot(1,-1);

}
void SLAM_GUI::move_robot_down_right()
{
   move_robot(1,1);

}
void SLAM_GUI::move_robot_down_left()
{
   move_robot(-1,1);

}


