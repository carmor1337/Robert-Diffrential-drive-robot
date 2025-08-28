add_library(Path_planning_headers INTERFACE)

target_include_directories(Path_planning_headers
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}  # this is the include folder path
)