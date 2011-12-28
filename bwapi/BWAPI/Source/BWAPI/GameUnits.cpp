#include "GameImpl.h"
#include <Util/Foreach.h>

#include <BW/Unit.h>
#include <BW/UnitID.h>

#include <BWAPI/UnitImpl.h>
#include <BWAPI/PlayerImpl.h>

#include "../../Debug.h"

namespace BWAPI
{
  //------------------------------------------------ GET UNIT FROM INDEX -------------------------------------
  UnitImpl* GameImpl::getUnitFromIndex(int index)
  {
    int i = (index & 0x7FF);
    return this->unitArray[i];
  }
  //------------------------------------------------ IS UNIT ALIVE
  bool inline isUnitAlive(UnitImpl* i, bool isHidden = false)
  {
    //this function determines if a unit in one of the alive unit lists is actually "alive" according to BWAPI
    //this function is only used in computeUnitExistence and shouldn't be called from any other function

    if ( !i ) return false;

    BW::Unit *u = i->getOriginalRawData;
    if ( !u ) return false;

    UnitType _getType = BWAPI::UnitType(u->unitType);

    // Replica of official Unit::IsDead function
    if ( !u->sprite || (u->orderID == BW::OrderID::Die && u->orderState == 1) )
    {
      //Broodwar->printf("%s has met a true death", _getType.getName().c_str());
      return false;
    }
    // The rest is garbage?

    if ( u->orderID == BW::OrderID::Die && u->orderState != 1)
    { // Starcraft will keep a unit alive when order state is not 1
      // if for some reason the "die" order was interrupted, the unit will remain alive with 0 hp
      //Broodwar->printf("Bad logic, %s would not die with order state %u", _getType.getName().c_str(), u->orderState);
      return false;
    }

    if ( isHidden ) //usually means: is inside another unit?
    {
      if (_getType == UnitTypes::Unknown)
      { // Unknown units are not actually dead, idk why we assume so
        //Broodwar->printf("Bad logic?, unknown unit type death");
        return false;//skip subunits if they are in this list
      }
      else if (_getType == UnitTypes::Protoss_Scarab )
      { // some reason we decided all protoss scarabs are dead
        //Broodwar->printf("Bad logic?, %s death", _getType.getName().c_str());
        return false;
      }
    }
    // unit is alive otherwise
    return true;
  }
  //------------------------------------------ Compute Unit Existence ----------------------------------------
  void GameImpl::computeUnitExistence()
  {
    foreach(UnitImpl* u, aliveUnits) //all alive units are dying until proven alive
    {
      u->wasAlive = true;
      u->isAlive  = false;
    }
    lastEvadedUnits = evadeUnits;//save last evaded units for updating shared memory (Server.cpp)

    //set the wasAccessible and wasVisible fields
    foreach(UnitImpl* u, accessibleUnits)
      u->wasAccessible = true;
    foreach(UnitImpl* u, evadeUnits)
      u->wasAccessible = false;

    //fill dyingUnits set with all aliveUnits and then clear the aliveUnits set.
    dyingUnits = aliveUnits;
    aliveUnits.clear();
    //Now we will add alive units to the aliveUnits set and remove them from the dyingUnits set based on the Broodwar unit lists:

    //compute alive and dying units
    for ( UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_VisibleUnit_First); u; u = u->getNext() )
    {
      if ( isUnitAlive(u) )
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        dyingUnits.erase(u);
        u->updateInternalData();
      }
    }
    for ( UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_HiddenUnit_First); u; u = u->getNext() )
    {
      if ( isUnitAlive(u, true) )
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        dyingUnits.erase(u);
        u->updateInternalData();
      }
    }
    for ( UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_ScannerSweep_First); u; u = u->getNext() )
    {
      if ( isUnitAlive(u) )
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        dyingUnits.erase(u);
        u->updateInternalData();
      }
    }
    //set the exists field to false for all dying units (though we don't update/clear their data yet)
    foreach(UnitImpl* u, dyingUnits)
      u->self->exists = false;
  }
  //------------------------------------------ Compute Client Sets -------------------------------------------
  void GameImpl::computePrimaryUnitSets()
  {
    //this frame computes the set of accessible units and visible units.
    accessibleUnits.clear();

    //discoverUnits is the set of units that have entered the accessibleUnits set this frame
    discoverUnits.clear();

    //evadeUnits is the set of units that have left the accessibleUnits set this frame
    evadeUnits.clear();

    //computes sets, also generating UnitCreate, UnitDiscover, UnitShow, UnitDestroy, UnitEvade, and UnitHide callbacks
    foreach(UnitImpl* u, aliveUnits)
    {
      if (u->canAccess())
      {
        if ( !u->wasAlive )
          events.push_back(Event::UnitCreate(u));
        if ( !u->wasCompleted && u->isCompleted() )
        {
          events.push_back(Event::UnitComplete(u));
          u->wasCompleted = true;
        }
        if ( !u->wasAccessible )
        {
          discoverUnits.push_back(u);
          events.push_back(Event::UnitDiscover(u));
        }
        if ( u->isVisible() )
        {
          if ( !u->wasVisible )
            events.push_back(Event::UnitShow(u));
          u->wasVisible = true;
        }
        if (!u->isVisible())
        {
          if ( u->wasVisible )
            events.push_back(Event::UnitHide(u));
          u->wasVisible = false;
        }
        accessibleUnits.insert(u);
      }
      else
      {
        if (u->wasAccessible)
        {
          if (u->wasVisible)
          {
            u->wasVisible = false;
            events.push_back(Event::UnitHide(u));
          }
          evadeUnits.push_back(u);
          events.push_back(Event::UnitEvade(u));
        }
      }
    }
    foreach(UnitImpl* u, dyingUnits)
    {
      if (u->wasAccessible)
      {
        if (u->wasVisible)
        {
          u->wasVisible = false;
          events.push_back(Event::UnitHide(u));
        }
        evadeUnits.push_back(u);
        events.push_back(Event::UnitEvade(u));
        events.push_back(Event::UnitDestroy(u));
      }
    }
  }
  void GameImpl::extractUnitData()
  {
    //this function extracts all current unit information from Broodwar memory for all the accessible units
    //and also generates the NukeDetect event when needed
    nukeDots.clear();
    foreach (UnitImpl *i, aliveUnits)
    {
      i->connectedUnits.clear();
      i->loadedUnits.clear();
      int airWeaponCooldown = i->getOriginalRawData->airWeaponCooldown;
      if ( i->getOriginalRawData->subUnit )
        airWeaponCooldown = i->getOriginalRawData->subUnit->airWeaponCooldown;
      int groundWeaponCooldown = i->getOriginalRawData->groundWeaponCooldown;
      if ( i->getOriginalRawData->subUnit )
        groundWeaponCooldown = i->getOriginalRawData->subUnit->groundWeaponCooldown;
      if ( i->getOriginalRawData->unitType == BW::UnitID::Protoss_Reaver || i->getOriginalRawData->unitType == BW::UnitID::Protoss_Hero_Warbringer )
        groundWeaponCooldown = i->getOriginalRawData->mainOrderTimer;

      i->startingAttack           = airWeaponCooldown > i->lastAirWeaponCooldown || groundWeaponCooldown > i->lastGroundWeaponCooldown;
      i->lastAirWeaponCooldown    = airWeaponCooldown;
      i->lastGroundWeaponCooldown = groundWeaponCooldown;
      if (i->canAccess())
      {
        if (i->getID() == -1)
          i->setID(server.getUnitID(i));
        i->updateData();
      }
      if ( i->getOriginalRawData->unitType == BW::UnitID::Terran_Ghost)
      {
        if (i->getOriginalRawData->orderID == BW::OrderID::NukePaint)
          i->nukePosition = Position(i->getOriginalRawData->orderTargetPos.x, i->getOriginalRawData->orderTargetPos.y);
        if (i->getOriginalRawData->orderID != BW::OrderID::NukeTrack)
          i->nukeDetected = false;
        else
        {
          Position target=i->nukePosition;
          if (isFlagEnabled(Flag::CompleteMapInformation) || isVisible(target.x()/32,target.y()/32))
            nukeDots.insert(target);
          if ( !i->nukeDetected )
          {
            i->nukeDetected = true;
            if (isFlagEnabled(Flag::CompleteMapInformation) || isVisible(target.x()/32,target.y()/32))
              events.push_back(Event::NukeDetect(target));
            else
              events.push_back(Event::NukeDetect(Positions::Unknown));
          }
        }
      }
    }
  }
  void GameImpl::augmentUnitData()
  {
    //this function modifies the extracted unit data for build unit, loaded units, larva, and interceptors
    foreach(UnitImpl* i, accessibleUnits)
    {
      UnitImpl* orderTargetUnit = UnitImpl::BWUnitToBWAPIUnit(i->getOriginalRawData->orderTargetUnit);
      if ( orderTargetUnit && orderTargetUnit->exists() && i->getOrder() == Orders::ConstructingBuilding )
      {
        UnitImpl* j             = orderTargetUnit;
        i->self->buildUnit      = server.getUnitID((Unit*)j);
        i->self->isConstructing = true;
        i->self->isIdle         = false;
        i->self->buildType      = j->self->type;
        j->self->buildUnit      = server.getUnitID((Unit*)i);
        j->self->isConstructing = true;
        j->self->isIdle         = false;
        j->self->buildType      = j->self->type;
      }
      else if ( i->getAddon() && !i->getAddon()->isCompleted() )
      {
        UnitImpl* j             = (UnitImpl*)i->getAddon();
        i->self->buildUnit      = server.getUnitID((Unit*)j);
        i->self->isConstructing = true;
        i->self->isIdle         = false;
        i->self->buildType      = j->self->type;
        j->self->buildUnit      = server.getUnitID((Unit*)i);
        j->self->isConstructing = true;
        j->self->isIdle         = false;
        j->self->buildType      = j->self->type;
      }
      if ( i->getTransport() )
        ((UnitImpl*)i->getTransport())->loadedUnits.insert(i);

      if ( i->getHatchery() )
      {
        UnitImpl* hatchery = (UnitImpl*)i->getHatchery();
        hatchery->connectedUnits.insert((Unit*)i);
        if (hatchery->connectedUnits.size() >= 3)
          hatchery->self->remainingTrainTime = 0;
      }
      if ( i->getCarrier() )
        ((UnitImpl*)i->getCarrier())->connectedUnits.insert(i);

    }
  }
  void GameImpl::computeSecondaryUnitSets()
  {
    // This function computes units on tile, player units, neutral units, minerals, geysers, pylons, and static unit sets
    // Also generates the UnitMorph and UnitRenegade callbacks

    foreach(PlayerImpl *p, players)
    {
      PlayerData *pd = p->self;
      MemZero(pd->allUnitCount);
      MemZero(pd->visibleUnitCount);
      MemZero(pd->completedUnitCount);
    }

    foreach(UnitImpl* u, discoverUnits)
    {
      PlayerImpl *unitPlayer = ((PlayerImpl*)u->getPlayer());
      if ( !unitPlayer )
        continue;

      unitPlayer->units.insert(u);
      if ( u->getType().getRace() != Races::Unknown )
        unitPlayer->wasSeenByBWAPIPlayer = true;

      if ( unitPlayer->isNeutral() )
      {
        neutralUnits.insert(u);
        if ( u->getType().isMineralField() )
          minerals.insert(u);
        else if ( u->getType() == UnitTypes::Resource_Vespene_Geyser )
          geysers.insert(u);
      }
      else
      {
        if ( unitPlayer == Broodwar->self() && u->getType() == UnitTypes::Protoss_Pylon )
          pylons.insert(u);
      }
    }
    foreach(UnitImpl* u, evadeUnits)
    {
      PlayerImpl *unitPlayer = ((PlayerImpl*)u->getPlayer());
      if ( !unitPlayer )
        continue;

      unitPlayer->units.erase(u);
      if ( unitPlayer->isNeutral() )
      {
        neutralUnits.erase(u);
        if ( u->getType().isMineralField() )
          minerals.erase(u);
        else if ( u->getType() == UnitTypes::Resource_Vespene_Geyser )
          geysers.erase(u);
      }
      else if ( unitPlayer == Broodwar->self() && u->getType() == UnitTypes::Protoss_Pylon )
      {        
        pylons.erase(u);
      }
    }

    // Clear the units on tile data
    SIZE mapSize = { Map::getWidth(), Map::getHeight() };
    for ( unsigned int x = 0; x < (unsigned int)mapSize.cx; ++x )
      for ( unsigned int y = 0; y < (unsigned int)mapSize.cy; ++y )
        unitsOnTileData[x][y].clear();

    foreach(UnitImpl* i, accessibleUnits)
    {
      if ( i->getType().isBuilding() && !i->isLifted() )
      {
        // Retrieve the buildings on tile
        int tx = i->getTilePosition().x();
        int ty = i->getTilePosition().y();
        SIZE typeTileSize = { i->getType().tileWidth(), i->getType().tileHeight() };
        for(int x = tx; x < tx + typeTileSize.cx && x < mapSize.cx; ++x)
          for(int y = ty; y < ty + typeTileSize.cy && y < mapSize.cy; ++y)
            unitsOnTileData[x][y].insert(i);
      }
      else
      {
        // @TODO: Assign using getUnitsInRectangle
        // Retrieve the units on tile
        int startX = i->getLeft() / TILE_SIZE;
        int endX   = (i->getRight() + TILE_SIZE - 1) / TILE_SIZE; // Division - round up
        int startY = i->getTop() / TILE_SIZE;
        int endY   = (i->getBottom() + TILE_SIZE - 1) / TILE_SIZE;
        for (int x = startX; x < endX && x < mapSize.cx; ++x)
          for (int y = startY; y < endY && y < mapSize.cy; ++y)
            unitsOnTileData[x][y].insert(i);
      }
      if (i->lastType != i->_getType && i->lastType != UnitTypes::Unknown && i->_getType != UnitTypes::Unknown)
      {
        events.push_back(Event::UnitMorph(i));
        if (i->lastType == UnitTypes::Resource_Vespene_Geyser)
        {
          neutralUnits.erase(i);
          geysers.erase(i);
        }
        if (i->_getType == UnitTypes::Resource_Vespene_Geyser)
        {
          neutralUnits.insert(i);
          geysers.insert(i);
        }
      }
      if (i->lastPlayer != i->_getPlayer && i->lastPlayer && i->_getPlayer )
      {
        events.push_back(Event::UnitRenegade(i));
        ((PlayerImpl*)i->lastPlayer)->units.erase(i);
        ((PlayerImpl*)i->_getPlayer)->units.insert(i);
      }
      int allUnits  = UnitTypes::AllUnits;
      int men       = UnitTypes::Men;
      int buildings = UnitTypes::Buildings;
      int factories = UnitTypes::Factories;
      int thisUnit  = i->_getType;
      
      // Increment specific unit count
      BWAPI::PlayerData *pSelf = ((PlayerImpl*)i->_getPlayer)->self;
      pSelf->allUnitCount[thisUnit]++;
      if (i->isVisible())
        pSelf->visibleUnitCount[thisUnit]++;
      if (i->isCompleted())
        pSelf->completedUnitCount[thisUnit]++;
      // increment all unit count
      pSelf->allUnitCount[allUnits]++;
      if (i->isVisible())
        pSelf->visibleUnitCount[allUnits]++;
      if (i->isCompleted())
        pSelf->completedUnitCount[allUnits]++;
      if ( i->_getType.isBuilding() )
      { // increment buildings unit count
        pSelf->allUnitCount[buildings]++;
        if (i->isVisible())
          pSelf->visibleUnitCount[buildings]++;
        if (i->isCompleted())
          pSelf->completedUnitCount[buildings]++;
        if ( (i->_getType.canProduce() || i->_getType.producesLarva()) ) // increment factories unit count
        {
          pSelf->allUnitCount[factories]++;
        if (i->isVisible())
          pSelf->visibleUnitCount[factories]++;
        if (i->isCompleted())
          pSelf->completedUnitCount[factories]++;
        }
      }
      else
      { // increment men unit count
        pSelf->allUnitCount[men]++;
        if ( i->isVisible() )
          pSelf->visibleUnitCount[men]++;
        if ( i->isCompleted() )
          pSelf->completedUnitCount[men]++;
      }
      i->lastPlayer = i->_getPlayer;
      i->lastType   = i->_getType;
    }

    if (this->staticNeutralUnits.empty()) //if we haven't saved the set of static units, save them now
    {
      foreach (UnitImpl* i, accessibleUnits)
      {
        if (i->_getPlayer->isNeutral())
        {
          i->saveInitialState();
          this->staticNeutralUnits.insert(i);
          if ( i->_getType.isMineralField() )
            this->staticMinerals.insert(i);
          else if (i->_getType == UnitTypes::Resource_Vespene_Geyser)
            this->staticGeysers.insert(i);
        }
      }
    }
  }
  //---------------------------------------------- UPDATE UNITS ----------------------------------------------
  void GameImpl::updateUnits()
  {
    static std::set<Unit*> selectedU;

    // Update all unit data
    computeUnitExistence();
    computePrimaryUnitSets();
    extractUnitData();
    augmentUnitData();
    applyLatencyCompensation();
    computeSecondaryUnitSets();

    // Update selection data
    selectedU = selectedUnitSet;
    selectedUnitSet.clear();
    foreach ( UnitImpl* u, selectedU )
    {
      if ( u )
      {
        if ( u->exists() )
          selectedUnitSet.insert(u);
        else
        {
          u->setSelected(false);
          if ( u->self )
            u->self->isSelected = false;
        } // if exists
      } // if u
    } // for each in selectedU

    // Get all units under disruption web and dark swarm
    foreach ( UnitImpl *_u, this->neutralUnits )
    {
      BWAPI::UnitType ut = _u->getType();
      if ( ut != UnitTypes::Spell_Dark_Swarm &&
           ut != UnitTypes::Spell_Disruption_Web )
        continue;

      int r = _u->getRight()  - (ut == UnitTypes::Spell_Disruption_Web ? 1 : 0);
      int b = _u->getBottom() - (ut == UnitTypes::Spell_Disruption_Web ? 1 : 0);
      foreach ( UnitImpl *uInside, this->getUnitsInRectangle(_u->getLeft(), _u->getTop(), r, b) )
      {
        if ( uInside->getType().isSpell() || uInside->getType().isFlyer() )
          continue;
        if ( ut == UnitTypes::Spell_Dark_Swarm )
          uInside->self->isUnderDarkSwarm = true;
        else if ( ut == UnitTypes::Spell_Disruption_Web )
          uInside->self->isUnderDWeb      = true;
      }
    } // for each neutral units
  } // updateUnits

  //------------------------------------------- CENTER ON SELECTED -------------------------------------------
  void GameImpl::moveToSelectedUnits()
  {
    int count = this->selectedUnitSet.size();
    int x = 0;
    int y = 0;
    foreach(BWAPI::UnitImpl *u, this->selectedUnitSet)
    {
      x += u->getPosition().x();
      y += u->getPosition().y();
    }
    x /= count;
    y /= count;
    x -= BW::BWDATA_GameScreenBuffer->wid / 2;
    y -= BW::BWDATA_GameScreenBuffer->ht  / 2 - 40;
    if ( x < 0 )
      x = 0;
    if ( y < 0 )
      y = 0;
    this->setScreenPosition(x, y);
  }

}


