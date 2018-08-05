#include <BWAPI.h>
#include "BulletImpl.h"
#include "GameImpl.h"

namespace BWAPI
{
  //------------------------------------------------- GET ID -------------------------------------------------
  int BulletImpl::getID() const
  {
    return data.id;
  }
  //------------------------------------------------ GET PLAYER ----------------------------------------------
  Player BulletImpl::getPlayer() const
  {
    return BroodwarImpl.getPlayer(data.player);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BulletType BulletImpl::getType() const
  {
    return BulletType(data.type);
  }
  //------------------------------------------------ GET SOURCE ----------------------------------------------
  Unit BulletImpl::getSource() const
  {
    return BroodwarImpl.getUnit(data.source);
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  Position BulletImpl::getPosition() const
  {
    return Position(data.positionX, data.positionY);
  }
  //----------------------------------------------- GET ANGLE ------------------------------------------------
  double BulletImpl::getAngle() const
  {
    return data.angle;
  }
  //--------------------------------------------- GET VELOCITY X ---------------------------------------------
  double BulletImpl::getVelocityX() const
  {
    return data.velocityX;
  }
  //--------------------------------------------- GET VELOCITY Y ---------------------------------------------
  double BulletImpl::getVelocityY() const
  {
    return data.velocityY;
  }
  //------------------------------------------------ GET TARGET ----------------------------------------------
  Unit BulletImpl::getTarget() const
  {
    return BroodwarImpl.getUnit(data.target);
  }
  //-------------------------------------------- GET TARGET POSITION -----------------------------------------
  Position BulletImpl::getTargetPosition() const
  {
    return Position(data.targetPositionX, data.targetPositionY);
  }
  //-------------------------------------------- GET REMOVE TIMER --------------------------------------------
  int BulletImpl::getRemoveTimer() const
  {
    return data.removeTimer;
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool BulletImpl::exists() const
  {
    return data.exists;
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
        return data.isVisible[id]; // return visibility
    }
    else  // fallback for self also being null
    {
      for(int i = 0; i < 9; ++i) // check if the bullet is visible by ANY player
      {
        if (data.isVisible[i] )
          return true;
      }
    }
    return false;
  }
}

