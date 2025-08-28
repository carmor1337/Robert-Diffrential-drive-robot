#set(EIGEN3_INCLUDE_DIR "/usr/include/eigen3")

#include_directories(${EIGEN3_INCLUDE_DIR})

find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics audio)
find_package (Eigen3 3.3 REQUIRED)

add_library(SLAM_lib 
            Circular_beam_sensor.cpp
            Odometry.cpp
            TrueMap.cpp
            KalmanFilter.cpp
            EKF_sensor_Fusion.cpp
            EKF_localization.cpp
            SLAM_GUI.cpp
            Robot.cpp)

target_include_directories(SLAM_lib 
            PUBLIC
                ${CMAKE_SOURCE_DIR}/SLAM/include
                ${EIGEN3_INCLUDE_DIR}
        )
#message(STATUS "SLAM include dir: ${CMAKE_SOURCE_DIR}/include")

