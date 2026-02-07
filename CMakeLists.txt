cmake_minimum_required(VERSION 3.24)

project(rgpstk C)

set(CMAKE_C_STANDARD 11)
include(CTest)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" ver)
string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(v_major ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(v_minor ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
set(v_patch ${CMAKE_MATCH_1})
add_compile_definitions(RGPSTK_VERSION_MAJOR=${v_major} RGPSTK_VERSION_MINOR=${v_minor} RGPSTK_VERSION_PATCH=${v_patch})

link_directories(${PROJECT_SOURCE_DIR}/build)
include_directories(${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_library(rgpstk STATIC lib/nmea.c lib/nmea.h
	tools/nmea_parser/src/main.c
	lib/version.c lib/version.h
	lib/geo.c
	lib/geo.h
	lib/geo_defs.h
)

add_subdirectory(tools)
add_subdirectory(test)