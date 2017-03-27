project(BWAPI-Client CXX)

# Define this variable BWAPI_CUSTOM_COMPILE_FLAGS to set your compile flags
IF(BWAPI_CUSTOM_COMPILE_FLAGS)
    ADD_DEFINITIONS(${BWAPI_CUSTOM_COMPILE_FLAGS})
ENDIF(BWAPI_CUSTOM_COMPILE_FLAGS)

SET(BWAPI_DLL_NAME BWAPI)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(BWAPI_LIB_NAME "BWAPI-Staticd")
    SET(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
ELSE()
    SET(BWAPI_LIB_NAME "BWAPI-Static")
    SET(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
ENDIF()

ADD_DEFINITIONS(/DNOMINMAX=1)

GET_FILENAME_COMPONENT(BWAPI_ROOT ../../bwapi ABSOLUTE)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BWAPI_ROOT}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BWAPI_ROOT}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BWAPI_ROOT}/lib)

SET(BWAPI_INCL_DIR ${BWAPI_ROOT}/include)
INCLUDE_DIRECTORIES(
    ${BWAPI_INCL_DIR}
    ${BWAPI_ROOT}/BWAPICore
    ${BWAPI_ROOT}/Storm
    )

SET(BWAPI_INCLUDES
    ${BWAPI_INCL_DIR}/BWAPI.h
    ${BWAPI_INCL_DIR}/BWAPI/Client.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/BulletData.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/BulletImpl.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/Client.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/Command.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/CommandType.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/Event.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/ForceData.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/ForceImpl.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/GameData.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/GameImpl.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/GameTable.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/PlayerData.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/PlayerImpl.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/RegionData.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/RegionImpl.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/Shape.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/ShapeType.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/UnitCommand.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/UnitData.h
    ${BWAPI_INCL_DIR}/BWAPI/Client/UnitImpl.h
    ${BWAPI_INCL_DIR}/BWAPI/AIModule.h
    ${BWAPI_INCL_DIR}/BWAPI/ArithmaticFilter.h
    ${BWAPI_INCL_DIR}/BWAPI/BestFilter.h
    ${BWAPI_INCL_DIR}/BWAPI/BroodwarOutputDevice.h
    ${BWAPI_INCL_DIR}/BWAPI/Bullet.h
    ${BWAPI_INCL_DIR}/BWAPI/Bulletset.h
    ${BWAPI_INCL_DIR}/BWAPI/BulletType.h
    ${BWAPI_INCL_DIR}/BWAPI/Client.h
    ${BWAPI_INCL_DIR}/BWAPI/Color.h
    ${BWAPI_INCL_DIR}/BWAPI/ComparisonFilter.h
    ${BWAPI_INCL_DIR}/BWAPI/Constants.h
    ${BWAPI_INCL_DIR}/BWAPI/CoordinateType.h
    ${BWAPI_INCL_DIR}/BWAPI/DamageType.h
    ${BWAPI_INCL_DIR}/BWAPI/Error.h
    ${BWAPI_INCL_DIR}/BWAPI/Event.h
    ${BWAPI_INCL_DIR}/BWAPI/EventType.h
    ${BWAPI_INCL_DIR}/BWAPI/ExplosionType.h
    ${BWAPI_INCL_DIR}/BWAPI/Filters.h
    ${BWAPI_INCL_DIR}/BWAPI/Flag.h
    ${BWAPI_INCL_DIR}/BWAPI/Force.h
    ${BWAPI_INCL_DIR}/BWAPI/Forceset.h
    ${BWAPI_INCL_DIR}/BWAPI/Game.h
    ${BWAPI_INCL_DIR}/BWAPI/GameType.h
    ${BWAPI_INCL_DIR}/BWAPI/Input.h
    ${BWAPI_INCL_DIR}/BWAPI/Interface.h
    ${BWAPI_INCL_DIR}/BWAPI/InterfaceEvent.h
    ${BWAPI_INCL_DIR}/BWAPI/Latency.h
    ${BWAPI_INCL_DIR}/BWAPI/Order.h
    ${BWAPI_INCL_DIR}/BWAPI/Player.h
    ${BWAPI_INCL_DIR}/BWAPI/Playerset.h
    ${BWAPI_INCL_DIR}/BWAPI/PlayerType.h
    ${BWAPI_INCL_DIR}/BWAPI/Position.h
    ${BWAPI_INCL_DIR}/BWAPI/Race.h
    ${BWAPI_INCL_DIR}/BWAPI/Region.h
    ${BWAPI_INCL_DIR}/BWAPI/Regionset.h
    ${BWAPI_INCL_DIR}/BWAPI/SetContainer.h
    ${BWAPI_INCL_DIR}/BWAPI/Streams.h
    ${BWAPI_INCL_DIR}/BWAPI/TechType.h
    ${BWAPI_INCL_DIR}/BWAPI/TournamentAction.h
    ${BWAPI_INCL_DIR}/BWAPI/Type.h
    ${BWAPI_INCL_DIR}/BWAPI/UnaryFilter.h
    ${BWAPI_INCL_DIR}/BWAPI/Unit.h
    ${BWAPI_INCL_DIR}/BWAPI/UnitCommand.h
    ${BWAPI_INCL_DIR}/BWAPI/UnitCommandType.h
    ${BWAPI_INCL_DIR}/BWAPI/Unitset.h
    ${BWAPI_INCL_DIR}/BWAPI/UnitSizeType.h
    ${BWAPI_INCL_DIR}/BWAPI/UnitType.h
    ${BWAPI_INCL_DIR}/BWAPI/UpgradeType.h
    )
