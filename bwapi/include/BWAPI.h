#pragma once
#ifndef __BWAPI_H__
#define __BWAPI_H__

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
#include <BWAPI/Order.h>
#include <BWAPI/Player.h>
#include <BWAPI/Playerset.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/Position.h>
#include <BWAPI/Race.h>
#include <BWAPI/Region.h>
#include <BWAPI/Regionset.h>
#include <BWAPI/SetContainer.h>
#include <BWAPI/TechType.h>
#include <BWAPI/Type.h>
#include <BWAPI/Unit.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/UnitCommandType.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/UnitSizeType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/WeaponType.h>

/// <summary>The primary namespace for the BWAPI interface.</summary> Everything that is BWAPI is
/// contained within it.
namespace BWAPI
{
  /// <summary>Retrieves the revision of the BWAPILIB module currently being used.</summary>
  ///
  /// @returns
  ///   An integer representing the revision number of the library.
  ///
  /// @threadsafe
  int BWAPI_getRevision();

  /// <summary>Checks if the BWAPILIB module was compiled in DEBUG mode.</summary>
  ///
  /// @retval true if this is a DEBUG build
  /// @retval false if this is a RELEASE build
  ///
  /// @threadsafe
  bool BWAPI_isDebug();
  
  /// <summary>The client version that this header file was compiled with.</summary>
  /// Used to determine a client's compatibility with the BWAPI server process.
  ///
  /// @note This value is purposely high to avoid collisions with revision values.
  /// @m_since{4,2,0}
  const int CLIENT_VERSION = 10002;
}

#endif
