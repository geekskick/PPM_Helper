enable_testing()
add_executable(ppm_test ppm_test.cpp)
target_link_libraries(ppm_test PUBLIC ppm_helper)