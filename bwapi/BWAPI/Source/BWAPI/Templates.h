#pragma once
#include <BWAPI.h>
#include <Util/Foreach.h>
namespace BWAPI
{
  namespace Templates
  {
    //------------------------------------------- CAN BUILD HERE ---------------------------------------------
    template <class GameImpl, class PlayerImpl, class UnitImpl>
    bool canBuildHere(Unit* builder, TilePosition position, UnitType type, bool checkExplored)
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
    bool canMake(Unit* builder, UnitType type)
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
        else if ( builderType == UnitTypes::Hero_Gantrithor )
        {
          if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= 8)
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
        else if ( builderType == UnitTypes::Hero_Warbringer )
        {
          if (builder->getScarabCount() + (int)builder->getTrainingQueue().size() >= 10)
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
    bool canResearch(Unit* unit, TechType type)
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
    bool canUpgrade(Unit* unit, UpgradeType type)
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

    //------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
    template <class GameImpl, class PlayerImpl, class UnitImpl>
    bool canIssueCommand(Unit* thisUnit, UnitCommand c)
    {
      // Basic header
      Broodwar->setLastError(Errors::None);
      if (!thisUnit->exists())
        return Broodwar->setLastError(Errors::Unit_Not_Visible);

      if (thisUnit->getPlayer() != Broodwar->self())
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      BWAPI::UnitCommandType ct = c.type;

      // Global can be ordered check
      if ( thisUnit->isLockedDown() || 
           thisUnit->isMaelstrommed() || 
           thisUnit->isStasised()  || 
           thisUnit->isUnpowered() ||
           thisUnit->getOrder() == Orders::ZergBirth )
        return Broodwar->setLastError(Errors::Unit_Busy);

      // Hallucination check
      if ( thisUnit->isHallucination()                      &&
           UnitCommandTypes::Attack_Move          != ct &&
           UnitCommandTypes::Attack_Unit          != ct &&
           UnitCommandTypes::Move                 != ct &&
           UnitCommandTypes::Patrol               != ct &&
           UnitCommandTypes::Hold_Position        != ct &&
           UnitCommandTypes::Stop                 != ct &&
           UnitCommandTypes::Follow               != ct &&
           UnitCommandTypes::Load                 != ct &&
           UnitCommandTypes::Right_Click_Position != ct &&
           UnitCommandTypes::Right_Click_Unit     != ct )
         return Broodwar->setLastError(Errors::Incompatible_UnitType);

      // Can be ordered check
      if ( !thisUnit->getType().isBuilding() &&
           !thisUnit->isInterruptible() &&
           (UnitCommandTypes::Attack_Unit           == ct ||
            UnitCommandTypes::Attack_Move           == ct ||
            UnitCommandTypes::Build                 == ct ||
            UnitCommandTypes::Follow                == ct ||
            UnitCommandTypes::Gather                == ct ||
            UnitCommandTypes::Load                  == ct ||
            UnitCommandTypes::Move                  == ct ||
            UnitCommandTypes::Patrol                == ct ||
            UnitCommandTypes::Repair                == ct ||
            UnitCommandTypes::Return_Cargo          == ct ||
            UnitCommandTypes::Right_Click_Position  == ct ||
            UnitCommandTypes::Right_Click_Unit      == ct ||
            UnitCommandTypes::Unload                == ct ||
            UnitCommandTypes::Unload_All            == ct ||
            UnitCommandTypes::Unload_All_Position   == ct ||
            UnitCommandTypes::Use_Tech              == ct ||
            UnitCommandTypes::Use_Tech_Unit         == ct ||
            UnitCommandTypes::Use_Tech_Position     == ct ) )
        return Broodwar->setLastError(Errors::Unit_Busy);


      // valid target check
      if ((!c.target ||
          !c.target->exists()) &&
          (UnitCommandTypes::Attack_Unit      == ct ||
           UnitCommandTypes::Set_Rally_Unit   == ct ||
           UnitCommandTypes::Follow           == ct ||
           UnitCommandTypes::Gather           == ct ||
           UnitCommandTypes::Repair           == ct ||
           UnitCommandTypes::Load             == ct ||
           UnitCommandTypes::Unload           == ct ||
           UnitCommandTypes::Right_Click_Unit == ct ||
           UnitCommandTypes::Use_Tech_Unit    == ct) )
        return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

      // Build/Train requirements
      if ( UnitCommandTypes::Build       == ct ||
           UnitCommandTypes::Build_Addon == ct ||
           UnitCommandTypes::Morph       == ct ||
           UnitCommandTypes::Train       == ct )
      {
        UnitType uType = UnitType(c.extra);
        if ( !Broodwar->canMake(thisUnit, uType) )
          return false;

        if ( thisUnit->isConstructing() || 
             !thisUnit->isCompleted()   || 
             (thisUnit->getType().isFlyingBuilding() && thisUnit->isLifted()) ||
             (UnitCommandTypes::Train != ct && thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
          return Broodwar->setLastError(Errors::Unit_Busy);

        if ( UnitCommandTypes::Build == ct )
        {
          if ( !uType.isBuilding() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);

          if ( !uType.isAddon() && !Broodwar->canBuildHere(thisUnit, BWAPI::TilePosition(c.x, c.y), uType, true) )
            return false;
        }
        else if ( UnitCommandTypes::Build_Addon == ct )
        {
          if ( !uType.isAddon() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);

          if ( thisUnit->getAddon() )
            return false;

          if ( !Broodwar->canBuildHere(thisUnit, BWAPI::TilePosition(thisUnit->getTilePosition().x() + 4, thisUnit->getTilePosition().y() + 1), uType) )
            return false;
        }
        else
        {
          if ( thisUnit->getType().producesLarva() && uType.whatBuilds().first == UnitTypes::Zerg_Larva && thisUnit->getLarva().size() == 0 )
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        }
      } // build/train

      // Research/Upgrade requirements
      if ( UnitCommandTypes::Research == ct && !Broodwar->canResearch(thisUnit,TechType(c.extra)) )
        return false;
      if ( UnitCommandTypes::Upgrade  == ct && !Broodwar->canUpgrade(thisUnit,UpgradeType(c.extra)) )
        return false;

      // Set Rally 
      if ( (UnitCommandTypes::Set_Rally_Position == ct || UnitCommandTypes::Set_Rally_Unit == ct) && !thisUnit->getType().canProduce() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      // Move/stop/standard
      if ( (UnitCommandTypes::Move          == ct || 
            UnitCommandTypes::Patrol        == ct ||
            UnitCommandTypes::Hold_Position == ct ||
            UnitCommandTypes::Stop          == ct ||
            UnitCommandTypes::Follow        == ct) &&
            thisUnit->getType().isBuilding() &&
            !thisUnit->isLifted() &&
            ct != UnitCommandTypes::Hold_Position &&
            ct != UnitCommandTypes::Stop )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

      // Gather
      if ( UnitCommandTypes::Gather == ct )
      {
        UnitType uType = c.target->getType();
        if ( !thisUnit->getType().isWorker() ||
             !uType.isResourceContainer() ||
             uType == UnitTypes::Resource_Vespene_Geyser )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( thisUnit->getPowerUp() || !c.target->isCompleted() )
          return Broodwar->setLastError(Errors::Unit_Busy);
      } // gather

      // return cargo
      if ( UnitCommandTypes::Return_Cargo == ct )
      {
        if ( !thisUnit->getType().isWorker() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( !thisUnit->isCarryingGas() && !thisUnit->isCarryingMinerals() )
          return Broodwar->setLastError(Errors::Insufficient_Ammo);
      } // return

      // Repair
      if ( UnitCommandTypes::Repair == ct )
      {
        UnitType targType = c.target->getType();
        if ( thisUnit->getType() != BWAPI::UnitTypes::Terran_SCV ||
             targType.getRace() != BWAPI::Races::Terran ||
             !targType.isMechanical() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
      } // repair

      // Ability requirements
      if ( UnitCommandTypes::Use_Tech          == ct ||
           UnitCommandTypes::Use_Tech_Position == ct ||
           UnitCommandTypes::Use_Tech_Unit     == ct ||
           UnitCommandTypes::Burrow            == ct ||
           UnitCommandTypes::Cloak             == ct ||
           UnitCommandTypes::Siege             == ct )
      {
        // Retrieve the tech type
        BWAPI::TechType tech = TechType(c.extra);
        if ( UnitCommandTypes::Burrow == ct )
          tech = BWAPI::TechTypes::Burrowing;
        else if ( UnitCommandTypes::Cloak == ct )
          tech = thisUnit->getType().cloakingTech();
        else if ( UnitCommandTypes::Siege == ct )
          tech = BWAPI::TechTypes::Tank_Siege_Mode;

        // do checks
        if ( !Broodwar->self()->hasResearched(tech) && thisUnit->getType() != UnitTypes::Zerg_Lurker )
          return Broodwar->setLastError(Errors::Insufficient_Tech);

        if ( thisUnit->getEnergy() < tech.energyUsed() )
          return Broodwar->setLastError(Errors::Insufficient_Energy);

        if ( tech != TechTypes::Burrowing && tech.whatUses().find(thisUnit->getType()) == tech.whatUses().end() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( tech == TechTypes::Spider_Mines && thisUnit->getSpiderMineCount() <= 0 )
          return Broodwar->setLastError(Errors::Insufficient_Ammo);

        //if ( tech == TechTypes::Nuclear_Strike && thisUnit->getPlayer()->completedUnitCount(UnitTypes::Terran_Nuclear_Missile) > 0 )
        //  return Broodwar->setLastError(Errors::Insufficient_Ammo);

        if (tech == TechTypes::Burrowing)
        {
          if ( !thisUnit->getType().isBurrowable() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);

          if ( thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Burrowing || thisUnit->getOrder() == Orders::Unburrowing )
            return false;
        }
        else if (tech == TechTypes::Tank_Siege_Mode)
        {
          if ( thisUnit->isSieged() )
            return false;

          if ( thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging )
            return Broodwar->setLastError(Errors::Unit_Busy);
        }
        else if (tech == TechTypes::Personnel_Cloaking || tech == TechTypes::Cloaking_Field)
        {
          if ( thisUnit->getSecondaryOrder() == Orders::Cloak )
            return false;
        }

        if ( tech == TechTypes::None || tech == TechTypes::Unknown )
          return Broodwar->setLastError(Errors::Incompatible_TechType);
      } // ability

      // Unburrow
      if ( UnitCommandTypes::Unburrow == ct )
      {
        if ( !thisUnit->getType().isBurrowable() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        if ( !thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Unburrowing )
          return false;
      } // unburrow

      // Decloak
      if ( UnitCommandTypes::Decloak == ct )
      {
        if ( thisUnit->getType().cloakingTech() == TechTypes::None )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( thisUnit->getSecondaryOrder() != Orders::Cloak )
          return false;
      } // decloak

      // Unsiege
      if ( UnitCommandTypes::Unsiege == ct )
      {
        if ( !thisUnit->isSieged() )
          return false;

        if ( thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }

      // lift/land
      if ( UnitCommandTypes::Lift == ct || UnitCommandTypes::Land == ct )
      {
        if ( !thisUnit->getType().isFlyingBuilding() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( UnitCommandTypes::Lift == ct ? thisUnit->isLifted() : !thisUnit->isLifted() )
          return false;
      } // lift/land

      // unload
      if ( UnitCommandTypes::Unload              == ct ||
           UnitCommandTypes::Unload_All          == ct ||
           UnitCommandTypes::Unload_All_Position == ct )
      {
        if ( thisUnit->getLoadedUnits().size() == 0 )
          return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

        if ( thisUnit->getType().spaceProvided() <= 0 )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        //if ( thisUnit->getType() == UnitTypes::Zerg_Overlord && thisUnit->getPlayer()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0 )
        //  return Broodwar->setLastError(Errors::Insufficient_Tech);

        if ( UnitCommandTypes::Unload == ct )
        {
          bool canUnload = false;
          std::set<Unit*> loadedUnits;
          if (loadedUnits.find(c.target)!=loadedUnits.end())
            canUnload = true;
          if ( !canUnload )
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        }
      } // unload

      // Halt construction
      if ( UnitCommandTypes::Halt_Construction == ct && thisUnit->getOrder() != Orders::ConstructingBuilding )
        return false;

      // Cancel construction
      if ( UnitCommandTypes::Cancel_Construction == ct )
      {
        if ( !thisUnit->getType().isBuilding() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( thisUnit->isCompleted() || (!thisUnit->isCompleted() && thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit()) )
          return false;
      }

      // cancel addon
      if ( UnitCommandTypes::Cancel_Addon == ct && (!thisUnit->getAddon() || thisUnit->getAddon()->isCompleted()) )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      // cancel train
      if ( UnitCommandTypes::Cancel_Train == ct && !thisUnit->isTraining() )
        return false;

      if ( UnitCommandTypes::Cancel_Train_Slot == ct && (!thisUnit->isTraining() || (thisUnit->getTrainingQueue().size() <= (unsigned int)c.extra && c.extra >= 0)) )
        return false;

      // cancel morph
      if ( UnitCommandTypes::Cancel_Morph == ct && (!thisUnit->isMorphing() || (!thisUnit->isCompleted() && thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit())) )
        return false;

      // cancel research
      if ( UnitCommandTypes::Cancel_Research == ct && thisUnit->getOrder() != Orders::ResearchTech )
        return false;

      // cancel upgrade
      if ( UnitCommandTypes::Cancel_Upgrade == ct && thisUnit->getOrder() != Orders::Upgrade )
        return false;

      return true;
    }
  }
}