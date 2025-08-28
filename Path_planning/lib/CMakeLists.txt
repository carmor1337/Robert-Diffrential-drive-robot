find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics audio)
find_package (Eigen3 3.3 REQUIRED)

add_library(Path_planning_headers INTERFACE)
target_include_directories(Path_planning_headers INTERFACE ${CMAKE_SOURCE_DIR}/Path_planning/include)


add_library(Path_planning_lib 
            Node.cpp
            DstarLite.cpp)

target_link_libraries(Path_planning_lib 
        PUBLIC Path_planning_headers Eigen3::Eigen  SLAM_lib
        )
