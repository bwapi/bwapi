#pragma once
#include <BWAPI.h>
#include "ForceData.h"
#include <string>

namespace BWAPI
{
  class Game;
  class PlayerInterface;
  typedef PlayerInterface *Player;

  class ForceImpl : public ForceInterface
  {
    private:
      const ForceData* self;
      int id;
    public:
      ForceImpl(int id);
      virtual int getID() const override;
      virtual std::string getName() const override;
      virtual Playerset getPlayers() const override;
  };
}