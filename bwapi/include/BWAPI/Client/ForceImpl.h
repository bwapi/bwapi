#pragma once
#include <BWAPI.h>
#include "ForceData.h"
#include <set>
#include <string>

namespace BWAPI
{
  class Game;
  class Player;
  class Unit;
  class ForceImpl : public Force
  {
    private:
      const BWAPIC::ForceData* self;
      int id;
    public:
      ForceImpl(int id);
      virtual int getID() const;
      virtual std::string getName() const;
      virtual std::set<Player*> getPlayers() const;
  };
}