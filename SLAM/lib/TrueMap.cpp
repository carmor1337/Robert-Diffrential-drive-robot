#include "../include/TrueMap.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <chrono>
#include <thread>
#include <stdexcept>




/*
This class represents a 2D grid map.
It has the following attributes:
- width: width of the map in meters
- height: height of the map in meters
- resolution: resolution of the map in meters
- num_cells_x: number of cells in x direction
- num_cells_y: number of cells in y direction
- map_data: 2D vector to store the map data (0: free, 1: obstacle, 2: visited, 3: path, 10 goal, 20 start, 100: robot) 



*/

TrueMap::TrueMap()
{
    num_cells_x = static_cast<int>(width / resolution);
    num_cells_y = static_cast<int>(height / resolution);
    map_data = std::vector<std::vector<int>>(num_cells_y, std::vector<int>(num_cells_x, 0));
    size = num_cells_x;
}

TrueMap::TrueMap(float map_width, 
                float map_height, 
                float res, 
                int map_select)

                :width(map_width),
                height(map_height),
                resolution(res)

            {
                num_cells_x = static_cast<int>(width / resolution);
                num_cells_y = static_cast<int>(height / resolution);
                map_data = std::vector<std::vector<int>>(num_cells_y, std::vector<int>(num_cells_x, 0));
                size = num_cells_x;
                set_map(map_select);


            }


// Function to get a value at (x, y)
int TrueMap::getValueAt(int x, int y) const 
{
    if (x >= 0 && x < num_cells_x && y >= 0 && y < num_cells_y) {
        return map_data[y][x];  // Access 2D vector
    } else {
        
        throw std::out_of_range("Index out of bounds!");
    }
}

// Function to set a value at (x, y)
void TrueMap::setValueAt(int x, int y, int value) 
{
    if (x >= 0 && x < num_cells_x && y >= 0 && y < num_cells_y) {
        map_data[y][x] = value;  // Modify 2D vector
    } else {
        throw std::out_of_range("Index out of bounds!");
    }
}

// Function to print the grid in terminal
void TrueMap::printMap() const 
{
    for (const auto& row : map_data) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << '\n';
    }
}
// Function to add an obstacle at (x, y)
void TrueMap::addObstacle(int x, int y) 
{
    setValueAt(x, y, 1);
}

void TrueMap::addObstacleHidden(int x, int y) 
{
    setValueAt(x, y, 5);
}
// Function to remove an obstacle at (x, y)
void TrueMap::removeObstacle(int x, int y) {

    setValueAt(x, y, 0);
}
// Function to add a square obstacle with top-left corner at (x, y) and size
void TrueMap::addSquareObstacle(int x, int y, int size) 
{

    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            addObstacle(i, j);
        }
    }
}
void TrueMap::addRetangularObstacle(int x, int y, int length, int width)
{

    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + length; j++) {
            addObstacle(i, j);
        }
    }
}
// Function to remove a square obstacle with top-left corner at (x, y) and size
void TrueMap::remove_square_obstacle(int x, int y, int size) 
{
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            removeObstacle(i, j);
        }
    }
}
// Function to add rectangular obstacles in a diagonal across the map 
void TrueMap::diagnoalSquaresMap()
{   int size = num_cells_x/10;
    int x = 1;
    int y = 1;
    int offset = 1;
    //addSquareObstacle(x, y, size);
    //addSquareObstacle(x + size + offset, y + size+offset , size);
    //addSquareObstacle(x + 2*(size+ offset),  y + 2*(size+ offset), size);
    while (x < num_cells_x-1 && y < num_cells_y)
    {   if (x +size > num_cells_x-1 || y +size > num_cells_y-1)
        {
            break;
        }
        addSquareObstacle(x, y, size);
        x += (size + offset);
        y += (size + offset);
    }
    //std::cout<< "Diagonal square map generate" <<std::endl;
}

