cmake_minimum_required(VERSION 3.24)

project(rgpstk_nmea_parser C)

set(CMAKE_C_STANDARD 99)

add_executable(rgpstk_nmea_parser src/main.c
	src/distance_delta.c src/distance_delta.h
)
target_link_libraries(rgpstk_nmea_parser PRIVATE rgpstk)
install(TARGETS rgpstk_nmea_parser RUNTIME DESTINATION bin)
