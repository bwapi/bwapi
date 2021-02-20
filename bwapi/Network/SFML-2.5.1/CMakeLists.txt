cmake_minimum_required(VERSION 3.14)

#########################
# Project configuration

project(SFML_network_lite
  VERSION 2.5.1
  DESCRIPTION "Gutted version of SFML-network"
)

file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h src/*.hpp)
file(GLOB_RECURSE INTERFACE_FILES include/*.h include/*.hpp)

add_library(${PROJECT_NAME} STATIC
  ${CPP_FILES}
  ${HEADER_FILES}
  ${INTERFACE_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${CPP_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Interface Files" FILES ${INTERFACE_FILES})

#########################
# Project Properties

set_target_properties(${PROJECT_NAME} PROPERTIES
  FOLDER Network
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if(MSVC)
  # Get rid of auto-inserted warnings
  string(REGEX REPLACE "/W[0-9]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  # /permissive- /Zc:rvalueCast    <-- Enforces conformance with the C++ standard (as best as VS can manage) (allows VS language extensions needed for windows.h)
  # /MP                 <-- Enable multi-processor compilation
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /Zc:rvalueCast /MP)
elseif(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wpedantic)
endif()
