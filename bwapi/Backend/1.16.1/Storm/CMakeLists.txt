cmake_minimum_required(VERSION 3.14)

#########################
# Project configuration

project(storm
  DESCRIPTION "Placeholder for linking with storm.dll"
)

if (NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(SEND_ERROR "${PROJECT_NAME} only supports a 32-bit Windows target")
endif()

file(GLOB_RECURSE CPP_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)
set(DEF_FILE storm.def)

add_library(${PROJECT_NAME} SHARED
  ${CPP_FILES}
  ${HEADER_FILES}
  ${DEF_FILE}
)

source_group("Source Files" FILES ${CPP_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Resource Files" FILES ${DEF_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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
  endforeach()
  # /W4                 <-- Warning level 4
  # /wd####             <-- Disable warning number
  # /permissive- /Zc:rvalueCast    <-- C++ standard conformance (as best as VS can manage, allow windows.h)
  # /MP                 <-- Enable multi-processor compilation
  # /MT /MTd            <-- MultiThreaded (statically linked runtime)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /wd4100 /permissive- /Zc:rvalueCast /MP)
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
  NOWINMESSAGES
  NOWINOFFSETS
  NOWINSTYLES
  WIN32_LEAN_AND_MEAN
  WIN32_NO_STATUS
)
