#include <BWAPI.h>
#include "BulletImpl.h"

namespace BWAPI
{
  //------------------------------------------------- GET ID -------------------------------------------------
  int BulletImpl::getID() const
  {
    return self->id;
  }
  //------------------------------------------------ GET PLAYER ----------------------------------------------
  Player* BulletImpl::getPlayer() const
  {
    return Broodwar->getPlayer(self->player);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BulletType BulletImpl::getType() const
  {
    return BulletType(self->type);
  }
  //------------------------------------------------ GET SOURCE ----------------------------------------------
  Unit* BulletImpl::getSource() const
  {
    return Broodwar->getUnit(self->source);
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  Position BulletImpl::getPosition() const
  {
    return Position(self->positionX,self->positionY);
  }
  //----------------------------------------------- GET ANGLE ------------------------------------------------
  double BulletImpl::getAngle() const
  {
    return self->angle;
  }
  //--------------------------------------------- GET VELOCITY X ---------------------------------------------
  double BulletImpl::getVelocityX() const
  {
    return self->velocityX;
  }
  //--------------------------------------------- GET VELOCITY Y ---------------------------------------------
  double BulletImpl::getVelocityY() const
  {
    return self->velocityY;
  }
  //------------------------------------------------ GET TARGET ----------------------------------------------
  Unit* BulletImpl::getTarget() const
  {
    return Broodwar->getUnit(self->target);
  }
  //-------------------------------------------- GET TARGET POSITION -----------------------------------------
  Position BulletImpl::getTargetPosition() const
  {
    return Position(self->targetPositionX,self->targetPositionY);
  }
  //-------------------------------------------- GET REMOVE TIMER --------------------------------------------
  int BulletImpl::getRemoveTimer() const
  {
    return self->removeTimer;
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool BulletImpl::exists() const
  {
    return self->exists;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible() const
  {
    if ( !Broodwar->self())
    {
      for(int i = 0; i < 9; ++i)
      {
        if (self->isVisible[i])
          return true;
      }
      return false;
    }
    return self->isVisible[Broodwar->self()->getID()];
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible(BWAPI::Player* player) const
  {
    if ( !player )
      return false;
    return self->isVisible[player->getID()];
  }
}

