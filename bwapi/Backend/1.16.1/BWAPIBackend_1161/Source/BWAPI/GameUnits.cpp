#include "GameImpl.h"
#include "../Convenience.h"

#include "../BW/CUnit.h"
#include "../BW/Dialog.h"

#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BWAPI/Race.h>
#include <BWAPI/Order.h>

#include <BWAPI4/Event.h>
#include <BWAPI/Flag.h>

namespace BWAPI4
{
  //------------------------------------------------ GET UNIT FROM INDEX -------------------------------------
  UnitImpl* GameImpl::getUnitFromIndex(int index)
  {
    index &= 0x7FF;
    if (static_cast<unsigned>(index) < unitArray.size())
      return this->unitArray[index];
    return nullptr;
  }
  //------------------------------------------------ IS UNIT ALIVE
  bool inline isUnitAlive(UnitImpl* i, bool isHidden = false)
  {
    //this function determines if a unit in one of the alive unit lists is actually "alive" according to BWAPI
    //this function is only used in computeUnitExistence and shouldn't be called from any other function

    if ( !i ) return false;

    BW::CUnit *u = i->getOriginalRawData;
    if ( !u ) return false;

    UnitType _getType = BWAPI::UnitType(u->unitType);

    // Replica of official UnitImpl::IsDead function
    if ( !u->sprite || (u->orderID == Orders::Die && u->orderState == 1) )
    {
      //BroodwarImpl.printf("%s has met a true death", _getType.getName().c_str());
      return false;
    }
    // The rest is garbage?

    if ( u->orderID == Orders::Die && u->orderState != 1)
    { // Starcraft will keep a unit alive when order state is not 1
      // if for some reason the "die" order was interrupted, the unit will remain alive with 0 hp
      //BroodwarImpl.printf("Bad logic, %s would not die with order state %u", _getType.getName().c_str(), u->orderState);
      return false;
    }

    if ( isHidden ) //usually means: is inside another unit?
    {
      if (_getType == UnitTypes::Unknown)
      { // Unknown units are not actually dead, idk why we assume so
        //BroodwarImpl.printf("Bad logic?, unknown unit type death");
        return false;//skip subunits if they are in this list
      }
      else if (_getType == UnitTypes::Protoss_Scarab )
      { // some reason we decided all protoss scarabs are dead
        //BroodwarImpl.printf("Bad logic?, %s death", _getType.getName().c_str());
        return false;
      }
    }
    // unit is alive otherwise
    return true;
  }
  //------------------------------------------ Compute Unit Existence ----------------------------------------
  void GameImpl::computeUnitExistence()
  {
    for(Unit ui : aliveUnits) //all alive units are dying until proven alive
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      u->wasAlive = true;
      u->isAlive  = false;
    }

    //set the wasAccessible and wasVisible fields
    for (Unit u : accessibleUnits)
    {
      static_cast<UnitImpl*>(u)->wasAccessible = true;
    }
    for (Unit u : evadeUnits)
    {
      static_cast<UnitImpl*>(u)->wasAccessible = false;
    }
    //fill dyingUnits set with all aliveUnits and then clear the aliveUnits set.
    dyingUnits = aliveUnits;
    aliveUnits.clear();

    // (We assume isAlive is false for all dyingUnits currently)
    // Now we will add alive units to the aliveUnits set and set their isAlive flag to true

