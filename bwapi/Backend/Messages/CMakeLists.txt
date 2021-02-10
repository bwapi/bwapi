cmake_minimum_required(VERSION 3.16)
include(FetchContent)

#########################
# Get Protobuf

set(PROTOBUF_VERSION 3.14.0)

# Disable all other protobuf projects
option(protobuf_BUILD_TESTS "" OFF)
option(protobuf_BUILD_CONFORMANCE "" OFF)
option(protobuf_BUILD_EXAMPLES "" OFF)
option(protobuf_BUILD_PROTOC_BINARIES "" OFF)
option(protobuf_BUILD_LIBPROTOC "" OFF)
option(protobuf_WITH_ZLIB "" OFF)
option(protobuf_BUILD_SHARED_LIBS "" OFF)

# Get Protobuf source code, the library is needed to compile the generated protobuf files
FetchContent_Declare(protobuf
  GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
  GIT_TAG v${PROTOBUF_VERSION}
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  SOURCE_SUBDIR cmake
)

# Download protoc (protobuf compiler) to avoid unnecessary builds
set(PROTOC_URL_PREFIX "https://github.com/protocolbuffers/protobuf/releases/download/v${PROTOBUF_VERSION}/protoc-${PROTOBUF_VERSION}")

if(CMAKE_HOST_WIN32 OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86")
    FetchContent_Declare(protoc URL "${PROTOC_URL_PREFIX}-win32.zip")
  else()
    FetchContent_Declare(protoc URL "${PROTOC_URL_PREFIX}-win64.zip")
  endif()
elseif(CMAKE_HOST_APPLE)
  FetchContent_Declare(protoc URL "${PROTOC_URL_PREFIX}-osx-x86_64.zip")
elseif(CMAKE_HOST_LINUX OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86")
    FetchContent_Declare(protoc URL "${PROTOC_URL_PREFIX}-linux-x86_32.zip")
  else()
    FetchContent_Declare(protoc URL "${PROTOC_URL_PREFIX}-linux-x86_64.zip")
  endif()
else()
  message(FATAL_ERROR "Unable to determine the host operating system for protoc.")
endif()

# Download the protobuf source and protoc
FetchContent_MakeAvailable(protobuf protoc)

# Set the protoc error format (to be passed to protoc CLI)
if(MSVC)
  set(PROTOC_ERROR_FORMAT msvs)
else()
  set(PROTOC_ERROR_FORMAT gcc)
endif()

# Set variables and properties
set(protoc_BIN_DIR ${protoc_SOURCE_DIR}/bin)
set(protobuf_INCLUDE_DIR ${protobuf_SOURCE_DIR}/src)

if(MSVC)
  set_target_properties(libprotobuf libprotobuf-lite PROPERTIES FOLDER protobuf)
endif()

#########################
# Project configuration

project(Messages
  DESCRIPTION "BWAPI protobuf network message definitions"
)

file(GLOB_RECURSE PROTO_INPUTS ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)

set(GENERATED_PROTO_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/cpp)
file(MAKE_DIRECTORY ${GENERATED_PROTO_DIR})

# Run protoc on inputs (as a pre-build step, CMake auto-detects that the OUTPUT from this is the INPUT to add_library)
# Also generate list of sources and headers
set(GENERATED_PROTO_SOURCES "")
set(GENERATED_PROTO_HEADERS "")
foreach(PROTO_FILE ${PROTO_INPUTS})
  string(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/(.+).proto" "${GENERATED_PROTO_DIR}/\\1.pb" PROTO_FILE_OUTPUT ${PROTO_FILE})
  set(PROTO_C_FILE "${PROTO_FILE_OUTPUT}.cc")
  set(PROTO_H_FILE "${PROTO_FILE_OUTPUT}.h")

  add_custom_command(
    OUTPUT "${PROTO_C_FILE}" "${PROTO_H_FILE}"
    COMMAND "${protoc_BIN_DIR}/protoc" ARGS "--cpp_out=${GENERATED_PROTO_DIR}" "--error_format=${PROTOC_ERROR_FORMAT}" "--proto_path=${CMAKE_CURRENT_SOURCE_DIR}" "${PROTO_FILE}"
    BYPRODUCTS "${PROTO_C_FILE}" "${PROTO_H_FILE}"
  )

  list(APPEND GENERATED_PROTO_SOURCES "${PROTO_C_FILE}")
  list(APPEND GENERATED_PROTO_HEADERS "${PROTO_H_FILE}")
endforeach()

# Finally get to the project
include_directories(
  ${protobuf_INCLUDE_DIR}
  ${GENERATED_PROTO_DIR}
)

add_library(${PROJECT_NAME} STATIC
  ${PROTO_INPUTS}
  ${GENERATED_PROTO_SOURCES}
  ${GENERATED_PROTO_HEADERS}
)

source_group("Proto Files" FILES ${PROTO_INPUTS})
source_group("Source Files" FILES ${GENERATED_PROTO_SOURCES})
source_group("Header Files" FILES ${GENERATED_PROTO_HEADERS})

#########################
# Project Properties

target_link_libraries(${PROJECT_NAME} libprotobuf)

set_target_properties(${PROJECT_NAME} PROPERTIES
  FOLDER Backend
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if(MSVC)
  # /MP                 <-- Enable multi-processor compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()
