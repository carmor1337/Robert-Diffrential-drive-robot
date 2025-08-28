#include "SLAM_GUI.h"
#include "Robot.h"
#include "Circular_beam_sensor.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_map>


Circular_beam_Sensor::Circular_beam_Sensor( Robot* robot, int num_beams)
    : robert(robot),
      num_of_beams(num_beams),
      degrees_per_beam(360.0f / num_beams),
      max_beam_dist(10.0f),
      min_beam_dist(0.0f)
{
    std::cout << "Circular beam sensor initialized\n";
    add_angles_to_angle_lookup();
}

void Circular_beam_Sensor::add_angles_to_angle_lookup()
{
    float angle = 0;
    for (int i = 0; i < num_of_beams; i++)
    {
        angle_lookup[i] = angle;
        angle += degrees_per_beam;
    }
}

std::vector<bool> Circular_beam_Sensor::what_beams_hit()
{
    std::vector<bool> hits(num_of_beams, false);
    for (int i = 0; i < num_of_beams; i++)
    {
        hits[i] = did_beam_hit(i);
    }
    return hits;
}

bool Circular_beam_Sensor::did_beam_hit(int id)
{
    std::pair<float, float> pos = robert->get_coordinates();
    float x = pos.first;
    float y = pos.second;

    float max_x = x + max_beam_dist * cos(angle_lookup[id]);
    float max_y = y + max_beam_dist * sin(angle_lookup[id]);

    float min_x = x + min_beam_dist * cos(angle_lookup[id]);
    float min_y = y + min_beam_dist * sin(angle_lookup[id]);

    // Convert to integer grid coordinates for the map
    int x0 = static_cast<int>(std::round(x));
    int y0 = static_cast<int>(std::round(y));
    int x1 = static_cast<int>(std::round(max_x));
    int y1 = static_cast<int>(std::round(max_y));

    // Bresenham's line algorithm to find which grid cells the beam intersects
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        // Check if the current grid cell contains an obstacle in the map
        if (robert->robot_map->isInObstacle(x0, y0)) {
            return true;  // Beam hits an obstacle
        }

        // If the end point is reached, break the loop
        if (x0 == x1 && y0 == y1) {
            break;
        }

        // Bresenham's step to next cell
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    return false;  // Beam didn't hit anything
}