void TrueMap::labyrintMap()
{
    int DISTANCE_BETWEEN_LABYRINTH_WALLS {2};
    int DISTANCE_TO_EXTERIOR_WALLS {1};
    bool HORIZONTAL_WALLS {1}; // due to map starting in the top right corner
    bool VERTICAL_WALLS {!HORIZONTAL_WALLS}; // Its either one or the other
    bool open_flip {0}; // 0 is opening at the top, 1 is opening at the bottom

    for (int column=1;column <map_data.size(); column += DISTANCE_BETWEEN_LABYRINTH_WALLS)
    {
    
        // Modify all elements in the column
        for (int row = 0; row < map_data[column].size(); ++row)
        {
            map_data[column][row] = 1; // Modify individual elements, not the entire row
        }

        // add the opening to a line 
        if (open_flip == 0)
        {
            map_data[column][map_data.size() -1] = 0;
        }
        if (open_flip == 1)
        {
            map_data[column][0] = 0;

        }
        open_flip = !open_flip; // flipping the bool for the next row
    }
    std::cout<< "Labyrinth map generate" <<std::endl;


}   

void TrueMap::LmiddleObstacle()
{
    int MIDDLE ={static_cast<int>(num_cells_x / 2)};
    const int OBSTACLE_LENGTH = static_cast<int>(num_cells_x / 3);
    const int OBSTACLE_WIDTH = width;
    
    addRetangularObstacle(
        MIDDLE,
        MIDDLE,
        OBSTACLE_LENGTH,
        OBSTACLE_WIDTH
    );
    addRetangularObstacle(
        MIDDLE,
        MIDDLE,
        OBSTACLE_WIDTH,
        OBSTACLE_LENGTH
    );

    std::cout<< "Middle obstacle map generate" <<std::endl;

    
}


void TrueMap::printMapSize2D()
{
    std::cout << "Map size: " << num_cells_x << " x " << num_cells_y << '\n';
}
void TrueMap::printMapSize1D()
{
    std::cout << "Map size: " << map_data.size() << " x " << map_data[0].size() << '\n';
}

// Function to check if a value is allowed
bool TrueMap::isInBounds(int x,int y)  
{
    
    return (x >= 0 && x < num_cells_x && y >= 0 && y < num_cells_y);
}

bool TrueMap::isInObstacle(int x, int y) const
{   

    return (getValueAt(x, y) == 1);
}

bool TrueMap::isInBoundsAndNotObstacle(int x, int y) 
{

    return isInBounds(x, y) && !isInObstacle(x, y);
}

// 2 maps, on true map filled in and one unfilled map. the unfilled map calls this function to get reveals from the true map in a square of size reveal_size
std::vector<std::pair<std::pair<int,int>,int>> TrueMap::reveal_map(const TrueMap& true_map, int x, int y, int reveal_size)
{
    std::vector<std::pair<std::pair<int,int>,int>> map_changes_vector;
    // if x + reveal size > size -> 
    int x_iter {x + reveal_size};
    int y_iter {y + reveal_size};

    if (x_iter > size) { x_iter = size;} 
    if (y_iter > size) { y_iter = size;}

    for (int i = x; i < x_iter; i++) 
    {
        for (int j = y; j < y_iter; j++)
        {
            if (true_map.isInObstacle(i,j))
            {
                std::pair<int,int> coordinates {i,j};
                std::pair<std::pair<int,int> ,int> obstacle_change {coordinates,1};
                map_changes_vector.push_back(obstacle_change);
                this->addObstacle(i,j); // obstacle
            } 
            else
            {
                //this->setValueAt(i,j,5); // reveald
            }

        }

    }
    return map_changes_vector;
}

void TrueMap::set_map(int map_select)
{
    if( map_select == 0)
    {
        diagnoalSquaresMap();
    }
    else if( map_select == 1)
    {
        labyrintMap();
    }

    else if( map_select == 2)
    {
        LmiddleObstacle();
    }


}


