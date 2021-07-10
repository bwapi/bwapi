cmake_minimum_required(VERSION 3.14)

#########################
# Project configuration

project(BWAPI_PluginInjector
  VERSION ${BWAPI_VERSION}
  DESCRIPTION "BWAPI Chaoslauncher(.bwl) and MPQDraft(.qdp) plugin injector"
)

if (NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(SEND_ERROR "${PROJECT_NAME} only supports a 32-bit Windows target")
endif()

file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h)
set(DEF_FILE QDPlugin.def)

add_library(${PROJECT_NAME} SHARED
  ${CPP_FILES}
  ${HEADER_FILES}
  ${DEF_FILE}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${CPP_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${HEADER_FILES})
source_group("Resource Files" FILES ${DEF_FILE})

#########################
# Post-build commands

set(README_PREFIX "${BWAPI_RELEASE_CHAOS_PLUGIN_DIR}/${PROJECT_NAME}")
set(BWL_PREFIX "${BWAPI_RELEASE_CHAOS_PLUGIN_DIR}/${PROJECT_NAME}")
set(QDP_PREFIX "${BWAPI_RELEASE_MPQDRAFT_DIR}/${PROJECT_NAME}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/README.md" "${README_PREFIX}$<$<CONFIG:Debug>:d>.txt"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${BWL_PREFIX}$<$<CONFIG:Debug>:d>.bwl"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${QDP_PREFIX}$<$<CONFIG:Debug>:d>.qdp"
  BYPRODUCTS ${README_PREFIX}.txt ${README_PREFIX}d.txt
    ${BWL_PREFIX}.bwl ${BWL_PREFIX}d.bwl
    ${QDP_PREFIX}.bwl ${QDP_PREFIX}d.bwl
  COMMENT "Copying README and .bwl for Chaoslauncher, copying .qdp for MPQDraft"
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
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /wd4996 /permissive- /Zc:rvalueCast /MP /MT$<$<CONFIG:Debug>:d>)
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
  NOCTLMGR
  NODEFERWINDOWPOS
  NODESKTOP
  NODRAWTEXT
  NOFONTSIG
  NOGDI
  NOGDICAPMASKS
  NOHELP
  NOICONS
  NOIME
  NOKANJI
  NOKERNEL
  NOKEYSTATES
  NOLANGUAGE
  #NOMB
  NOMCX
  NOMDI
  NOMEMMGR
  NOMENUS
  NOMETAFILE
  NOMINMAX
  NOMSG
  NONCMESSAGES
  NONLS
  NOOPENFILE
  NOPROFILER
  NORASTEROPS
  NORESOURCE
  NOSCROLL
  NOSECURITY
  NOSERVICE
  #NOSHOWWINDOW
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
