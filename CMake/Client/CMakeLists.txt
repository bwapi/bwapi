project(BWAPI-Client CXX)

# Define this variable BWAPI_CUSTOM_COMPILE_FLAGS to set your compile flags
IF(BWAPI_CUSTOM_COMPILE_FLAGS)
    ADD_DEFINITIONS(${BWAPI_CUSTOM_COMPILE_FLAGS})
ENDIF(BWAPI_CUSTOM_COMPILE_FLAGS)

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(BWAPICLIENT_LIB_NAME BWAPIClientd)
    SET(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
ELSE ()
    SET(BWAPICLIENT_LIB_NAME BWAPIClient)
    SET(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
ENDIF ()

ADD_DEFINITIONS(/DNOMINMAX=1)

GET_FILENAME_COMPONENT(BWAPI_ROOT ../../bwapi ABSOLUTE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BWAPI_ROOT}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BWAPI_ROOT}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BWAPI_ROOT}/lib)

SET(BWAPI_INCL_DIR ${BWAPI_ROOT}/include)
INCLUDE_DIRECTORIES(
        ${BWAPI_INCL_DIR}
        ${BWAPI_INCL_DIR}/BWAPI/Client
        ${BWAPI_ROOT}/Storm
        )

SET(BWAPI_CLIENT_DIR ${BWAPI_ROOT}/BWAPIClient/Source)
INCLUDE_DIRECTORIES(${BWAPI_CLIENT_DIR})
SET(BWAPI_CLIENT_SRC
        ${BWAPI_CLIENT_DIR}/BulletImpl.cpp
        ${BWAPI_CLIENT_DIR}/Client.cpp
        ${BWAPI_CLIENT_DIR}/Command.h
        ${BWAPI_CLIENT_DIR}/Convenience.h
        ${BWAPI_CLIENT_DIR}/ForceImpl.cpp
        ${BWAPI_CLIENT_DIR}/GameImpl.cpp
        ${BWAPI_CLIENT_DIR}/PlayerImpl.cpp
        ${BWAPI_CLIENT_DIR}/RegionImpl.cpp
        ${BWAPI_CLIENT_DIR}/UnitImpl.cpp
        )
SOURCE_GROUP("BWAPI/Client" FILES ${BWAPI_CLIENT_SRC})

SET(BWAPI_UTIL_DIR ${BWAPI_ROOT}/Util/Source)
INCLUDE_DIRECTORIES(${BWAPI_UTIL_DIR})
SET(BWAPI_UTIL_SRC
        ${BWAPI_ROOT}/Storm/storm.cpp
        ${BWAPI_ROOT}/Storm/storm.h
        ${BWAPI_UTIL_DIR}/Util/Clamp.h
        ${BWAPI_UTIL_DIR}/Util/Convenience.h
        ${BWAPI_UTIL_DIR}/Util/Exceptions.cpp
        ${BWAPI_UTIL_DIR}/Util/Exceptions.h
        ${BWAPI_UTIL_DIR}/Util/MemoryFrame.cpp
        ${BWAPI_UTIL_DIR}/Util/MemoryFrame.h
        ${BWAPI_UTIL_DIR}/Util/Mutex.cpp
        ${BWAPI_UTIL_DIR}/Util/Mutex.h
        ${BWAPI_UTIL_DIR}/Util/Path.h
        ${BWAPI_UTIL_DIR}/Util/RemoteProcess.cpp
        ${BWAPI_UTIL_DIR}/Util/RemoteProcess.h
        ${BWAPI_UTIL_DIR}/Util/RemoteProcessID.h
        ${BWAPI_UTIL_DIR}/Util/sha1.cpp
        ${BWAPI_UTIL_DIR}/Util/sha1.h
        ${BWAPI_UTIL_DIR}/Util/SharedMemory.cpp
        ${BWAPI_UTIL_DIR}/Util/SharedMemory.h
        ${BWAPI_UTIL_DIR}/Util/SharedStructure.h
        ${BWAPI_UTIL_DIR}/Util/StringUtil.h
        ${BWAPI_UTIL_DIR}/Util/Types.h
        )
SOURCE_GROUP("BWAPI/Util" FILES ${BWAPI_UTIL_SRC})

SET(BWAPI_SHARED_DIR ${BWAPI_ROOT}/Shared)
INCLUDE_DIRECTORIES(${BWAPI_SHARED_DIR})
SET(BWAPI_SHARED_SRC
        ${BWAPI_SHARED_DIR}/BulletShared.cpp
        ${BWAPI_SHARED_DIR}/ForceShared.cpp
        ${BWAPI_SHARED_DIR}/GameShared.cpp
        ${BWAPI_SHARED_DIR}/PlayerShared.cpp
        ${BWAPI_SHARED_DIR}/RegionShared.cpp
        ${BWAPI_SHARED_DIR}/Templates.h
        ${BWAPI_SHARED_DIR}/UnitShared.cpp
        )
SOURCE_GROUP("BWAPI/Shared" FILES ${BWAPI_SHARED_SRC})

ADD_LIBRARY(${BWAPICLIENT_LIB_NAME} STATIC
        ${BWAPI_CLIENT_SRC}
        ${BWAPI_UTIL_SRC}
        ${BWAPI_SHARED_SRC}
        )
SET_TARGET_PROPERTIES(${BWAPICLIENT_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
