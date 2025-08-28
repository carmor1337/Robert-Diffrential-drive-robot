cmake_minimum_required(VERSION 3.10)
project(SLAM)

# Create executables
add_executable(SLAM_exec src/main.cpp)
add_executable(GUI src/GUI.cpp)

# C++ standard
target_compile_features(SLAM_exec PRIVATE cxx_std_17)
target_compile_features(GUI PRIVATE cxx_std_17)

# Find dependencies
# set(SFML_DIR "C:/SFML/lib/cmake/SFML")  # Optional if SFML not auto-detected
find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics audio)
find_package(Eigen3 3.3 REQUIRED)

# Include and link
add_subdirectory(lib)

target_include_directories(SLAM_exec PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(GUI PRIVATE ${CMAKE_SOURCE_DIR}/include)

set(COMMON_LIBS SLAM_lib sfml-graphics sfml-window sfml-system Eigen3::Eigen)

target_link_libraries(SLAM_exec PRIVATE ${COMMON_LIBS})
target_link_libraries(GUI PRIVATE ${COMMON_LIBS})
