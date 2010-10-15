PROJECT(Util_cmake)

SET(SOURCES 
  Source/Util/Exceptions.cpp
  Source/Util/FileLogger.cpp
  Source/Util/Logger.cpp
  Source/Util/Strings.cpp
)

ADD_DEFINITIONS(
  -D_CRT_SECURE_NO_WARNINGS
)

ADD_LIBRARY(Util_cmake STATIC ${SOURCES})
SET_TARGET_PROPERTIES(Util_cmake PROPERTIES PROJECT_LABEL Util)
SET_TARGET_PROPERTIES(Util_cmake PROPERTIES OUTPUT_NAME Util)
