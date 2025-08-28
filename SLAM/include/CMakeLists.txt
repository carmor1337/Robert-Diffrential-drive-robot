#add_library(SLAM_lib)

#target_include_directories(SLAM_lib
#        INTERFACE
#        ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(SLAM_lib 
PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${EIGEN3_INCLUDE_DIR}
)