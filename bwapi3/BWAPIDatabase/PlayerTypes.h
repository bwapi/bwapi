#include <string>
#include <map>
#include <set>

#include <BWAPITypes/PlayerType.h>
#include <BWAPITypes/PlayerTypeId.h>

#pragma once

#include <string>
#include <map>
#include <set>

#include <BWAPITypes/PlayerType.h>

namespace BWAPI
{
  namespace PlayerTypes
  {
    extern PlayerType playerTypeData[PlayerTypeIds::count];
    extern std::map<std::string, PlayerTypeId> playerTypeMap;
    extern std::set< PlayerTypeId > playerTypeSet;

    void init();
    PlayerTypeId getIdByName(const std::string& name);
  }
}
