cmake_minimum_required(VERSION 3.24)

project(rgpstk_tests C)

set(CMAKE_C_STANDARD 11)

add_executable(test_runner_nmea_lat_long test_runner_nmea_lat_long.c)
target_link_libraries(test_runner_nmea_lat_long PRIVATE rgpstk)
add_test(NAME test_nmea_lat_long COMMAND test_runner_nmea_lat_long)