cmake_minimum_required(VERSION 3.14)
include(LoadProto3)

#########################
# Project configuration

project(Messages
  DESCRIPTION "BWAPI protobuf network message definitions"
)

file(GLOB_RECURSE PROTO_INPUTS *.proto)
set(GENERATED_PROTO_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/cpp)

protoc_to_cpp_files("${PROTO_INPUTS}" "${GENERATED_PROTO_DIR}" GENERATED_PROTO_SOURCES GENERATED_PROTO_HEADERS)

add_library(${PROJECT_NAME} STATIC
  ${PROTO_INPUTS}
  ${GENERATED_PROTO_SOURCES}
  ${GENERATED_PROTO_HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${protobuf_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

source_group("Proto Files" FILES ${PROTO_INPUTS})
source_group("Source Files" FILES ${GENERATED_PROTO_SOURCES})
source_group("Header Files" FILES ${GENERATED_PROTO_HEADERS})

#########################
# Project Properties

target_link_libraries(${PROJECT_NAME} libprotobuf)

set_target_properties(${PROJECT_NAME} PROPERTIES
  FOLDER Network
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if(MSVC)
  # Get rid of auto-inserted warnings
  string(REGEX REPLACE "/W[0-9]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  # /MP                 <-- Enable multi-processor compilation
  target_compile_options(${PROJECT_NAME} PRIVATE /MP /W0)
endif()
