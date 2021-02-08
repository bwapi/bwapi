cmake_minimum_required(VERSION 3.16)

project(BWAPIProject)

#########################
# General CMake settings

set(CMAKE_COLOR_MAKEFILE ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#########################
# BWAPI Project globals

set(BWAPI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

#########################
# Projects

add_subdirectory(Frontend/BWAPILIB)
