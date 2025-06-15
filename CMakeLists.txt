cmake_minimum_required(VERSION 3.10)
project(market_system)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
file(GLOB_RECURSE SOURCES
    "src/accounting/*.cpp"
    "src/utils/*.cpp"
    "src/main.cpp"
)

# Create executable
add_executable(market_system ${SOURCES})

# Set compiler flags
if(MSVC)
    target_compile_options(market_system PRIVATE /W4)
else()
    target_compile_options(market_system PRIVATE -Wall -Wextra -Wpedantic)
endif() 