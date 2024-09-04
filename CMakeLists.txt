cmake_minimum_required(VERSION 3.10)

# Project name
project(BitsyGame)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory for the header files
include_directories(include)

# Add the executable target for the main application
add_executable(BitsyGame main.cpp)

# Add the executable target for the tests
add_executable(BitsyGameTests tests/test_bitsy_game.cpp src/BitsyGame.cpp)
