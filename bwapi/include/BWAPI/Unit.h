#pragma once

#include <list>

#include <BWAPI/Order.h>
#include <BWAPI/TechType.h>
#include <BW/UnitType.h>
#include <BW/UpgradeType.h>
#include <BW/Position.h>
#include <BW/TilePosition.h>
#include <Util/Types.h>

namespace BWAPI
{
  class Player;
  class Unit
  {
  public:
    virtual Player* getOwner() const=0;
    virtual BW::UnitType getType() const=0;
    virtual u16 health() const=0;
    virtual u32 shield() const=0;
    virtual u16 energy() const=0;
    virtual const BW::Position& getPosition() const=0;
    virtual BW::TilePosition getTilePosition() const=0;

    virtual Unit* getTarget() const=0;
    virtual BW::Position getTargetPosition() const=0;
    virtual Order getOrderID() const=0;
    virtual Unit* getOrderTarget() const=0;
    virtual u8 getOrderTimer() const=0;
    virtual Order getSecondaryOrderID() const=0;
    virtual Unit* getBuildUnit() const=0;
    virtual u16 getRemainingBuildTime() const=0;
    virtual Unit* getChild() const=0;

    virtual bool isLoaded() const=0;
    virtual bool isVisible() const=0;
    virtual bool isSelected() const=0;
    virtual bool isCompleted() const=0;
    virtual bool isLifted() const=0;
    virtual bool isBurrowed() const=0;
    virtual bool isIdle() const=0;
    virtual bool isCloaked() const=0;
    virtual bool isDisabled() const=0;
    virtual bool isTraining() const=0;
    virtual bool isMineral() const=0;
    virtual std::list<BW::UnitType > getTrainingQueue() const=0;

    virtual void attackLocation(BW::Position position, BWAPI::Order order)=0;
    virtual void rightClick(BW::Position position)=0;
    virtual void rightClick(Unit *target)=0;
    virtual void train(BW::UnitType type)=0;
    virtual void build(BW::TilePosition position, BW::UnitType type)=0;
    virtual void invent(TechType tech)=0;
    virtual void upgrade(BW::UpgradeType upgrade)=0;
    virtual void stop()=0;
    virtual void holdPosition()=0;
    virtual void patrol(BW::Position position)=0;
    virtual void useTech(TechType tech)=0;
    virtual void useTech(TechType tech, BW::Position position)=0;
    virtual void useTech(TechType tech, Unit* target)=0;
  };
}
