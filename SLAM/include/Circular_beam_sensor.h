#ifndef CIRCULAR_BEAM_SENSOR_H
#define CIRCULAR_BEAM_SENSOR_H



#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <unordered_map>
#include <cmath>
#include <memory>


class Robot; // forward declaration

//#pragma once

class Circular_beam_Sensor
{
private:
    int num_of_beams;
    float degrees_per_beam;
    const float max_beam_dist {10};
    const float min_beam_dist {0};
    std::shared_ptr<Robot> robert;

    std::unordered_map<int, float> angle_lookup;

    void add_angles_to_angle_lookup();

public:
Circular_beam_Sensor(Robot* robert, int num_beams);

    std::vector<bool> what_beams_hit();
    bool did_beam_hit(int id);
};


#endif // MACRO