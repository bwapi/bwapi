#pragma once
#include <BWAPI.h>
#include <Util/Foreach.h>
namespace BWAPI
{
  namespace Templates
  {
    //------------------------------------------- CAN BUILD HERE ---------------------------------------------
    template <class GameImpl, class PlayerImpl, class UnitImpl>
    bool _canBuildHere(Unit* builder, TilePosition position, UnitType type, bool checkExplored)
    {
      Broodwar->setLastError(Errors::Unbuildable_Location);
      int width  = type.tileWidth();
      int height = type.tileHeight();

      int left   = position.x();
      int top    = position.y();
      int right  = left + width;
      int bottom = top + height;

      /* Map limit Check */
      if (left < 0) // left
        return false;
      if (top < 0) // top
        return false;
      if (right > Broodwar->mapWidth()) // right
        return false;
      if (bottom >= Broodwar->mapHeight()) // bottom
        return false;

      //if the unit is a refinery, we just need to check the set of geysers to see if the position
      //matches one of them (and the type is still vespene geyser)
      if ( type.isRefinery() )
      {
        foreach (Unit* g, Broodwar->getGeysers())
        {
          if (g->getTilePosition() == position)
          {
            if (g->isVisible() && g->getType() != UnitTypes::Resource_Vespene_Geyser)
              return false;
            return Broodwar->setLastError(Errors::None);
          }
        }
        return false;
      }

      /* Tile buildability check */
      for(int ix = left; ix < right; ++ix)
      {
        for(int iy = top; iy < bottom; ++iy)
        {
          // Check if tile is buildable and explored
          if ( !Broodwar->isBuildable(ix, iy) || ( checkExplored && !Broodwar->isExplored(ix, iy)) )
            return false; // @TODO: Error code for !isExplored ??

          // Check if builder is capable of reaching the building site
          if ( builder && !builder->hasPath( (Position)TilePosition(ix, iy) ) )
            return false;
        }
      }

      /* Ground unit dimension check */
      int targetX = left * 32 + type.tileWidth()  * 32 / 2;
      int targetY = top  * 32 + type.tileHeight() * 32 / 2;
      for(int ix = left; ix < right; ++ix)
      {
        for(int iy = top; iy < bottom; ++iy)
        {
          foreach(Unit *u, Broodwar->unitsOnTile(ix,iy))
          {
            if ( u != builder &&
                 u->getPosition().x() + u->getType().dimensionRight() >= targetX - type.dimensionLeft()  &&
                 u->getPosition().y() + u->getType().dimensionDown()  >= targetY - type.dimensionUp()    &&
                 u->getPosition().x() - u->getType().dimensionLeft()  <= targetX + type.dimensionRight() &&
                 u->getPosition().y() - u->getType().dimensionUp()    <= targetY + type.dimensionDown() )
            {
              if ( !type.isAddon() )
                return false;
              else if ( !u->getType().canMove() )
                return false;
            }
          }
        }
      }

      /* Creep Check */
      if (type.getRace() == BWAPI::Races::Zerg )
      {
        //Most Zerg buildings can only be built on creep
        if ( !type.isResourceDepot() )
          for(int ix = left; ix < right; ++ix)
            for(int iy = top; iy < bottom; ++iy)
              if (!Broodwar->hasCreep(ix, iy))
                return false;
      }
      else
      {
        //Non-zerg buildings cannot be built on creep
        for(int ix = left; ix < right; ++ix)
          for(int iy = top; iy < bottom; ++iy)
            if (Broodwar->hasCreep(ix, iy))
              return false;
      }

      /* Power Check */
      if ( type.requiresPsi() && !Broodwar->hasPower(left, top, width, height) )
        return false;

      /* Resource Check (CC, Nex, Hatch) */
      if (type.isResourceDepot())
      {
        foreach (BWAPI::Unit* m, Broodwar->getStaticMinerals())
        {
          if (Broodwar->isVisible(m->getInitialTilePosition()) ||
              Broodwar->isVisible(m->getInitialTilePosition().x() + 1, m->getInitialTilePosition().y()))
            if (!m->isVisible())
              continue; // tile position is visible, but mineral is not => mineral does not exist
          if (m->getInitialTilePosition().x() > left - 5 &&
              m->getInitialTilePosition().y() > top  - 4 &&
              m->getInitialTilePosition().x() < left + 7 &&
              m->getInitialTilePosition().y() < top  + 6)
            return false;
        }
        foreach (BWAPI::Unit* g, Broodwar->getStaticGeysers())
        {
          if (g->getInitialTilePosition().x() > left - 7 &&
              g->getInitialTilePosition().y() > top  - 5 &&
              g->getInitialTilePosition().x() < left + 7 &&
              g->getInitialTilePosition().y() < top  + 6)
            return false;
        }
      }
      //if the build site passes all these tests, return true.
      return Broodwar->setLastError(Errors::None);
    }
    //------------------------------------------- CAN MAKE ---------------------------------------------------
    template <class GameImpl, class PlayerImpl, class UnitImpl>
    bool _canMake(Unit* builder, UnitType type)
    {
      /* Error checking */
      Broodwar->setLastError(Errors::None);
      if ( !Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      BWAPI::UnitType requiredType = type.whatBuilds().first;

      if ( builder )
      {
        /* Check if the owner of the unit is you */
        if (builder->getPlayer() != Broodwar->self())
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        BWAPI::UnitType builderType = builder->getType();
        if ( type == UnitTypes::Zerg_Nydus_Canal && builderType == UnitTypes::Zerg_Nydus_Canal )
        {
          if ( !builder->isCompleted() )
            return Broodwar->setLastError(Errors::Unit_Busy);

          if ( builder->getNydusExit() )
            return Broodwar->setLastError(Errors::Unknown);

          return true;
        }

        /* Check if this unit can actually build the unit type */
        if ( requiredType == UnitTypes::Zerg_Larva && builderType.producesLarva() )
        {
          if ( builder->getLarva().size() == 0 )
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        }
        else if ( builderType != requiredType )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        /* Carrier space */
        if ( builderType == UnitTypes::Protoss_Carrier )
        {
          int max_amt = 4;
          if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Carrier_Capacity) > 0)
            max_amt += 4;
          if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= max_amt)
            return Broodwar->setLastError(Errors::Insufficient_Space);
        }

