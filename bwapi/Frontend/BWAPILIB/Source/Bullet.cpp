#include <BWAPI/Bullet.h>
#include <BWAPI/Game.h>

namespace BWAPI {
  Player Bullet::getPlayer() const
  {
    return getGame().getPlayer(getData().player);
  }

  Unit Bullet::getSource() const 
  { 
    return getGame().getUnit(getData().source); 
  }

  Unit Bullet::getTarget() const 
  { 
    return getGame().getUnit(getData().target); 
  }

}
