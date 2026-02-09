cmake_minimum_required(VERSION 3.24)

project(rgpstk_nmea_checksum C)

set(CMAKE_C_STANDARD 99)

add_executable(rgpstk_nmea_checksum src/main.c)
target_link_libraries(rgpstk_nmea_checksum PRIVATE rgpstk)
install(TARGETS rgpstk_nmea_checksum RUNTIME DESTINATION bin)