cmake_minimum_required(VERSION 3.16)

#########################
# General CMake settings

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Make only Debug/Release valid in IDEs
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

enable_testing()

#########################
# BWAPI Project globals

if(DEFINED ENV{APPVEYOR_BUILD_VERSION})
  set(BWAPI_VERSION $ENV{APPVEYOR_BUILD_VERSION})
else()
  set(BWAPI_VERSION 5.0.0)
endif()

set(BWAPI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

#########################
# Projects

project(BWAPIProject
  VERSION ${BWAPI_VERSION}
  HOMEPAGE_URL https://bwapi.github.io/
)

add_subdirectory(Frontend/BWAPILIB)
add_subdirectory(Frontend/BWAPILIBUnitTest)
add_subdirectory(Frontend) # Coverage targets
add_subdirectory(Backend/Messages)
add_subdirectory(Backend/BWAPIBackendCore)
add_subdirectory(Backend/SCRAPIMessages)
add_subdirectory(Backend/SCRAPIBackendCore)
add_subdirectory(contrib/SFML-2.5.1)
