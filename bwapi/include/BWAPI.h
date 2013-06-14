#pragma once
#include <BWAPI/InterfaceEvent.h>
#include <BWAPI/Interface.h>
#include <BWAPI/AIModule.h>
#include <BWAPI/Bullet.h>
#include <BWAPI/Bulletset.h>
#include <BWAPI/BulletType.h>
#include <BWAPI/Color.h>
#include <BWAPI/Constants.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/DamageType.h>
#include <BWAPI/Error.h>
#include <BWAPI/Event.h>
#include <BWAPI/EventType.h>
#include <BWAPI/ExplosionType.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Flag.h>
#include <BWAPI/Force.h>
#include <BWAPI/Forceset.h>
#include <BWAPI/Game.h>
#include <BWAPI/GameType.h>
#include <BWAPI/Input.h>
#include <BWAPI/Latency.h>
#include <BWAPI/Order.h>
#include <BWAPI/Player.h>
#include <BWAPI/Playerset.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/Position.h>
#include <BWAPI/PositionUnit.h>
#include <BWAPI/Race.h>
#include <BWAPI/Region.h>
#include <BWAPI/Regionset.h>
#include <BWAPI/TechType.h>
#include <BWAPI/TournamentAction.h>
#include <BWAPI/Type.h>
#include <BWAPI/Unit.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/UnitCommandType.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/UnitSizeType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Vectorset.h>
#include <BWAPI/WeaponType.h>

namespace BWAPI
{
  /// Retrieves the revision of the BWAPILIB module currently being used.
  ///
  /// @returns
  ///   An integer representing the revision number of the library.
  ///
  /// @threadsafe
  int BWAPI_getRevision();

  /// Checks if the BWAPILIB module was compiled in DEBUG mode.
  ///
  /// @retval true if this is a DEBUG build
  /// @retval false if this is a RELEASE build
  ///
  /// @threadsafe
  bool BWAPI_isDebug();
}
