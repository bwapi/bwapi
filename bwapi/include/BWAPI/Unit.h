#pragma once

#include <list>

#include <BWAPI/Order.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>

namespace BWAPI
{
  class Player;
  class Unit
  {
  public:
    virtual Player* getOwner() const=0;
    virtual UnitType getType() const=0;
    virtual int health() const=0;
    virtual int shield() const=0;
    virtual int energy() const=0;
    virtual int getKillCount() const=0;
    virtual Position getPosition() const=0;
    virtual TilePosition getTilePosition() const=0;

    virtual Unit* getTarget() const=0;
    virtual Position getTargetPosition() const=0;
    virtual Order getOrder() const=0;
    virtual Unit* getOrderTarget() const=0;
    virtual int getOrderTimer() const=0;
    virtual Order getSecondaryOrder() const=0;
    virtual Unit* getBuildUnit() const=0;
    virtual int getRemainingBuildTime() const=0;
    virtual Unit* getChild() const=0;

    virtual bool isLoaded() const=0;
    virtual bool isVisible() const=0;
    virtual bool isSelected() const=0;
    virtual bool isCompleted() const=0;
    virtual bool isLifted() const=0;
    virtual bool isBurrowed() const=0;
    virtual bool isIdle() const=0;
    virtual bool isMoving() const=0;
    virtual bool isAccelerating() const=0;
    virtual bool isBraking() const=0;
    virtual bool isStartingAttack() const=0;
    virtual bool isCloaked() const=0;
    virtual bool isDisabled() const=0;
    virtual bool isTraining() const=0;
    virtual std::list<UnitType > getTrainingQueue() const=0;

    virtual void attackMove(Position position)=0;
    virtual void rightClick(Position position)=0;
    virtual void rightClick(Unit* target)=0;
    virtual void train(UnitType type)=0;
    virtual void build(TilePosition position, UnitType type)=0;
    virtual void invent(TechType tech)=0;
    virtual void upgrade(UpgradeType upgrade)=0;
    virtual void stop()=0;
    virtual void holdPosition()=0;
    virtual void patrol(Position position)=0;
    virtual void useTech(TechType tech)=0;
    virtual void useTech(TechType tech, Position position)=0;
    virtual void useTech(TechType tech, Unit* target)=0;
  };
}