        /* Reaver Space */
        if ( builderType == UnitTypes::Protoss_Reaver )
        {
          int max_amt = 5;
          if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0)
            max_amt += 5;
          if (builder->getScarabCount() + (int)builder->getTrainingQueue().size() >= max_amt)
            return Broodwar->setLastError(Errors::Insufficient_Space);
        }
      } // builder

      /* Check if player has enough minerals */
      if ( Broodwar->self()->minerals() < type.mineralPrice() )
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      /* Check if player has enough gas */
      if ( Broodwar->self()->gas() < type.gasPrice() )
        return Broodwar->setLastError(Errors::Insufficient_Gas);
      
      /* Check if player has enough supplies */
      BWAPI::Race typeRace = type.getRace();
      if ( type.supplyRequired() > 0 && Broodwar->self()->supplyTotal(typeRace) < Broodwar->self()->supplyUsed(typeRace) + type.supplyRequired() - (requiredType.getRace() == typeRace ? requiredType.supplyRequired() : 0) )
        return Broodwar->setLastError(Errors::Insufficient_Supply);

      UnitType addon = UnitTypes::None;
      std::map<UnitType, int>::const_iterator requiredEnd = type.requiredUnits().end();
      for(std::map<UnitType, int>::const_iterator i = type.requiredUnits().begin(); i != requiredEnd; ++i)
      {
        if (i->first.isAddon())
          addon = i->first;

        bool pass = false;
        if (Broodwar->self()->completedUnitCount(i->first) >= i->second)
          pass = true;
        if ( i->first == UnitTypes::Zerg_Hatchery &&
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Hatchery) +
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Lair)     +
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Hive)     >= i->second )
          pass = true;
        if ( i->first == UnitTypes::Zerg_Lair && 
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Lair) + 
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
          pass = true;
        if ( i->first == UnitTypes::Zerg_Spire && 
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Spire) +
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Greater_Spire) >= i->second )
          pass = true;
        
        if ( !pass )
          return Broodwar->setLastError(Errors::Insufficient_Tech);
      }

      if (type.requiredTech() != TechTypes::None && !Broodwar->self()->hasResearched(type.requiredTech()))
        return Broodwar->setLastError(Errors::Insufficient_Tech);

      if ( builder && 
           addon != UnitTypes::None &&
           addon.whatBuilds().first == type.whatBuilds().first &&
           (!builder->getAddon() || builder->getAddon()->getType() != addon) )
        return Broodwar->setLastError(Errors::Insufficient_Tech);
      return true;
    }
    //------------------------------------------- CAN RESEARCH -----------------------------------------------
    template <class GameImpl, class PlayerImpl, class UnitImpl>
    bool _canResearch(Unit* unit, TechType type)
    {
      /* Error checking */
      Broodwar->setLastError(Errors::None);
      if ( !Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if ( unit )
      {
        if (unit->getPlayer() != Broodwar->self())
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        if (unit->getType() != type.whatResearches())
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( unit->isLifted() || !unit->isIdle() || !unit->isCompleted() )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }
      if (Broodwar->self()->isResearching(type))
        return Broodwar->setLastError(Errors::Currently_Researching);

      if (Broodwar->self()->hasResearched(type))
        return Broodwar->setLastError(Errors::Already_Researched);

      if (Broodwar->self()->minerals() < type.mineralPrice())
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      if (Broodwar->self()->gas() < type.gasPrice())
        return Broodwar->setLastError(Errors::Insufficient_Gas);

      return true;
    }
    //------------------------------------------- CAN UPGRADE ------------------------------------------------
    template <class GameImpl, class PlayerImpl, class UnitImpl>
    bool _canUpgrade(Unit* unit, UpgradeType type)
    {
      Broodwar->setLastError(Errors::None);
      if ( !Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if ( unit )
      {
        if (unit->getPlayer() != Broodwar->self())
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        if (unit->getType() != type.whatUpgrades())
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( unit->isLifted() || !unit->isIdle() || !unit->isCompleted() )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }
      if (Broodwar->self()->isUpgrading(type))
        return Broodwar->setLastError(Errors::Currently_Upgrading);

      if (Broodwar->self()->getUpgradeLevel(type) >= type.maxRepeats())
        return Broodwar->setLastError(Errors::Fully_Upgraded);

      if (Broodwar->self()->minerals() < type.mineralPriceBase()+type.mineralPriceFactor()*(Broodwar->self()->getUpgradeLevel(type)))
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      if (Broodwar->self()->gas() < type.gasPriceBase()+type.gasPriceFactor()*(Broodwar->self()->getUpgradeLevel(type)))
        return Broodwar->setLastError(Errors::Insufficient_Gas);

      return true;
    }
  }
}