SOURCE_GROUP("BWAPI" FILES ${BWAPI_INCLUDES})

SET(BWAPI_BWAPI_DIR ${BWAPI_ROOT}/BWAPI/Source)
INCLUDE_DIRECTORIES(${BWAPI_BWAPI_DIR} ${BWAPI_BWAPI_DIR}/BWAPI)

SET(BWAPI_BWAPILIB_DIR ${BWAPI_ROOT}/BWAPILib/Source)

SET(BWAPI_BWAPI_SRC
    ${BWAPI_BWAPI_DIR}/Assembly.cpp
    ${BWAPI_BWAPI_DIR}/Assembly.h
    ${BWAPI_BWAPI_DIR}/BW/AIController.h
    ${BWAPI_BWAPI_DIR}/BW/Bitmap.cpp
    ${BWAPI_BWAPI_DIR}/BW/Bitmap.h
    ${BWAPI_BWAPI_DIR}/BW/BNETCommands.h
    ${BWAPI_BWAPI_DIR}/BW/CBullet.h 
    ${BWAPI_BWAPI_DIR}/BW/CheatFlags.h
    ${BWAPI_BWAPI_DIR}/BW/CheatType.cpp
    ${BWAPI_BWAPI_DIR}/BW/CheatType.h
    ${BWAPI_BWAPI_DIR}/BW/CImage.cpp
    ${BWAPI_BWAPI_DIR}/BW/CImage.h
    ${BWAPI_BWAPI_DIR}/BW/Constants.h
    ${BWAPI_BWAPI_DIR}/BW/COrder.h
    ${BWAPI_BWAPI_DIR}/BW/CSprite.cpp
    ${BWAPI_BWAPI_DIR}/BW/CSprite.h
    ${BWAPI_BWAPI_DIR}/BW/CThingy.cpp 
    ${BWAPI_BWAPI_DIR}/BW/CThingy.h
    ${BWAPI_BWAPI_DIR}/BW/CUnit.cpp 
    ${BWAPI_BWAPI_DIR}/BW/CUnit.h
    ${BWAPI_BWAPI_DIR}/BW/Dialog.cpp
    ${BWAPI_BWAPI_DIR}/BW/Dialog.h
    ${BWAPI_BWAPI_DIR}/BW/FlingyID.h
    ${BWAPI_BWAPI_DIR}/BW/Font.cpp
    ${BWAPI_BWAPI_DIR}/BW/Font.h
    ${BWAPI_BWAPI_DIR}/BW/GroupFlags.h
    ${BWAPI_BWAPI_DIR}/BW/Internal.cpp
    ${BWAPI_BWAPI_DIR}/BW/MenuPosition.h
    ${BWAPI_BWAPI_DIR}/BW/MiniTileFlags.h
    ${BWAPI_BWAPI_DIR}/BW/MovementFlags.h
    ${BWAPI_BWAPI_DIR}/BW/Offsets.h
    ${BWAPI_BWAPI_DIR}/BW/OrderFlags.h
    ${BWAPI_BWAPI_DIR}/BW/OrderTypes.cpp
    ${BWAPI_BWAPI_DIR}/BW/OrderTypes.h
    ${BWAPI_BWAPI_DIR}/BW/Path.cpp 
    ${BWAPI_BWAPI_DIR}/BW/Path.h
    ${BWAPI_BWAPI_DIR}/BW/Pathing.cpp 
    ${BWAPI_BWAPI_DIR}/BW/Pathing.h
    ${BWAPI_BWAPI_DIR}/BW/PlayerID.h 
    ${BWAPI_BWAPI_DIR}/BW/Position.h
    ${BWAPI_BWAPI_DIR}/BW/PositionUnitTarget.cpp
    ${BWAPI_BWAPI_DIR}/BW/PositionUnitTarget.h
    ${BWAPI_BWAPI_DIR}/BW/Render.cpp
    ${BWAPI_BWAPI_DIR}/BW/Render.h
    ${BWAPI_BWAPI_DIR}/BW/Structures.h
    ${BWAPI_BWAPI_DIR}/BW/Target.cpp
    ${BWAPI_BWAPI_DIR}/BW/Target.h
    ${BWAPI_BWAPI_DIR}/BW/TileSet.cpp
    ${BWAPI_BWAPI_DIR}/BW/TileSet.h
    ${BWAPI_BWAPI_DIR}/BW/TileType.h
    ${BWAPI_BWAPI_DIR}/BW/TriggerEngine.cpp
    ${BWAPI_BWAPI_DIR}/BW/TriggerEngine.h
    ${BWAPI_BWAPI_DIR}/BW/Triggers.h
    ${BWAPI_BWAPI_DIR}/BW/UnitPrototypeFlags.h
    ${BWAPI_BWAPI_DIR}/BW/UnitStatusFlags.h
    ${BWAPI_BWAPI_DIR}/BW/UnitTarget.cpp
    ${BWAPI_BWAPI_DIR}/BW/UnitTarget.h 
    ${BWAPI_BWAPI_DIR}/BW/WeaponTargetFlags.h
    ${BWAPI_BWAPI_DIR}/BWAPI/AutoMenuManager.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/AutoMenuManager.h
    ${BWAPI_BWAPI_DIR}/BWAPI/BulletImpl.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/BulletImpl.h
    ${BWAPI_BWAPI_DIR}/BWAPI/BWtoBWAPI.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/BWtoBWAPI.h
    ${BWAPI_BWAPI_DIR}/BWAPI/Command.h
    ${BWAPI_BWAPI_DIR}/BWAPI/CommandOptimizer.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/CommandOptimizer.h
    ${BWAPI_BWAPI_DIR}/BWAPI/CommandTemp.h
    ${BWAPI_BWAPI_DIR}/BWAPI/ForceImpl.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/ForceImpl.h
    ${BWAPI_BWAPI_DIR}/BWAPI/GameBullets.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/GameCommands.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/GameDrawing.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/GameEvents.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/GameImpl.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/GameImpl.h
    ${BWAPI_BWAPI_DIR}/BWAPI/GameInternals.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/GameMenu.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/GameUnits.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/Map.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/Map.h
    ${BWAPI_BWAPI_DIR}/BWAPI/PlayerImpl.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/PlayerImpl.h
    ${BWAPI_BWAPI_DIR}/BWAPI/RegionImpl.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/RegionImpl.h
    ${BWAPI_BWAPI_DIR}/BWAPI/Server.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/Server.h
    ${BWAPI_BWAPI_DIR}/BWAPI/UnitImpl.cpp
    ${BWAPI_BWAPI_DIR}/BWAPI/UnitImpl.h
    ${BWAPI_BWAPI_DIR}/BWAPI/UnitUpdate.cpp
    ${BWAPI_BWAPI_DIR}/CodePatch.cpp
    ${BWAPI_BWAPI_DIR}/CodePatch.h
    ${BWAPI_BWAPI_DIR}/Config.cpp
    ${BWAPI_BWAPI_DIR}/Config.h
    ${BWAPI_BWAPI_DIR}/Detours.cpp
    ${BWAPI_BWAPI_DIR}/Detours.h
    ${BWAPI_BWAPI_DIR}/DLLMain.cpp
    ${BWAPI_BWAPI_DIR}/DLLMain.h
    ${BWAPI_BWAPI_DIR}/ExceptionFilter.cpp
    ${BWAPI_BWAPI_DIR}/ExceptionFilter.h
    ${BWAPI_BWAPI_DIR}/GameUpdate.cpp
    ${BWAPI_BWAPI_DIR}/Graphics.cpp
    ${BWAPI_BWAPI_DIR}/Graphics.h
    ${BWAPI_BWAPI_DIR}/NewHackUtil.cpp
    ${BWAPI_BWAPI_DIR}/NewHackUtil.h
    ${BWAPI_BWAPI_DIR}/Resolution.cpp
    ${BWAPI_BWAPI_DIR}/Resolution.h
    ${BWAPI_BWAPI_DIR}/Thread.cpp
    ${BWAPI_BWAPI_DIR}/Thread.h
    ${BWAPI_BWAPI_DIR}/WMode.cpp
    ${BWAPI_BWAPI_DIR}/WMode.h
    ${BWAPI_BWAPILIB_DIR}/AIModule.cpp
    ${BWAPI_BWAPILIB_DIR}/BroodwarOutputDevice.cpp
    ${BWAPI_BWAPILIB_DIR}/BulletType.cpp
    ${BWAPI_BWAPILIB_DIR}/BWAPI.cpp
    ${BWAPI_BWAPILIB_DIR}/Color.cpp
    ${BWAPI_BWAPILIB_DIR}/DamageType.cpp
    ${BWAPI_BWAPILIB_DIR}/Error.cpp
    ${BWAPI_BWAPILIB_DIR}/Event.cpp
    ${BWAPI_BWAPILIB_DIR}/ExplosionType.cpp
    ${BWAPI_BWAPILIB_DIR}/Filters.cpp
    ${BWAPI_BWAPILIB_DIR}/Forceset.cpp
    ${BWAPI_BWAPILIB_DIR}/Game.cpp
    ${BWAPI_BWAPILIB_DIR}/GameType.cpp
    ${BWAPI_BWAPILIB_DIR}/Order.cpp
    ${BWAPI_BWAPILIB_DIR}/Player.cpp
    ${BWAPI_BWAPILIB_DIR}/Playerset.cpp
    ${BWAPI_BWAPILIB_DIR}/PlayerType.cpp
    ${BWAPI_BWAPILIB_DIR}/Position.cpp
    ${BWAPI_BWAPILIB_DIR}/Race.cpp
    ${BWAPI_BWAPILIB_DIR}/Region.cpp
    ${BWAPI_BWAPILIB_DIR}/Regionset.cpp
    ${BWAPI_BWAPILIB_DIR}/Streams.cpp
    ${BWAPI_BWAPILIB_DIR}/TechType.cpp
    ${BWAPI_BWAPILIB_DIR}/Unit.cpp
    ${BWAPI_BWAPILIB_DIR}/UnitCommandType.cpp
    ${BWAPI_BWAPILIB_DIR}/Unitset.cpp
    ${BWAPI_BWAPILIB_DIR}/UnitSizeType.cpp
    ${BWAPI_BWAPILIB_DIR}/UnitType.cpp
    ${BWAPI_BWAPILIB_DIR}/UpgradeType.cpp
    ${BWAPI_BWAPILIB_DIR}/WeaponType.cpp
    ${BWAPI_ROOT}/BWAPICore/APMCounter.cpp
    ${BWAPI_ROOT}/BWAPICore/APMCounter.h
    ${BWAPI_ROOT}/BWAPICore/FPSCounter.cpp
    ${BWAPI_ROOT}/BWAPICore/FPSCounter.h
    ${BWAPI_ROOT}/BWAPILib/UnitCommand.cpp
    )
