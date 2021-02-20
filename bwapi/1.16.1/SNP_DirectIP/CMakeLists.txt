cmake_minimum_required(VERSION 3.14)

#########################
# Project configuration

project(SNP_DirectIP
  DESCRIPTION "SNP for DirectIP and LocalPC network modes"
)

if (NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(SEND_ERROR "${PROJECT_NAME} only supports a 32-bit Windows target")
endif()

file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h)
set(DEF_FILE SNPModule.def)
set(RC_FILE UI.rc)

add_library(${PROJECT_NAME} SHARED
  ${CPP_FILES}
  ${HEADER_FILES}
  ${DEF_FILE}
  ${RC_FILE}
  resource.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${CPP_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${HEADER_FILES})
source_group("Resource Files" FILES ${DEF_FILE} ${RC_FILE} resource.h)

target_link_libraries(${PROJECT_NAME} ws2_32 storm)

#########################
# Post-build commands

set(CAPS_MPQ "${CMAKE_CURRENT_SOURCE_DIR}/caps.mpq")
set(SNP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.snp")

file(TO_NATIVE_PATH "${CAPS_MPQ}" NATIVE_CAPS_MPQ)
file(TO_NATIVE_PATH "${SNP_FILE}" NATIVE_SNP_FILE)
file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}" NATIVE_BINARY_DIR)

# Note: Using windows-specific copy command. CMake 3.18 has a platform-independent "cat" command but we are targeting Windows here anyway.
# The cat command was also broken in Windows for binary files until 3.18.5
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND copy /Y /B "$<SHELL_PATH:$<TARGET_FILE:${PROJECT_NAME}>>" + "${NATIVE_CAPS_MPQ}" "${NATIVE_SNP_FILE}"
  COMMAND ${CMAKE_COMMAND} -E copy "${SNP_FILE}" "${BWAPI_RELEASE_STARCRAFT_DIR}/${PROJECT_NAME}.snp"
  BYPRODUCTS ${SNP_FILE} ${BWAPI_RELEASE_STARCRAFT_DIR}/${PROJECT_NAME}.snp
  COMMENT "Concatenating caps MPQ to create SNP file."
  VERBATIM
)

#########################
# Project Properties

set_target_properties(${PROJECT_NAME} PROPERTIES
  FOLDER "1.16.1"
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if(MSVC)
  # Get rid of auto-inserted flags
  foreach(flag CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    string(REGEX REPLACE "/W[0-9]" "" ${flag} "${${flag}}")
    string(REGEX REPLACE "/MDd?" "" ${flag} "${${flag}}")
  endforeach()
  # /W4                 <-- Warning level 4
  # /wd####             <-- Disable warning number
  # /permissive- /Zc:rvalueCast    <-- C++ standard conformance (as best as VS can manage, allow windows.h)
  # /MP                 <-- Enable multi-processor compilation
  # /MT /MTd            <-- MultiThreaded (statically linked runtime)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /wd4100 /permissive- /Zc:rvalueCast /MP /MT$<$<CONFIG:Debug>:d>)
endif()

# Minimize Windows.h overhead
add_compile_definitions(
  NO_STATE_FLAGS
  NOAPISET
  NOATOM
  NOCLIPBOARD
  NOCOLOR
  NOCOMM
  NOCRYPT
  #NOCTLMGR
  NODEFERWINDOWPOS
  NODESKTOP
  NODRAWTEXT
  #NOFONTSIG
  #NOGDI
  NOGDICAPMASKS
  NOHELP
  NOICONS
  NOIME
  NOKANJI
  NOKERNEL
  NOKEYSTATES
  NOLANGUAGE
  NOMB
  NOMCX
  NOMDI
  NOMEMMGR
  NOMENUS
  NOMETAFILE
  NOMINMAX
  #NOMSG
  NONCMESSAGES
  NONLS
  NOOPENFILE
  NOPROFILER
  #NORASTEROPS
  NORESOURCE
  NOSCROLL
  NOSECURITY
  NOSERVICE
  NOSHOWWINDOW
  NOSOUND
  NOSYSCOMMANDS
  NOSYSMETRICS
  NOSYSPARAMSINFO
  NOTEXTMETRIC
  NOTRACKMOUSEEVENT
  #NOUSER
  NOVIRTUALKEYCODES
  NOWH
  NOWINABLE
  NOWINDOWSTATION
  #NOWINMESSAGES
  NOWINOFFSETS
  NOWINSTYLES
  WIN32_LEAN_AND_MEAN
  WIN32_NO_STATUS
)
