#pragma once
#include "BWSLInterface.h"

IP_STRIP

#include <set>
#include <string>

namespace BWSL
{
  class Player;
  class Force
  {
  private:
    int id;
  public :
    Force(int id);
    BWSL_EXPORT std::string getName() const;
    BWSL_EXPORT std::set<Player*> getPlayers() const;
  };
}