SOURCE_GROUP("BWAPI/BWAPI" FILES ${BWAPI_BWAPI_SRC})

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
    ${BWAPI_ROOT}/svnrev.h
    ${BWAPI_SHARED_DIR}/BulletShared.cpp        
    ${BWAPI_SHARED_DIR}/ForceShared.cpp
    ${BWAPI_SHARED_DIR}/GameShared.cpp          
    ${BWAPI_SHARED_DIR}/PlayerShared.cpp
    ${BWAPI_SHARED_DIR}/RegionShared.cpp        
    ${BWAPI_SHARED_DIR}/Templates.h
    ${BWAPI_SHARED_DIR}/UnitShared.cpp
    )
SOURCE_GROUP("BWAPI/Shared" FILES ${BWAPI_SHARED_SRC})

ADD_CUSTOM_COMMAND(OUTPUT ${BWAPI_ROOT}/svnrev.h PRE_BUILD
        COMMAND cscript.exe ${BWAPI_ROOT}/revisionUpdate.vbs
        WORKING_DIRECTORY ${BWAPI_ROOT})

SET(BWAPI_EVERYTHING
        ${BWAPI_BWAPI_SRC}
        ${BWAPI_INCLUDES}
        ${BWAPI_SHARED_SRC}
        ${BWAPI_UTIL_SRC}
        )
ADD_LIBRARY(${BWAPI_LIB_NAME} STATIC ${BWAPI_EVERYTHING})
SET_TARGET_PROPERTIES(${BWAPI_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

#
#--- DLL goes here ---------------------
# Actually it doesn't. It freezes and never starts.
# TODO: Setting up correct compile flags
#

#ADD_DEFINITIONS(/DYNAMICBASE:NO)
#
#LINK_LIBRARIES(version)
#ADD_LIBRARY(${BWAPI_DLL_NAME} SHARED ${BWAPI_EVERYTHING})
#SET_TARGET_PROPERTIES(${BWAPI_DLL_NAME} PROPERTIES LINKER_LANGUAGE CXX)
