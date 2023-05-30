cmake_minimum_required(VERSION 3.24)
project(pbad_2d_bin_packing)

set(CMAKE_CXX_STANDARD 11)

#add_executable(pbad_2d_bin_packing main.cpp Generator.h Generator.cpp)
add_executable(pbad_2d_bin_packing main.cpp Palette.cpp Palette.h NeuralNetwork.cpp NeuralNetwork.h Palette.h Generator.h Generator.cpp)
