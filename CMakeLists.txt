cmake_minimum_required(VERSION 3.24)
project(rgpstk C)

set(CMAKE_C_STANDARD 11)

add_library(rgpstk STATIC lib/nmea.c lib/nmea.h)
