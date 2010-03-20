PROJECT(BWAPILIB_cmake)

SET(SOURCES 
  Source/AIModule.cpp
  Source/AttackType.cpp
  Source/BWAPI.cpp
  Source/Color.cpp
  Source/DamageType.cpp
  Source/Error.cpp
  Source/ExplosionType.cpp
  Source/Order.cpp
  Source/PlayerType.cpp
  Source/Position.cpp
  Source/Race.cpp
  Source/TechType.cpp
  Source/TilePosition.cpp
  Source/UnitSizeType.cpp
  Source/UnitType.cpp
  Source/UpgradeType.cpp
  Source/WeaponType.cpp
)

INCLUDE_DIRECTORIES(
  ../Util/Source
  ../include
)

IF (BWAPI_FAST_BUILD)
  ADD_LIBRARY(BWAPILIB_cmake STATIC Source/fastbuild.cpp)
ELSE ()
  ADD_LIBRARY(BWAPILIB_cmake STATIC ${SOURCES})
ENDIF ()

SET_TARGET_PROPERTIES(BWAPILIB_cmake PROPERTIES PROJECT_LABEL BWAPILIB)
SET_TARGET_PROPERTIES(BWAPILIB_cmake PROPERTIES OUTPUT_NAME BWAPILIB)

#-------------------------------------------------------------------------------------------------------------
# HACK: Avoid collisions with VC++ "Rebuild Solution" building since BWAPI.lib is actually 
# build from BWAPILIB_cmake and not BWAPI_cmake
GET_TARGET_PROPERTY(BWAPILIB_OUTPUT BWAPILIB_cmake LOCATION)

GET_FILENAME_COMPONENT(COPY_SRC "${BWAPILIB_OUTPUT}" ABSOLUTE)
GET_FILENAME_COMPONENT(COPY_PATH "${BWAPILIB_OUTPUT}" PATH)
SET(COPY_DST "${COPY_PATH}/BWAPI.lib")

ADD_CUSTOM_COMMAND(TARGET BWAPILIB_cmake POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${COPY_SRC}" "${COPY_DST}")