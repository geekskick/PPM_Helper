cmake_minimum_required(VERSION 3.8)
project(SongSim)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp ppm_file.cpp ppm_file.h)
add_executable(SongSim ${SOURCE_FILES})