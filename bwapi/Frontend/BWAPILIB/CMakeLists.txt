cmake_minimum_required(VERSION 3.16)

#########################
# Project configuration

project(BWAPILIB
  DESCRIPTION "Static library containing BWAPI types, containers, and interfaces"
)

file(GLOB_RECURSE CPP_FILES Source/*.cpp)
file(GLOB_RECURSE HEADER_FILES Source/*.h)
file(GLOB_RECURSE INTERFACE_FILES ${BWAPI_INCLUDE_DIR}/*.h)
include_directories(${BWAPI_INCLUDE_DIR})
add_library(${PROJECT_NAME} STATIC
  ${CPP_FILES}
  ${HEADER_FILES}
  ${INTERFACE_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Source Files" FILES ${CPP_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${BWAPI_INCLUDE_DIR} PREFIX "Header Files/Interface" FILES ${INTERFACE_FILES})

#########################
# Project Properties

set_target_properties(${PROJECT_NAME} PROPERTIES
  FOLDER Frontend
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
  # /Za /permissive- /Zc:rvalueCast    <-- Enforces conformance with the C++ standard (as best as VS can manage)
  # /wd4100 /wd4458     <-- Disable warnings 4100 and 4458
  # /MP                 <-- Enable multi-processor compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Za /permissive- /Zc:rvalueCast /wd4100 /wd4458 /MP")
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wpedantic)
endif()
