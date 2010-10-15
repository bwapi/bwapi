#-------------------------------------------------------------------------------------------------------------
# Hide the annoying CMAKE_ECLIPSE_EXECUTABLE-NOTFOUND variable
MARK_AS_ADVANCED(FORCE CMAKE_ECLIPSE_EXECUTABLE)

#-------------------------------------------------------------------------------------------------------------
# Setup the systems include directories for Visual Studio so that Eclipse can find the headers  
# Analogous to what is done in CMakeFindEclipseCDT4.cmake

# The environment from the vcvars.bat file has to be setup

SET(_dirs
  #$ENV{VCINSTALLDIR}\\include
  #$ENV{WindowsSdkDir}Include
  $ENV{INCLUDE}
)

# TODO: set the VC++ predefined macros for use in Eclipse IDE
SET(_defines "")

IF (NOT CMAKE_ECLIPSE_C_SYSTEM_INCLUDE_DIRS)
  IF ("${CMAKE_C_COMPILER_ID}" STREQUAL MSVC)
    SET(CMAKE_ECLIPSE_C_SYSTEM_INCLUDE_DIRS "${_dirs}" CACHE INTERNAL "C compiler system include directories")
    SET(CMAKE_ECLIPSE_C_SYSTEM_DEFINED_MACROS "${_defines}" CACHE INTERNAL "C compiler system defined macros")
  ENDIF ()
ENDIF ()

IF (NOT CMAKE_ECLIPSE_CXX_SYSTEM_INCLUDE_DIRS)
  IF ("${CMAKE_C_COMPILER_ID}" STREQUAL MSVC)
    SET(CMAKE_ECLIPSE_CXX_SYSTEM_INCLUDE_DIRS "${_dirs}" CACHE INTERNAL "CXX compiler system include directories")
    SET(CMAKE_ECLIPSE_CXX_SYSTEM_DEFINED_MACROS "${_defines}" CACHE INTERNAL "CXX compiler system defined macros")
  ENDIF ()
ENDIF ()