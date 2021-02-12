cmake_minimum_required(VERSION 3.16)
include(LoadGoogleTest)

#########################
# Project configuration

project(BWAPILIBUnitTest
  DESCRIPTION "Unit tests for BWAPILIB"
)

file(GLOB_RECURSE CPP_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_executable(${PROJECT_NAME}
  ${CPP_FILES}
  ${HEADER_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${CPP_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files" FILES ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} BWAPILIB gmock)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} --gtest_output=xml:unit_test_results.xml)

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
  # /MP                 <-- Enable multi-processor compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Za /permissive- /Zc:rvalueCast /MP")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wpedantic)

  if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "")
    include(CodeCoverage)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -fprofile-arcs -ftest-coverage -O0)
    target_link_libraries(${PROJECT_NAME} gcov)
  endif()
endif()
