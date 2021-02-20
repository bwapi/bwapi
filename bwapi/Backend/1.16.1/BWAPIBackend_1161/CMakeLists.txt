cmake_minimum_required(VERSION 3.14)

#########################
# Project configuration

project(BWAPI
  VERSION ${BWAPI_VERSION}
  DESCRIPTION "BWAPI.dll for Starcraft 1.16.1"
)

if (NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(SEND_ERROR "${PROJECT_NAME} only supports a 32-bit Windows target")
endif()

file(GLOB_RECURSE CPP_FILES Source/*.cpp)
file(GLOB_RECURSE HEADER_FILES Source/*.h)
file(GLOB_RECURSE BWAPILIB4_CPP_FILES BWAPILIB_4_3_0/*.cpp)
file(GLOB_RECURSE BWAPILIB4_HEADER_FILES bwapi4_include/*.h)

add_library(${PROJECT_NAME} SHARED
  ${CPP_FILES}
  ${HEADER_FILES}
  ${BWAPILIB4_CPP_FILES}
  ${BWAPILIB4_HEADER_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Source Files" FILES ${CPP_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/BWAPILIB_4_3_0 PREFIX "BWAPILIB_Old" FILES ${BWAPILIB4_CPP_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/bwapi4_include PREFIX "BWAPILIB_Old" FILES ${BWAPILIB4_HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "bwapi4_include")
target_link_libraries(${PROJECT_NAME} BWAPILIB BWAPINetworkCore storm Version)

#########################
# Post-build commands

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${BWAPI_RELEASE_BWAPIDATA_DIR}/BWAPI$<$<CONFIG:Debug>:d>.dll"
  BYPRODUCTS "${BWAPI_RELEASE_BWAPIDATA_DIR}/BWAPI.dll" "${BWAPI_RELEASE_BWAPIDATA_DIR}/BWAPId.dll"
  COMMENT "Copying BWAPI.dll to release directory."
)

find_path(STARCRAFT_DIR Starcraft.exe
  HINTS
    "[HKEY_CURRENT_USER\\SOFTWARE\\WOW6432Node\\Blizzard Entertainment\\Starcraft;InstallPath]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Blizzard Entertainment\\Starcraft;InstallPath]"
    "[HKEY_CURRENT_USER\\SOFTWARE\\Blizzard Entertainment\\Starcraft;InstallPath]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Blizzard Entertainment\\Starcraft;InstallPath]"
  NO_DEFAULT_PATH
)

if(STARCRAFT_DIR)
  message("Found Starcraft directory: ${STARCRAFT_DIR}")

  set(STARCRAFT_BWAPI_PREFIX "${STARCRAFT_DIR}/bwapi-data/BWAPI")
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${STARCRAFT_BWAPI_PREFIX}$<$<CONFIG:Debug>:d>.dll"
    BYPRODUCTS "${STARCRAFT_BWAPI_PREFIX}.dll" "${STARCRAFT_BWAPI_PREFIX}d.dll"
    COMMENT "Copying BWAPI.dll to Starcraft directory."
  )
endif()

#########################
# Project Properties

set_target_properties(${PROJECT_NAME} PROPERTIES
  FOLDER "1.16.1"
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# TODO: Add this /MT, currently not compatible because dependencies are compiling with /MD (meaning CRT is linked dynamically instead of statically)
if(MSVC)
  # Get rid of auto-inserted flags
  foreach(flag CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    string(REGEX REPLACE "/W[0-9]" "" ${flag} "${${flag}}")
    #string(REGEX REPLACE "/MDd?" "" ${flag} "${${flag}}")
  endforeach()
  # /W4                 <-- Warning level 4
  # /wd####             <-- Disable warning number
  # /permissive- /Zc:rvalueCast    <-- C++ standard conformance (as best as VS can manage, allow windows.h)
  # /MP                 <-- Enable multi-processor compilation
  # /MT /MTd            <-- MultiThreaded (statically linked runtime)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /wd4100 /permissive- /Zc:rvalueCast /MP)
endif()

# Minimize Windows.h overhead
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
  #NOMB
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
  #WIN32_NO_STATUS
  _CRT_SECURE_NO_WARNINGS
)
