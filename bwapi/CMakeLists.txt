CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(BWAPIv2_cmake)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

IF (${CMAKE_GENERATOR} MATCHES "NMake")
  SET(OUTDIR ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
ELSE ()
  SET(OUTDIR ${PROJECT_BINARY_DIR})
ENDIF ()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTDIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTDIR})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTDIR})

#-------------------------------------------------------------------------------------------------------------
OPTION(BWAPI_FAST_BUILD "Include all source files into one big and compile that (this can be faster)" OFF)

#-------------------------------------------------------------------------------------------------------------

ADD_SUBDIRECTORY(Util)
ADD_SUBDIRECTORY(BWAPILIB)
ADD_SUBDIRECTORY(BWAPI)
ADD_SUBDIRECTORY(BWAPI_ChaosLauncherInjector)

#-------------------------------------------------------------------------------------------------------------
# Includes
INCLUDE(svnrevconfig)
INCLUDE(setupeclipse)

#-------------------------------------------------------------------------------------------------------------
# Doxygen

FIND_PACKAGE(Doxygen)

IF (DOXYGEN_FOUND)
  SET(DOXYFILE_INPUT_DIR "${PROJECT_SOURCE_DIR}/include")
  SET(DOXYFILE_OUTPUT_DIR "${PROJECT_BINARY_DIR}/doc")
  
  CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/include/Doxyfile.in"
    "${PROJECT_BINARY_DIR}/Doxyfile"
    IMMEDIATE @ONLY)
    
  FILE(TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/Doxyfile" DOXYFILE_PATH)
  
  ADD_CUSTOM_TARGET(doc COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYFILE_PATH}")
ENDIF ()

#-------------------------------------------------------------------------------------------------------------
# Custom target to directly install the Chaoslauncher plugin into the Chaoslauncher directory

FIND_PATH(STARCRAFT_ROOT_DIR StarCraft.exe
  PATHS
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Blizzard Entertainment\\Starcraft;InstallPath]"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Blizzard Entertainment\\Starcraft;InstallPath]"
  $ENV{ProgramFiles}/Starcraft
  $ENV{SystemDrive}/Starcraft
  $ENV{UserProfile}/Starcraft
)

FIND_PATH(CHAOS_DIR Chaoslauncher.exe
  PATHS
  $ENV{ChaosDir}
  ${STARCRAFT_ROOT_DIR}/Chaoslauncher
)
	
GET_TARGET_PROPERTY(BWAPI_OUTPUT BWAPI_cmake LOCATION)
GET_TARGET_PROPERTY(BWAPI_ChaosLauncherInjector_OUTPUT BWAPI_ChaosLauncherInjector_cmake LOCATION)

ADD_CUSTOM_TARGET(install_clauncher 
  COMMAND ${CMAKE_COMMAND} -E copy "${BWAPI_OUTPUT}" "${CHAOS_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "${BWAPI_ChaosLauncherInjector_OUTPUT}" "${CHAOS_DIR}"
  DEPENDS BWAPI_cmake BWAPI_ChaosLauncherInjector_cmake
)

# allows choosing whether to install after building  
OPTION(BWAPI_INSTALL_AFTER_BUILD "run install_clauncher after building" TRUE)

IF (BWAPI_INSTALL_AFTER_BUILD)
  SET_TARGET_PROPERTIES(install_clauncher PROPERTIES EXCLUDE_FROM_ALL FALSE)
ELSE()
  SET_TARGET_PROPERTIES(install_clauncher PROPERTIES EXCLUDE_FROM_ALL TRUE)
ENDIF()

#-------------------------------------------------------------------------------------------------------------
# Export

EXPORT(PACKAGE BWAPI)
EXPORT(TARGETS BWAPILIB_cmake FILE bwapi-targets.cmake)

CONFIGURE_FILE(
  bwapi-config.in.cmake
  bwapi-config.cmake  
  IMMEDIATE @ONLY)
