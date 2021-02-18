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
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if(MSVC)
  # /permissive- /Zc:rvalueCast    <-- C++ standard conformance (as best as VS can manage, allow windows.h)
  # /MP                 <-- Enable multi-processor compilation
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /Zc:rvalueCast /MP)

  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Za /permissive- /Zc:rvalueCast /wd4100 /wd4458 /MP")
endif()
