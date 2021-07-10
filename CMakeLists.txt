cmake_minimum_required(VERSION 3.14)

message("Running CMake version ${CMAKE_VERSION}")

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

set(BWAPI_RELEASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/distrib")
set(BWAPI_RELEASE_CHAOS_PLUGIN_DIR "${BWAPI_RELEASE_DIR}/Chaoslauncher/Plugins")
set(BWAPI_RELEASE_MPQDRAFT_DIR "${BWAPI_RELEASE_DIR}/MPQdraft")
set(BWAPI_RELEASE_STARCRAFT_DIR "${BWAPI_RELEASE_DIR}/Starcraft")
set(BWAPI_RELEASE_BWAPIDATA_DIR "${BWAPI_RELEASE_STARCRAFT_DIR}/bwapi-data")
set(BWAPI_RELEASE_DOCUMENTATION_DIR "${BWAPI_RELEASE_DIR}/documentation")

#########################
# Projects

project(BWAPIProject
  VERSION ${BWAPI_VERSION}
  HOMEPAGE_URL https://bwapi.github.io/
)

add_subdirectory(Clients/DevAIClient)
add_subdirectory(Clients/ExampleAIClient)
add_subdirectory(Clients/ExampleTournamentClient)

add_subdirectory(Library) # Coverage targets
add_subdirectory(Library/BWAPILIB)
add_subdirectory(Library/BWAPILIBUnitTest)
add_subdirectory(Library/DocumentationGen)
add_subdirectory(Library/FunctionalTests)

add_subdirectory(Network/BWAPIFrontendClient)
add_subdirectory(Network/BWAPINetworkCore)
add_subdirectory(Network/Messages)
add_subdirectory(Network/SCRAPIMessages)
add_subdirectory(Network/SCRAPINetworkCore)
add_subdirectory(Network/SFML-2.5.1)

# 1.16.1 Projects, only Win32 supported
if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
  add_subdirectory(1.16.1/BWAPI_PluginInjector)
  add_subdirectory(1.16.1/BWAPIBackend_1161)
  add_subdirectory(1.16.1/SNP_DirectIP)
  add_subdirectory(1.16.1/Storm)
endif()
