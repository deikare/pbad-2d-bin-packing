#!/bin/bash

g++ -fopenmp -std=gnu++11 -I/usr/share/doc/libeigen3-dev -I/home/deikare/pbad-2d-bin-packing/include/libcmaes -L/home/deikare/pbad-2d-bin-packing/lib -o sample_code main.cpp -lcmaes
