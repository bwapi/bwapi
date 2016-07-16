#include <BWAPI.h>
#include <GameImpl.h>
#include <Templates.h>

namespace BWAPI
{
  //----------------------------------------------- GET FORCES -----------------------------------------------
  const Forceset& GameImpl::getForces() const
  {
    return forces;
  }
  //----------------------------------------------- GET PLAYERS ----------------------------------------------
  const Playerset& GameImpl::getPlayers() const
  {
    return playerSet;
  }
  //------------------------------------------------- GET UNITS ----------------------------------------------
  const Unitset& GameImpl::getAllUnits() const
  {
    return accessibleUnits;
  }
  //------------------------------------------------- GET MINERALS -------------------------------------------
  const Unitset& GameImpl::getMinerals() const
  {
    return minerals;
  }
  //------------------------------------------------- GET GEYSERS --------------------------------------------
  const Unitset& GameImpl::getGeysers() const
  {
    return geysers;
  }
  //------------------------------------------------- GET NEUTRAL UNITS --------------------------------------
  const Unitset& GameImpl::getNeutralUnits() const
  {
    return neutralUnits;
  }
  //------------------------------------------------- GET STATIC MINERALS ------------------------------------
  const Unitset& GameImpl::getStaticMinerals() const
  {
    return staticMinerals;
  }
  //------------------------------------------------- GET STATIC GEYSERS -------------------------------------
  const Unitset& GameImpl::getStaticGeysers() const
  {
    return staticGeysers;
  }
  //------------------------------------------------- GET STATIC NEUTRAL UNITS -------------------------------
  const Unitset& GameImpl::getStaticNeutralUnits() const
  {
    return staticNeutralUnits;
  }
  //------------------------------------------------ GET BULLETS ---------------------------------------------
  const Bulletset& GameImpl::getBullets() const
  {
    return bullets;
  }
  //------------------------------------------------ GET NUKE DOTS -------------------------------------------
  const Position::list& GameImpl::getNukeDots() const
  {
    return nukeDots;
  }
  //------------------------------------------------ GET EVENTS ----------------------------------------------
  const std::list< Event >& GameImpl::getEvents() const
  {
    return events;
  }
  //----------------------------------------------- GET LAST ERROR -------------------------------------------
  Error GameImpl::getLastError() const
  {
    // returns the last error encountered in BWAPI
    return lastError;
  }
  //--------------------------------------------- SET LAST ERROR ---------------------------------------------
  bool GameImpl::setLastError(BWAPI::Error e) const
  {
    // implies that an error has occured
    lastError = e;
    return e == Errors::None;
  }
  //--------------------------------------------- CAN BUILD HERE ---------------------------------------------
  bool GameImpl::canBuildHere(TilePosition position, UnitType type, Unit builder, bool checkExplored)
  {
    return Templates::canBuildHere(builder,position,type,checkExplored);
  }
  //--------------------------------------------- CAN MAKE ---------------------------------------------------
  bool GameImpl::canMake(UnitType type, Unit builder) const
  {
    return Templates::canMake(builder,type);
  }
  //--------------------------------------------- CAN RESEARCH -----------------------------------------------
  bool GameImpl::canResearch(TechType type, Unit unit, bool checkCanIssueCommandType)
  {
    return Templates::canResearch(unit,type,checkCanIssueCommandType);
  }
  //--------------------------------------------- CAN UPGRADE ------------------------------------------------
  bool GameImpl::canUpgrade(UpgradeType type, Unit unit, bool checkCanIssueCommandType)
  {
    return Templates::canUpgrade(unit,type,checkCanIssueCommandType);
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  const TilePosition::list& GameImpl::getStartLocations() const
  {
    return startLocations;
  }
  //----------------------------------------------- GET ALL REGIONS ------------------------------------------
  const Regionset &GameImpl::getAllRegions() const
  {
    return this->regionsList;
  }
  //------------------------------------------------- GAME DATA ----------------------------------------------
  const GameData* GameImpl::getGameData() const
  {
    return data;
  }
};

