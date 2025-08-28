#ifndef TRUEMAP_H
#define TRUEMAP_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>



class TrueMap
{
private:
    float width {20};   // in meters
    float height {20};  // in meters
    float resolution {0.05};  // in meters, cannot be less than

    int num_cells_x ;
    int num_cells_y ;
    int map;
    std::vector<std::vector<int>> allowed_values = {{0, 1, 2, 3, 10, 20, 100}};



public:
    std::vector<std::vector<int>> map_data; 
    std::vector<std::pair<int, int>> path;

    TrueMap(); // Default constructor
    TrueMap(float map_width, float map_height, float res, int map_select= 0);
    
    int size {num_cells_x};
    // Function to get a value at (x, y)
    int getValueAt(int x, int y) const;

    // Function to set a value at (x, y)
    void setValueAt(int x, int y, int value);

    // Function to print the grid
    void printMap() const;

    // Function to add an obstacle at (x, y)
    void addObstacle(int x, int y);

    void addObstacleHidden(int x, int y) ;

    // Function to remove an obstacle at (x, y)
    void removeObstacle(int x, int y);

    // Function to add a square obstacle with top-left corner at (x, y) and size
    void addSquareObstacle(int x, int y, int size);

    // Function to remove a square obstacle with top-left corner at (x, y) and size
    void remove_square_obstacle(int x, int y, int size);

    void addRetangularObstacle(int x, int y, int length, int width);

    // Function to add rectangular obstacles in a diagonal across the map
    void diagnoalSquaresMap();

    void labyrintMap();

    void LmiddleObstacle();

    // Function to print the map size (2D)
    void printMapSize2D();

    // Function to print the map size (1D)
    void printMapSize1D();

    // Function to check if a value is allowed
    bool isInBounds(int x, int y);

    // Function to check if a point is in an obstacle
    bool isInObstacle(int x, int y) const;

    // Function to check if a point is in bounds and not in an obstacle
    bool isInBoundsAndNotObstacle(int x, int y);

    std::vector<std::pair<std::pair<int,int>,int>> reveal_map(const TrueMap& map, int x, int y, int reveal_size);

    void set_map(int map_selector);


};
#endif // TRUEMAP_
