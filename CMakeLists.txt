cmake_minimum_required(VERSION 3.10)
project(OrderBook)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

# Define the executable
add_executable(OrderBook benchmark.cpp)

# Set output directory for the executable to the root directory
set_target_properties(OrderBook PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
