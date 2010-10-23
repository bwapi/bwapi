#pragma once
#include "GameImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "../../BWAPI/Source/BWAPI/Templates.h"

namespace BWAPI
{
  namespace Templates
  {
    bool canBuildHere(Unit* builder, TilePosition position, UnitType type, bool checkExplored)
    {
      return _canBuildHere<class GameImpl, class PlayerImpl, class UnitImpl>(builder,position,type,checkExplored);
    }
    bool canMake(Unit* builder, UnitType type)
    {
      return _canMake<class GameImpl, class PlayerImpl, class UnitImpl>(builder,type);
    }
    bool canResearch(Unit* unit, TechType type)
    {
      return _canResearch<class GameImpl, class PlayerImpl, class UnitImpl>(unit,type);
    }
    bool canUpgrade(Unit* unit, UpgradeType type)
    {
      return _canUpgrade<class GameImpl, class PlayerImpl, class UnitImpl>(unit,type);
    }
  }
}
