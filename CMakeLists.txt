cmake_minimum_required(VERSION 3.24)

if (USE_PICO_LIB)
	include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

	project(rgpstk C CXX ASM)

	set(CMAKE_C_STANDARD 11)
	set(CMAKE_CXX_STANDARD 17)
else ()
	project(rgpstk C)

	set(CMAKE_C_STANDARD 99)
	include(CTest)
endif ()

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

set(LIB_SOURCES lib/nmea.c  lib/version.c  lib/geo.c)
set(LIB_HEADERS lib/nmea.h lib/version.h lib/geo.h)

add_library(rgpstk_objects OBJECT ${LIB_SOURCES} ${LIB_HEADERS}) # headers added for IDE related reasons
set_target_properties(rgpstk_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_library(rgpstk_static STATIC $<TARGET_OBJECTS:rgpstk_objects>)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

if (USE_PICO_LIB)
	set_target_properties(rgpstk_static PROPERTIES OUTPUT_NAME "rgpstk_pico")
	target_link_libraries(rgpstk_static PRIVATE pico_stdlib m)
else ()
	set_target_properties(rgpstk_static PROPERTIES OUTPUT_NAME "rgpstk")
	add_library(rgpstk_shared SHARED $<TARGET_OBJECTS:rgpstk_objects>)
	set_target_properties(rgpstk_shared PROPERTIES OUTPUT_NAME "rgpstk")
	target_link_libraries(rgpstk_static PRIVATE m)
	target_link_libraries(rgpstk_shared PRIVATE m)
	install(TARGETS rgpstk_shared LIBRARY DESTINATION lib)
	install(TARGETS rgpstk_static ARCHIVE DESTINATION lib)
	install(FILES ${LIB_HEADERS} DESTINATION include/rgpstk)

	include(cmake/coverage.cmake)
	handle_coverage(rgpstk_objects)
	handle_coverage(rgpstk_static)
	handle_coverage(rgpstk_shared)

	add_subdirectory(tools)
	add_subdirectory(test)
endif ()
unset(USE_PICO_LIB CACHE)