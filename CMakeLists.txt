cmake_minimum_required(VERSION 3.8)
project(SongSim)

enable_testing()
include(CTest)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(src)
add_subdirectory(test)

add_executable(SongSim main.cpp)
target_compile_options(SongSim PRIVATE -Werror -Wall -Wextra -pedantic)
target_include_directories(SongSim PRIVATE /usr/local/Cellar/tclap/1.2.2/include/)
target_link_libraries(SongSim PUBLIC ppm_helper)
