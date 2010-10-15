#-------------------------------------------------------------------------------------------------------------
# Configure svnrev.h to reflect the svn revision number
# TODO: Create a CMake target to do this

FIND_PROGRAM(SVN_EXECUTABLE svn
  DOC "subversion command line client")

MACRO(Subversion_GET_REVISION dir variable)
  EXECUTE_PROCESS(COMMAND ${SVN_EXECUTABLE} info ${dir}
    OUTPUT_VARIABLE ${variable}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  STRING(REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
    "\\2" ${variable} "${${variable}}")
ENDMACRO()

MACRO(Subversion_GET_LOCAL_MODS dir variable)
  EXECUTE_PROCESS(COMMAND ${SVN_EXECUTABLE} status -q ${dir}
    OUTPUT_VARIABLE ${variable}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDMACRO()

IF (NOT SVN_EXECUTABLE STREQUAL "SVN_EXECUTABLE-NOTFOUND")
  Subversion_GET_REVISION(${PROJECT_SOURCE_DIR} SVN_REV)
  Subversion_GET_LOCAL_MODS(${PROJECT_SOURCE_DIR} SVN_LOCAL_MODS)
  
  SET(SVN_REV_STR "${SVN_REV}")
  IF(NOT SVN_LOCAL_MODS STREQUAL "")
    SET(SVN_REV_STR "${SVN_REV_STR}_M")
  ENDIF()
ELSE ()
  SET(SVN_REV 0)
  SET(SVN_REV_STR "Unknown")
ENDIF ()

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/svnrev.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/svnrev.h"
  IMMEDIATE @ONLY
)
