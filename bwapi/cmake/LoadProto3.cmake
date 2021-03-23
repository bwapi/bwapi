include(FetchContent)

set(PROTOBUF_VERSION 3.14.0)

# Disable all other protobuf projects
set(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_CONFORMANCE OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_PROTOC_BINARIES OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_LIBPROTOC OFF CACHE BOOL "" FORCE)
set(protobuf_WITH_ZLIB OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(protobuf_MSVC_STATIC_RUNTIME OFF CACHE BOOL "" FORCE)

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
FetchContent_MakeAvailable(protoc)
FetchContent_GetProperties(protobuf)
if(NOT protobuf_POPULATED)
  FetchContent_Populate(protobuf)

  add_subdirectory(${protobuf_SOURCE_DIR}/cmake
                   ${protobuf_BINARY_DIR})
endif()

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
  set_target_properties(libprotobuf libprotobuf-lite PROPERTIES FOLDER "Third Party/protobuf")
endif()


# Run protoc on inputs (as a pre-build step, CMake auto-detects that the OUTPUT from this is the INPUT to add_library)
# Also generate list of sources and headers
# Must call before add_library/add_executable
# Note: Inputs must be quoted because I'm a noob
function(PROTOC_TO_CPP_FILES PROTO_INPUTS PROTO_OUTPUT_DIR OUT_GEN_SOURCES OUT_GEN_HEADERS)
  file(MAKE_DIRECTORY ${PROTO_OUTPUT_DIR})

  set(GENERATED_PROTO_SOURCES "")
  set(GENERATED_PROTO_HEADERS "")

  foreach(PROTO_INPUT ${PROTO_INPUTS})
    string(REGEX REPLACE ".*[\\\\/](.+)\\.proto" "${PROTO_OUTPUT_DIR}/\\1.pb" PROTO_OUTPUT_FILE ${PROTO_INPUT})
    set(PROTO_C_FILE "${PROTO_OUTPUT_FILE}.cc")
    set(PROTO_H_FILE "${PROTO_OUTPUT_FILE}.h")
    string(REGEX MATCH ".*[\\\\/]" PROTO_PATH ${PROTO_INPUT})

    add_custom_command(
      OUTPUT "${PROTO_C_FILE}" "${PROTO_H_FILE}"
      COMMAND "${protoc_BIN_DIR}/protoc" ARGS "--cpp_out=${PROTO_OUTPUT_DIR}" "--error_format=${PROTOC_ERROR_FORMAT}" "--proto_path=${PROTO_PATH}" "${PROTO_INPUT}"
    )

    list(APPEND GENERATED_PROTO_SOURCES "${PROTO_C_FILE}")
    list(APPEND GENERATED_PROTO_HEADERS "${PROTO_H_FILE}")
  endforeach()

  set(${OUT_GEN_SOURCES} ${GENERATED_PROTO_SOURCES} PARENT_SCOPE)
  set(${OUT_GEN_HEADERS} ${GENERATED_PROTO_HEADERS} PARENT_SCOPE)
endfunction()

