PROJECT(BWAPI_cmake)

SET(SOURCES 
  Source/BW/CheatType.cpp
  Source/BW/OrderTypes.cpp
  Source/BW/Position.cpp
  Source/BW/PositionUnitTarget.cpp
  Source/BW/Race.cpp
  Source/BW/TechType.cpp
  Source/BW/TilePosition.cpp
  Source/BW/TileSet.cpp
  Source/BW/UnitTarget.cpp
  Source/BW/UnitType.cpp
  Source/BW/UpgradeType.cpp
  Source/BW/WeaponType.cpp
  Source/BWAPI/Command.cpp
  Source/BWAPI/CommandAttackMove.cpp
  Source/BWAPI/CommandAttackUnit.cpp
  Source/BWAPI/CommandBuild.cpp
  Source/BWAPI/CommandBurrow.cpp
  Source/BWAPI/CommandCancelAddon.cpp
  Source/BWAPI/CommandCancelConstruction.cpp
  Source/BWAPI/CommandCancelMorph.cpp
  Source/BWAPI/CommandCancelResearch.cpp
  Source/BWAPI/CommandCancelTrain.cpp
  Source/BWAPI/CommandCancelUpgrade.cpp
  Source/BWAPI/CommandCloak.cpp
  Source/BWAPI/CommandDecloak.cpp
  Source/BWAPI/CommandFollow.cpp
  Source/BWAPI/CommandHoldPosition.cpp
  Source/BWAPI/CommandInvent.cpp
  Source/BWAPI/CommandLand.cpp
  Source/BWAPI/CommandLift.cpp
  Source/BWAPI/CommandLoad.cpp
  Source/BWAPI/CommandMorphBuilding.cpp
  Source/BWAPI/CommandMorphUnit.cpp
  Source/BWAPI/CommandPatrol.cpp
  Source/BWAPI/CommandRepair.cpp
  Source/BWAPI/CommandReturnCargo.cpp
  Source/BWAPI/CommandRightClick.cpp
  Source/BWAPI/CommandSetRally.cpp
  Source/BWAPI/CommandSiege.cpp
  Source/BWAPI/CommandStop.cpp
  Source/BWAPI/CommandTrain.cpp
  Source/BWAPI/CommandUnburrow.cpp
  Source/BWAPI/CommandUnload.cpp
  Source/BWAPI/CommandUnloadAll.cpp
  Source/BWAPI/CommandUnsiege.cpp
  Source/BWAPI/CommandUpgrade.cpp
  Source/BWAPI/ForceImpl.cpp
  Source/BWAPI/GameImpl.cpp
  Source/BWAPI/Map.cpp
  Source/BWAPI/PlayerImpl.cpp
  Source/BWAPI/ScreenLogger.cpp
  Source/BWAPI/ShapeBox.cpp
  Source/BWAPI/ShapeCircle.cpp
  Source/BWAPI/ShapeDot.cpp
  Source/BWAPI/ShapeEllipse.cpp
  Source/BWAPI/ShapeLine.cpp
  Source/BWAPI/ShapeText.cpp
  Source/BWAPI/ShapeTriangle.cpp
  Source/BWAPI/UnitImpl.cpp
  Source/DLLMain.cpp
  Source/NewHackUtil.cpp
)

INCLUDE_DIRECTORIES( 
  ../Util/Source
  ../include
  Source
  
  # FIXME: This is for the #include "../../svnrev.h" which is created in the build directory
  ${CMAKE_CURRENT_BINARY_DIR}/Source
)

ADD_DEFINITIONS(
  -D_CRT_SECURE_NO_WARNINGS
)

IF (BWAPI_FAST_BUILD)
  ADD_LIBRARY(BWAPI_cmake MODULE Source/fastbuild.cpp)
ELSE ()
  ADD_LIBRARY(BWAPI_cmake MODULE ${SOURCES})
ENDIF ()

TARGET_LINK_LIBRARIES(BWAPI_cmake BWAPILIB_cmake Util_cmake)
SET_TARGET_PROPERTIES(BWAPI_cmake PROPERTIES PROJECT_LABEL BWAPI)
SET_TARGET_PROPERTIES(BWAPI_cmake PROPERTIES OUTPUT_NAME BWAPI)
