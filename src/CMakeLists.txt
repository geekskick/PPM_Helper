add_library(ppm_helper STATIC ppm_file.cpp)
target_include_directories(ppm_helper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})