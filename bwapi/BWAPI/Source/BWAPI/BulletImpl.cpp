#include "BulletImpl.h"
#include <BW/Bullet.h>
#include <BW/Offsets.h>
#include "BWAPI/GameImpl.h"
#include "BWAPI/PlayerImpl.h"
#include "BWAPI/UnitImpl.h"

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BulletImpl::BulletImpl(BW::Bullet* originalBullet,
                     u16 index)
      : bwOriginalBullet(originalBullet)
      , index(index)
      , __exists(false)
      , lastExists(false)
  {
    self=&data;
  }
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
    if (Broodwar->self()==NULL)
    {
      for(int i=0;i<9;i++)
      {
        if (self->isVisible[i]) return true;
      }
      return false;
    }
    return self->isVisible[Broodwar->self()->getID()];
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible(BWAPI::Player* player) const
  {
    if (player==NULL) return false;
    return self->isVisible[player->getID()];
  }
  //----------------------------------------------- SET EXISTS -----------------------------------------------
  void BulletImpl::setExists(bool exists)
  {
    __exists = exists;
  }
  //---------------------------------------------- SAVE EXISTS -----------------------------------------------
  void BulletImpl::saveExists()
  {
    if (lastExists==false && __exists==true)
    {
      id = BroodwarImpl.bulletCount;
      BroodwarImpl.bulletCount++;
    }
    lastExists = __exists;
  }
  //---------------------------------------------- GET RAW DATA ----------------------------------------------
  BW::Bullet* BulletImpl::getRawData() const
  {
    return this->bwOriginalBullet;
  }
  //---------------------------------------- BW BULLET TO BWAPI BULLET ---------------------------------------
  BulletImpl* BulletImpl::BWBulletToBWAPIBullet(BW::Bullet* bullet)
  {
    if (bullet == NULL)
      return NULL;

    u16 index = (u16)( ((u32)bullet - (u32)BW::BWDATA_BulletNodeTable) / 112) & 0x7F;
    if (index > BW::BULLET_ARRAY_MAX_LENGTH)
      return NULL;
    return BroodwarImpl.getBulletFromIndex(index);
  }
  void BulletImpl::updateData()
  {
    UnitImpl* _getSource = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->sourceUnit);
    Player* _getPlayer = NULL;
    if (_getSource!=NULL)
      _getPlayer = _getSource->_getPlayer;
    int selfPlayerID = BroodwarImpl.server.getPlayerID(Broodwar->self());

    bool _exists = __exists;
    if (bwOriginalBullet == NULL)
      _exists = false;
      
    if (!_exists)
      self->exists = false;
    else if (isVisible())
      self->exists = true;
    else
      self->exists = (BroodwarImpl._isReplay() || BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation));
    if (self->exists)
    {
      self->id = id;
      UnitImpl* source  = _getSource;
      if (source == NULL || !source->isAlive)
        self->player = -1;
      else
        self->player = source->_getPlayer->getID();
      self->type = bwOriginalBullet->type;

      if (source == NULL || !source->exists())
        self->source = -1;
      else
        self->source = source->getID();

      self->positionX = bwOriginalBullet->position.x;
      self->positionY = bwOriginalBullet->position.y;
      int d = bwOriginalBullet->currentDirection;
      d -= 64;
      if (d < 0)
        d += 256;
      self->angle = (double)d * 3.14159265358979323846 / 128.0;
      self->velocityX = (double)(bwOriginalBullet->current_speedX / 256.0);
      self->velocityY = (double)(bwOriginalBullet->current_speedY / 256.0);

      Unit* target = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->targetUnit);
      if (target==NULL || !target->exists())
        self->target = -1;
      else
        self->target = target->getID();
      self->targetPositionX = bwOriginalBullet->targetPosition.x;
      self->targetPositionY = bwOriginalBullet->targetPosition.y;
      self->removeTimer = bwOriginalBullet->max_time;
    }
    else
    {
      self->id = -1;
      self->player = -1;
      self->type = BulletTypes::None.getID();
      self->source = -1;
      self->positionX = Positions::None.x();
      self->positionY = Positions::None.y();
      self->angle = 0;
      self->velocityX = 0;
      self->velocityY = 0;
      self->target = -1;
      self->targetPositionX = 0;
      self->targetPositionY = 0;
      self->removeTimer = 0;
    }
    if (_exists)
    {
      for(int i=0;i<9;i++)
      {
        if (i==selfPlayerID) continue;
        PlayerImpl* player = (PlayerImpl*)Broodwar->getPlayer(i);
        if (bwOriginalBullet->sprite == NULL || player == NULL)
          self->isVisible[i]=false;
        else if (!BroodwarImpl._isReplay() && !BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
          self->isVisible[i]=false;
        else if (_getPlayer == player)
          self->isVisible[i]=true;
        else if (player->isNeutral())
          self->isVisible[i]=bwOriginalBullet->sprite->visibilityFlags > 0;
        else
          self->isVisible[i]=(bwOriginalBullet->sprite->visibilityFlags & (1 << player->getIndex())) != 0;
      }
      if (selfPlayerID>-1)
      {
        if (bwOriginalBullet->sprite == NULL)
          self->isVisible[selfPlayerID] = false;
        else if (BroodwarImpl._isReplay())
          self->isVisible[selfPlayerID] = bwOriginalBullet->sprite->visibilityFlags > 0;
        else if (_getPlayer == BWAPI::BroodwarImpl.self())
          self->isVisible[selfPlayerID] = true;
        else
          self->isVisible[selfPlayerID] = (bwOriginalBullet->sprite->visibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0;
      }
    }
    else
    {
      for(int i=0;i<9;i++)
        self->isVisible[i] = false;
    }
  }
}