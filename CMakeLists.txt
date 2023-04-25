cmake_minimum_required(VERSION 3.24)
project(pbad_2d_bin_packing)

set(CMAKE_CXX_STANDARD 11)

add_executable(pbad_2d_bin_packing main.cpp NeuralNetwork.cpp NeuralNetwork.h)
#add_executable(pbad_2d_bin_packing main.cpp Palette.cpp Palette.h NeuralNetwork.cpp NeuralNetwork.h)
