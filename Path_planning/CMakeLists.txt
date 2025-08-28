cmake_minimum_required(VERSION 3.10)
project(Path_planning)


# Find dependencies
find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics audio)
find_package(Eigen3 3.3 REQUIRED)

# Add your own library
add_subdirectory(lib)
add_subdirectory(tests)    # Tests that depend on the libs

# Create executable
add_executable(DstarLite_exec src/DstarLite_main.cpp)


# C++ standard
target_compile_features(DstarLite_exec PRIVATE cxx_std_17)

# Include directory for your own headers
#target_include_directories(DstarLite_exec PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link libraries
set(COMMON_LIBS 
    Path_planning_lib
    SLAM_lib 
    sfml-graphics
    sfml-window
    sfml-system
    Eigen3::Eigen)

target_link_libraries(DstarLite_exec PRIVATE ${COMMON_LIBS})