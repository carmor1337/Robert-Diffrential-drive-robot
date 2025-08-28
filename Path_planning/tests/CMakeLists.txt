find_package(GTest REQUIRED)

add_executable(PathPlannerTests 
                test_node_behaviour.cpp
                test_Dstarlite_functions.cpp)

target_include_directories(PathPlannerTests PRIVATE ${GTEST_INCLUDE_DIRS})

target_compile_features(PathPlannerTests PRIVATE cxx_std_17)

target_link_libraries(PathPlannerTests PRIVATE Path_planning_lib GTest::gtest_main)

add_test(NAME PathPlannerTests COMMAND PathPlannerTests)
