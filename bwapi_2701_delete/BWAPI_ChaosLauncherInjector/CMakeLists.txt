PROJECT(BWAPI_ChaosLauncherInjector_cmake)

SET(SOURCES 
  source/DLLMain.cpp
)

INCLUDE_DIRECTORIES( 
  ../Util/Source
  ../include
  
  # FIXME: This is for the #include "../../svnrev.h" which is created in the build directory
  ${CMAKE_CURRENT_BINARY_DIR}/Source
)

ADD_DEFINITIONS(
  -D_CRT_SECURE_NO_WARNINGS
)

ADD_LIBRARY(BWAPI_ChaosLauncherInjector_cmake MODULE ${SOURCES})
SET_TARGET_PROPERTIES(BWAPI_ChaosLauncherInjector_cmake PROPERTIES SUFFIX .bwl)
SET_TARGET_PROPERTIES(BWAPI_ChaosLauncherInjector_cmake PROPERTIES PROJECT_LABEL BWAPI_ChaosLauncherInjector)
SET_TARGET_PROPERTIES(BWAPI_ChaosLauncherInjector_cmake PROPERTIES OUTPUT_NAME BWAPI_ChaosLauncherInjector)