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
  }
}
