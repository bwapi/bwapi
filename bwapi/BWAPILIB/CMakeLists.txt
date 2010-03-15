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

IF (FAST_BUILD)
  ADD_LIBRARY(BWAPILIB_cmake STATIC Source/fastbuild.cpp)
ELSE ()
  ADD_LIBRARY(BWAPILIB_cmake STATIC ${SOURCES})
ENDIF ()

SET_TARGET_PROPERTIES(BWAPILIB_cmake PROPERTIES PROJECT_LABEL BWAPILIB)
SET_TARGET_PROPERTIES(BWAPILIB_cmake PROPERTIES OUTPUT_NAME BWAPILIB)