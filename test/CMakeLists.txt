cmake_minimum_required(VERSION 3.24)

project(rgpstk_tests C)

set(CMAKE_C_STANDARD 11)

include("${CMAKE_SOURCE_DIR}/cmake/coverage.cmake")

add_executable(test_runner src/test_runner.c tests/test_nmea_lat_long.c)
target_link_libraries(test_runner PRIVATE rgpstk)
add_test(NAME all_tests COMMAND "${CMAKE_SOURCE_DIR}/build/test_runner")
set_tests_properties(all_tests PROPERTIES ENVIRONMENT "LLVM_PROFILE_FILE=${CMAKE_SOURCE_DIR}/coverage/test_run.profraw")
handle_coverage(test_runner)
