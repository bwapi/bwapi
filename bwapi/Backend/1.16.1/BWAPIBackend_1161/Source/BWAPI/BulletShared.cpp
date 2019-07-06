#include <BWAPI.h>
#include "BulletImpl.h"
#include "GameImpl.h"

namespace BWAPI
{
  //------------------------------------------------- GET ID -------------------------------------------------
  int BulletImpl::getID() const
  {
    return self->id;
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool BulletImpl::exists() const
  {
    return self->exists;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible(BWAPI::Player player) const
  {
    if ( player == nullptr )  // Default to self
      player = BroodwarImpl.self();

    if ( player != nullptr ) // Only if player is valid
    {
      int id = player->getID();
      if ( id >= 0 && id < 9 ) // Check if ID is valid
        return self->isVisible[id]; // return visibility
    }
    else  // fallback for self also being null
    {
      for(int i = 0; i < 9; ++i) // check if the bullet is visible by ANY player
      {
        if ( self->isVisible[i] )
          return true;
      }
    }
    return false;
  }
}