    //compute alive units
    for ( UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA::UnitNodeList_VisibleUnit_First); u; u = u->getNext() )
    {
      if ( isUnitAlive(u) )
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        u->updateInternalData();
      }
    }
    for ( UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA::UnitNodeList_HiddenUnit_First); u; u = u->getNext() )
    {
      if ( isUnitAlive(u, true) )
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        u->updateInternalData();
      }
    }
    for ( UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA::UnitNodeList_ScannerSweep_First); u; u = u->getNext() )
    {
      if ( isUnitAlive(u) )
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        u->updateInternalData();
      }
    }
    
    // Since we know isAlive is false for the dying units, but true for the alive units,
    // then we can remove the alive ones from the dyingUnits set in linear fashion
    auto it = dyingUnits.begin();
    while ( it != dyingUnits.end() )
    {
      if ( static_cast<UnitImpl*>(*it)->isAlive )
      {
        // Remove from the set if it's not dead
        it = dyingUnits.erase(it);
      }
      else
      {
        // We can also set exists to false for units that will remain dying
        static_cast<UnitImpl*>(*it)->self->exists = false;
        it++;
      }
    }
    
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
    for(Unit ui : aliveUnits)
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      if (u->canAccess())
      {
        if ( !u->wasAlive )
          events.push_back(Event::UnitCreate(u));
        if ( !u->wasCompleted && u->_isCompleted )
        {
          events.push_back(Event::UnitComplete(u));
          u->wasCompleted = true;
        }
        if ( !u->wasAccessible )
        {
          discoverUnits.insert(u);
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
          evadeUnits.insert(u);
          events.push_back(Event::UnitEvade(u));
        }
      }
    }
    for(Unit ui : dyingUnits)
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      if (u->wasAccessible)
      {
        if (u->wasVisible)
        {
          u->wasVisible = false;
          events.push_back(Event::UnitHide(u));
        }
        evadeUnits.insert(u);
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
    for (Unit ui : aliveUnits)
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      u->connectedUnits.clear();
      u->loadedUnits.clear();

      if (u->getID() == -1)
        u->setID(server.getUnitID(u));

      u->updateData();

      if ( u->getOriginalRawData->unitType == UnitTypes::Terran_Ghost)
      {
        if (u->getOriginalRawData->orderID == Orders::NukePaint)
          u->nukePosition = BWAPI::Position(u->getOriginalRawData->orderTarget.pt);
        if (u->getOriginalRawData->orderID != Orders::NukeTrack)
          u->nukeDetected = false;
        else
        {
          Position target=u->nukePosition;
          if (isFlagEnabled(Flag::CompleteMapInformation) || isVisible(target.x / 32, target.y / 32))
          {
            nukeDots.push_back(target);
          }
          if ( !u->nukeDetected )
          {
            u->nukeDetected = true;
            if (isFlagEnabled(Flag::CompleteMapInformation) || isVisible(target.x/32,target.y/32))
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
    for(Unit ui : accessibleUnits)
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      UnitImpl* orderTargetUnit = UnitImpl::BWUnitToBWAPIUnit(u->getOriginalRawData->orderTarget.pUnit);
      UnitImpl* buildUnit       = UnitImpl::BWUnitToBWAPIUnit(u->getOriginalRawData->currentBuildUnit);
      if ( orderTargetUnit && orderTargetUnit->exists() && u->getOrder() == Orders::ConstructingBuilding )
      {
        UnitImpl* j             = orderTargetUnit;
        u->self->buildUnit      = server.getUnitID(j);
        u->self->isConstructing = true;
        u->self->isIdle         = false;
        u->self->buildType      = j->self->type;
        j->self->buildUnit      = server.getUnitID(u);
        j->self->isConstructing = true;
        j->self->isIdle         = false;
        j->self->buildType      = j->self->type;
      }
      else if ( u->getAddon() && !u->getAddon()->isCompleted() )
      {
        UnitImpl* j             = static_cast<UnitImpl*>(u->getAddon());
        u->self->buildUnit      = server.getUnitID(j);
        u->self->isConstructing = true;
        u->self->isIdle         = false;
        u->self->buildType      = j->self->type;
        j->self->buildUnit      = server.getUnitID(u);
        j->self->isConstructing = true;
        j->self->isIdle         = false;
        j->self->buildType      = j->self->type;
      }
      else if ( buildUnit && buildUnit->exists() && u->isTraining() )
      {
        // Apply buildUnit symmetry
        UnitImpl* j             = buildUnit;
        j->self->buildUnit      = server.getUnitID(u);
        j->self->isConstructing = true;
        j->self->isIdle         = false;
        j->self->buildType      = j->self->type;
      }
      if (u->getTransport())
      {
        static_cast<UnitImpl*>(u->getTransport())->loadedUnits.insert(u);
      }

      if ( u->getHatchery() )
      {
        UnitImpl* hatchery = static_cast<UnitImpl*>(u->getHatchery());
        hatchery->connectedUnits.insert(u);
        if (hatchery->connectedUnits.size() >= 3)
          hatchery->self->remainingTrainTime = 0;
      }
      if (u->getCarrier())
      {
        static_cast<UnitImpl*>(u->getCarrier())->connectedUnits.insert(u);
      }
    }
  }
  void GameImpl::computeSecondaryUnitSets()
  {
    // This function computes units on tile, player units, neutral units, minerals, geysers, pylons, and static unit sets
    // Also generates the UnitMorph and UnitRenegade callbacks

    for(PlayerImpl* p : players)
    {
      PlayerData *pd = p->self;
      MemZero(pd->allUnitCount);
      MemZero(pd->visibleUnitCount);
      MemZero(pd->completedUnitCount);
    }

    for(Unit u : discoverUnits)
    {
      PlayerImpl *unitPlayer = u->getPlayer();
      if ( !unitPlayer )
        continue;
    
      unitPlayer->units.insert(u);

      // Create a local copy of the unit type
      UnitType type = u->getType();

      if ( type.getRace() != Races::Unknown )
        unitPlayer->wasSeenByBWAPIPlayer = true;

      if ( unitPlayer->isNeutral() )
      {
        neutralUnits.insert(u);
        if ( type.isMineralField() )
          minerals.insert(u);
        else if ( type == UnitTypes::Resource_Vespene_Geyser )
          geysers.insert(u);
      }
      else
      {
        if ( unitPlayer == BroodwarImpl.self() && type == UnitTypes::Protoss_Pylon )
          pylons.insert(u);
      }
    }
    for(Unit u : evadeUnits)
    {
      PlayerImpl *unitPlayer = u->getPlayer();
      if ( !unitPlayer )
        continue;

      unitPlayer->units.erase(u);

      // Create a local copy of the unit type
      UnitType type = u->getType();

      if ( unitPlayer->isNeutral() )
      {
        neutralUnits.erase(u);
        if ( type.isMineralField() )
          minerals.erase(u);
        else if ( type == UnitTypes::Resource_Vespene_Geyser )
          geysers.erase(u);
      }
      else if ( unitPlayer->getNationId() == BroodwarImpl.self()->getNationId() && type == UnitTypes::Protoss_Pylon )
      {
        pylons.erase(u);
      }
    }

    for(Unit ui : accessibleUnits)
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      if (u->lastType != u->_getType && u->lastType != UnitTypes::Unknown && u->_getType != UnitTypes::Unknown)
      {
        u->wasCompleted = false; // After morphing units mostly are not completed
        events.push_back(Event::UnitMorph(u));
        if (u->lastType == UnitTypes::Resource_Vespene_Geyser)
        {
          neutralUnits.erase(u);
          geysers.erase(u);
        }
        else if (u->_getType == UnitTypes::Resource_Vespene_Geyser)
        {
          neutralUnits.insert(u);
          geysers.insert(u);
        }
      }
      if (u->lastPlayer != u->getPlayer() && u->lastPlayer && u->getPlayer() )
      {
        events.push_back(Event::UnitRenegade(u));
        u->lastPlayer->units.erase(u);
        u->getPlayer()->units.insert(u);
      }
      int allUnits  = UnitTypes::AllUnits;
      int men       = UnitTypes::Men;
      int buildings = UnitTypes::Buildings;
      int factories = UnitTypes::Factories;
      int thisUnit  = u->_getType;
      
      // Increment specific unit count
      BWAPI4::PlayerData *pSelf = u->getPlayer()->self;
      pSelf->allUnitCount[thisUnit]++;
      if (u->isVisible())
        pSelf->visibleUnitCount[thisUnit]++;
      if (u->isCompleted())
        pSelf->completedUnitCount[thisUnit]++;
      // increment all unit count
      pSelf->allUnitCount[allUnits]++;
      if (u->isVisible())
        pSelf->visibleUnitCount[allUnits]++;
      if (u->isCompleted())
        pSelf->completedUnitCount[allUnits]++;
      if ( u->_getType.isBuilding() )
      { // increment buildings unit count
        pSelf->allUnitCount[buildings]++;
        if (u->isVisible())
          pSelf->visibleUnitCount[buildings]++;
        if (u->isCompleted())
          pSelf->completedUnitCount[buildings]++;
        if ( (u->_getType.canProduce() || u->_getType.producesLarva()) ) // increment factories unit count
        {
          pSelf->allUnitCount[factories]++;
        if (u->isVisible())
          pSelf->visibleUnitCount[factories]++;
        if (u->isCompleted())
          pSelf->completedUnitCount[factories]++;
        }
      }
      else
      { // increment men unit count
        pSelf->allUnitCount[men]++;
        if ( u->isVisible() )
          pSelf->visibleUnitCount[men]++;
        if ( u->isCompleted() )
          pSelf->completedUnitCount[men]++;
      }
      u->lastPlayer = u->getPlayer();
      u->lastType   = u->_getType;
    }

    if (this->staticNeutralUnits.empty()) //if we haven't saved the set of static units, save them now
    {
      for (Unit ui : aliveUnits)
      {
        UnitImpl *u = static_cast<UnitImpl*>(ui);
        if (u->getPlayer()->isNeutral())
        {
          u->saveInitialState();
          this->staticNeutralUnits.insert(u);
          if ( u->_getType.isMineralField() )
            this->staticMinerals.insert(u);
          else if (u->_getType == UnitTypes::Resource_Vespene_Geyser)
            this->staticGeysers.insert(u);
        }
      }
    }
  }
  //---------------------------------------------- UPDATE UNITS ----------------------------------------------
  void GameImpl::updateUnits()
  {
    static Unitset selectedU;

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
    for ( Unit ui : selectedU )
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      if ( u->exists() )
        selectedUnitSet.insert(u);
      else
      {
        u->setSelected(false);
        if ( u->self )
          u->self->isSelected = false;
      } // if exists
    } // for each in selectedU

    // Get all units under disruption web and dark swarm
    for ( Unit u : neutralUnits )
    {
      BWAPI::UnitType ut = u->getType();
      if ( ut != UnitTypes::Spell_Dark_Swarm &&
           ut != UnitTypes::Spell_Disruption_Web )
        continue;

      int r = u->getRight()  - (ut == UnitTypes::Spell_Disruption_Web ? 1 : 0);
      int b = u->getBottom() - (ut == UnitTypes::Spell_Disruption_Web ? 1 : 0);

      // Get units under the ability that are affected
      auto affectedUnitsCondition = [](Unit u) {
        return !u->getType().isSpell() && !u->isFlying();
      };
      Unitset unitsInside = this->getUnitsInRectangle(u->getLeft(), u->getTop(), r, b, affectedUnitsCondition);
      for ( Unit uInsidei : unitsInside )
      {
        UnitImpl *uInside = static_cast<UnitImpl*>(uInsidei);
        // Assign the boolean for whatever spell the unit is under
        if ( ut == UnitTypes::Spell_Dark_Swarm )
          uInside->self->isUnderDarkSwarm = true;
        else if ( ut == UnitTypes::Spell_Disruption_Web )
          uInside->self->isUnderDWeb    = true;
      }
    } // for each neutral units
  } // updateUnits

  //------------------------------------------- CENTER ON SELECTED -------------------------------------------
  void GameImpl::moveToSelectedUnits()
  {
    // Retrieve the average position of the entire unit set
    Position pos( this->selectedUnitSet.getPosition() );

    // Move the position to the center of the screen
    pos -= Position(BW::BWDATA::GameScreenBuffer.width() / 2, BW::BWDATA::GameScreenBuffer.height() / 2 - 40);

    // Make this position a valid position
    pos = makeValid(pos);

    // Move to the screen position
    this->setScreenPosition(pos.x, pos.y);
  }

}


