#pragma once
#include <BWAPI.h>
#include <algorithm>

#include "UnitImpl.h"

namespace BWAPI
{
  using namespace Filter;
  namespace Templates
  {
    //--------------------------------------------- FORWARD DECLARATIONS -------------------------------------
    static inline bool canAttackMove(Unit thisUnit, bool checkCommandibility = true);
    static inline bool canAttackMoveGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canAttackUnit(Unit thisUnit, bool checkCommandibility = true);
    static inline bool canAttackUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canAttackUnitGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canAttackUnitGrouped(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canSetRallyPosition(Unit thisUnit, bool checkCommandibility = true);
    static inline bool canSetRallyUnit(Unit thisUnit, bool checkCommandibility = true);
    static inline bool canSetRallyUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canMove(Unit thisUnit, bool checkCommandibility = true);
    static inline bool canRightClickPosition(Unit thisUnit, bool checkCommandibility = true);
    static inline bool canRightClickPositionGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canRightClickUnit(Unit thisUnit, bool checkCommandibility = true);
    static inline bool canRightClickUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canRightClickUnitGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canRightClickUnitGrouped(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canUseTechWithoutTarget(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canUseTechUnit(Unit thisUnit, BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnits = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canUseTechPosition(Unit thisUnit, BWAPI::TechType tech, Position target, bool checkTargetsPositions = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    //--------------------------------------------- HAS POWER ------------------------------------------------
    const bool bPsiFieldMask[10][16] = {
      { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
      { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
      { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
      { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
      { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }
    };
    static inline bool hasPower(int x, int y, UnitType unitType, const Unitset &pylons)
    {
      if ( unitType >= 0 && unitType < UnitTypes::None && (!unitType.requiresPsi() || !unitType.isBuilding()) )
        return true;

      // Loop through all pylons for the current player
      for (Unit i : pylons)
      {
        if ( !i->exists() || !i->isCompleted() )
          continue;

        Position p = i->getPosition();
        if ( abs(p.x - x) >= 256 )
          continue;

        if ( abs(p.y - y) >= 160 )
          continue;

        if ( bPsiFieldMask[(y - p.y + 160) / 32][(x - p.x + 256) / 32] )
          return true;
      }
      return false;
    }
    //-------------------------------------------- UNIT FINDER -----------------------------------------------
    template <class finder, typename _T>
    void iterateUnitFinder(finder *finder_x, finder *finder_y, int finderCount, int left, int top, int right, int bottom, const _T &callback)
    {
      // Note that the native finder in Broodwar uses an id between 1 and 1700, 0 being an unused entry
      // IDs provided by the client are BWAPI IDs, which are not bound
      std::unordered_map<unsigned, unsigned> finderFlags;
      
      // Declare some variables
      int r = right, b = bottom;
      bool isWidthExtended  = right - left + 1 < UnitTypes::maxUnitWidth();
      bool isHeightExtended = top - bottom + 1 < UnitTypes::maxUnitHeight();

      // Check if the location is smaller than the largest unit
      if ( isWidthExtended )
        r += UnitTypes::maxUnitWidth();
      if ( isHeightExtended )
        b += UnitTypes::maxUnitHeight();

      // Obtain finder indexes for all bounds
      finder *p_xend = finder_x + finderCount;
      finder *p_yend = finder_y + finderCount;

      // Create finder elements for compatibility with stl functions
      finder finderVal;

      // Search for the values using built-in binary search algorithm and comparator
      const auto cmp = [](const finder& a,const finder& b){ return a.searchValue < b.searchValue; };

      finderVal.searchValue = left;
      finder *pLeft   = std::lower_bound(finder_x, p_xend, finderVal, cmp);

      finderVal.searchValue = top;
      finder *pTop    = std::lower_bound(finder_y, p_yend, finderVal, cmp);

      finderVal.searchValue = r+1;
      finder *pRight  = std::upper_bound(pLeft, p_xend, finderVal, cmp);

      finderVal.searchValue = b+1;
      finder *pBottom = std::upper_bound(pTop, p_yend, finderVal, cmp);

      // Iterate the X entries of the finder
      for ( finder *px = pLeft; px < pRight; ++px )
      {
        int iUnitIndex = px->unitIndex;
        if ( finderFlags[iUnitIndex] == 0 )
        {
          if ( isWidthExtended )  // If width is small, check unit bounds
          {
            Unit u = static_cast<GameImpl*>(BroodwarPtr)->_unitFromIndex(iUnitIndex);
            if ( u && u->getLeft() <= right )
              finderFlags[iUnitIndex] = 1;
          }
          else
            finderFlags[iUnitIndex] = 1;
        }
      }
      // Iterate the Y entries of the finder
      for ( finder *py = pTop; py < pBottom; ++py )
      {
        int iUnitIndex = py->unitIndex;
        if ( finderFlags[iUnitIndex] == 1 )
        {
          if ( isHeightExtended ) // If height is small, check unit bounds
          {
            Unit u = static_cast<GameImpl*>(BroodwarPtr)->_unitFromIndex(iUnitIndex);
            if ( u && u->getTop() <= bottom )
              finderFlags[iUnitIndex] = 2;
          }
          else
            finderFlags[iUnitIndex] = 2;
        }
      }
      // Final Iteration
      for ( finder *px = pLeft; px < pRight; ++px )
      {
        int iUnitIndex = px->unitIndex;
        if ( finderFlags[iUnitIndex] == 2 )
        {
          Unit u = static_cast<GameImpl*>(BroodwarPtr)->_unitFromIndex(iUnitIndex);
          if ( u && u->exists() )
            callback(u);
        }
        // Reset finderFlags so that callback isn't called for duplicates
        finderFlags[iUnitIndex] = 0;
      }
    }
    //------------------------------------------- CAN BUILD HERE ---------------------------------------------
    static inline bool canBuildHere(Unit builder, TilePosition position, UnitType type, bool checkExplored)
    {
      Broodwar->setLastError(Errors::Unbuildable_Location);

      if (builder && type.isAddon())
      {
        position += TilePosition(4, 1); // addon build offset
      }

      // lt = left top, rb = right bottom
      TilePosition lt = position;
      TilePosition rb = lt + type.tileSize();

      // Map limit check
      if ( !lt.isValid() || !(Position(rb) - Position(1,1)).isValid() )
        return false;

      //if the unit is a refinery, we just need to check the set of geysers to see if the position
      //matches one of them (and the type is still vespene geyser)
      if ( type.isRefinery() )
      {
        for (Unit g : Broodwar->getGeysers())
        {
          if (g->getTilePosition() == position)
          {
            if (g->isVisible() && g->getType() != UnitTypes::Resource_Vespene_Geyser)
              return false;
            return Broodwar->setLastError();
          }
        }
        return false;
      }

      // Tile buildability check
      for ( int x = lt.x; x < rb.x; ++x )
      {
        for ( int y = lt.y; y < rb.y; ++y )
        {
          // Check if tile is buildable/unoccupied and explored.
          if ( !Broodwar->isBuildable(x, y) || ( checkExplored && !Broodwar->isExplored(x,y)) )
            return false; // @TODO: Error code for !isExplored ??
        }
      }

      // Check if builder is capable of reaching the building site
      if ( builder )
      {
        if ( !builder->getType().isBuilding() )
        {
          if ( !builder->hasPath( Position(lt) + Position(type.tileSize())/2 ) )
            return false;
        }
        else if ( !builder->getType().isFlyingBuilding() && type != UnitTypes::Zerg_Nydus_Canal && !type.isFlagBeacon() )
        {
          return false;
        }
      }

      // Ground unit dimension check
      if ( type != UnitTypes::Special_Start_Location )
      {
        Position targPos = Position(lt) + Position( type.tileSize() )/2;
        Unitset unitsInRect( Broodwar->getUnitsInRectangle(Position(lt), Position(rb), !IsFlying    &&
                                                                                        !IsLoaded   &&
                                                                                        [&builder, &type](Unit u){ return u != builder || type == UnitTypes::Zerg_Nydus_Canal;} &&
                                                                                        GetLeft   <= targPos.x + type.dimensionRight()  &&
                                                                                        GetTop    <= targPos.y + type.dimensionDown()   &&
                                                                                        GetRight  >= targPos.x - type.dimensionLeft()   &&
                                                                                        GetBottom >= targPos.y - type.dimensionUp() )    );
        for (Unit u : unitsInRect)
        {
          BWAPI::UnitType iterType = u->getType();
          // Addons can be placed over units that can move, pushing them out of the way
          if ( !(type.isAddon() && iterType.canMove()) )
            return false;
        }

        // Creep Check
        // Note: Zerg structures that don't require creep can still be placed on creep
        bool needsCreep = type.requiresCreep();
        if (type.getRace() != Races::Zerg || needsCreep)
        {
          for (int x = lt.x; x < rb.x; ++x)
          {
            for (int y = lt.y; y < rb.y; ++y)
            {
              if (needsCreep != Broodwar->hasCreep(x, y))
                return false;
            }
          }
        }

        // Power Check
        if ( type.requiresPsi() && !Broodwar->hasPower(lt, type) )
          return false;

      } //don't ignore units

      // Resource Check (CC, Nex, Hatch)
      if ( type.isResourceDepot() )
      {
        for (BWAPI::Unit m : Broodwar->getStaticMinerals())
        {
          TilePosition tp = m->getInitialTilePosition();
          if ( (Broodwar->isVisible(tp) || Broodwar->isVisible(tp.x + 1, tp.y)) && !m->isVisible() )
              continue; // tile position is visible, but mineral is not => mineral does not exist
          if (tp.x > lt.x - 5 &&
              tp.y > lt.y - 4 &&
              tp.x < lt.x + 7 &&
              tp.y < lt.y + 6)
            return false;
        }
        for (BWAPI::Unit g : Broodwar->getStaticGeysers())
        {
          TilePosition tp = g->getInitialTilePosition();
          if (tp.x > lt.x - 7 &&
              tp.y > lt.y - 5 &&
              tp.x < lt.x + 7 &&
              tp.y < lt.y + 6)
            return false;
        }
      }

      // A building can build an addon at a different location (i.e. automatically lifts (if not already lifted)
      // then lands at the new location before building the addon), so we need to do similar checks for the
      // location that the building will be when it builds the addon.
      if ( builder && !builder->getType().isAddon() && type.isAddon() )
      {
        if (!canBuildHere(builder, lt - TilePosition(4, 1), builder->getType(), checkExplored))
          return false;
      }

      //if the build site passes all these tests, return true.
      return Broodwar->setLastError();
    }
    //------------------------------------------- CAN MAKE ---------------------------------------------------
    static inline bool canMake(Unit builder, UnitType type)
    {
      // Error checking
      Broodwar->setLastError();
      if ( !Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      // Check if the unit type is available (UMS game)
      if ( !Broodwar->self()->isUnitAvailable(type) )
        return Broodwar->setLastError(Errors::Access_Denied);

      // Get the required UnitType
      BWAPI::UnitType requiredType = type.whatBuilds().first;

      Player pSelf = Broodwar->self();
      if ( builder != nullptr ) // do checks if a builder is provided
      {
        // Check if the owner of the unit is you
        if (builder->getPlayer() != pSelf)
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

        // Check if this unit can actually build the unit type
        if ( requiredType == UnitTypes::Zerg_Larva && builderType.producesLarva() )
        {
          if ( builder->getLarva().size() == 0 )
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        }
        else if ( builderType != requiredType )
        {
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        }

        // Carrier/Reaver space checking
        int max_amt;
        switch ( builderType )
        {
        case UnitTypes::Enum::Protoss_Carrier:
        case UnitTypes::Enum::Hero_Gantrithor:
          // Get max interceptors
          max_amt = 4;
          if ( pSelf->getUpgradeLevel(UpgradeTypes::Carrier_Capacity) > 0 || builderType == UnitTypes::Hero_Gantrithor )
            max_amt += 4;

          // Check if there is room
          if ( builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= max_amt )
            return Broodwar->setLastError(Errors::Insufficient_Space);
          break;
        case UnitTypes::Enum::Protoss_Reaver:
        case UnitTypes::Enum::Hero_Warbringer:
          // Get max scarabs
          max_amt = 5;
          if ( pSelf->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0 || builderType == UnitTypes::Hero_Warbringer )
            max_amt += 5;

          // check if there is room
          if (builder->getScarabCount() + (int)builder->getTrainingQueue().size() >= max_amt)
            return Broodwar->setLastError(Errors::Insufficient_Space);
          break;
        }
      } // if builder != nullptr

      // Check if player has enough minerals
      if ( pSelf->minerals() < type.mineralPrice() )
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      // Check if player has enough gas
      if ( pSelf->gas() < type.gasPrice() )
        return Broodwar->setLastError(Errors::Insufficient_Gas);
      
      // Check if player has enough supplies
      BWAPI::Race typeRace = type.getRace();
      const int supplyRequired = type.supplyRequired() * (type.isTwoUnitsInOneEgg() ? 2 : 1);
      if (supplyRequired > 0 && pSelf->supplyTotal(typeRace) < pSelf->supplyUsed(typeRace) + supplyRequired - (requiredType.getRace() == typeRace ? requiredType.supplyRequired() : 0))
        return Broodwar->setLastError(Errors::Insufficient_Supply);

      UnitType addon = UnitTypes::None;
      for (auto &it : type.requiredUnits())
      {
        if (it.first.isAddon())
          addon = it.first;

        if (!pSelf->hasUnitTypeRequirement(it.first, it.second))
          return Broodwar->setLastError(Errors::Insufficient_Tech);
      }

      if (type.requiredTech() != TechTypes::None && !pSelf->hasResearched(type.requiredTech()))
        return Broodwar->setLastError(Errors::Insufficient_Tech);

      if ( builder && 
           addon != UnitTypes::None &&
           addon.whatBuilds().first == type.whatBuilds().first &&
           (!builder->getAddon() || builder->getAddon()->getType() != addon) )
        return Broodwar->setLastError(Errors::Insufficient_Tech);

      return true;
    }
    //------------------------------------------- CAN COMMAND ------------------------------------------------
    static inline bool canCommand(Unit thisUnit)
    {
      // Basic header
      Broodwar->setLastError();
      if ( thisUnit->getPlayer() != Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if ( !thisUnit->exists() )
        return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

      // Global can be ordered check
      if ( thisUnit->isLockedDown() || 
           thisUnit->isMaelstrommed() || 
           thisUnit->isStasised()  || 
           !thisUnit->isPowered() ||
           thisUnit->getOrder() == Orders::ZergBirth ||
           thisUnit->isLoaded() )
      {
        if ( !thisUnit->getType().producesLarva() )
        {
          return Broodwar->setLastError(Errors::Unit_Busy);
        }
        else
        {
          Unitset larvae( thisUnit->getLarva() );
          for (Unit larva : larvae)
          {
            if ( canCommand(larva) )
              return Broodwar->setLastError();
          }
          return Broodwar->setLastError(Errors::Unit_Busy);
        }
      }

      UnitType uType = thisUnit->getType();
      if ( uType == UnitTypes::Protoss_Interceptor ||
           uType == UnitTypes::Terran_Vulture_Spider_Mine ||
           uType == UnitTypes::Spell_Scanner_Sweep ||
           uType == UnitTypes::Special_Map_Revealer )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isCompleted() &&
           ( uType == UnitTypes::Protoss_Pylon ||
             uType == UnitTypes::Terran_Supply_Depot ||
             uType.isResourceContainer() ||
             uType == UnitTypes::Protoss_Shield_Battery ||
             uType == UnitTypes::Terran_Nuclear_Missile ||
             uType.isPowerup() ||
             ( uType.isSpecialBuilding() && !uType.isFlagBeacon() ) ) )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( !thisUnit->isCompleted() &&
           !uType.isBuilding() &&
           !thisUnit->isMorphing() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return Broodwar->setLastError();
    }
    static inline bool canCommandGrouped(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->getType().isBuilding() || thisUnit->getType().isCritter() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN TARGET -------------------------------------------------
    static inline bool canTargetUnit(Unit targetUnit)
    {
      if ( !targetUnit || !targetUnit->exists() )
        return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
      if ( !targetUnit->isCompleted() &&
           !targetUnit->getType().isBuilding() &&
           !targetUnit->isMorphing() &&
           targetUnit->getType() != UnitTypes::Protoss_Archon &&
           targetUnit->getType() != UnitTypes::Protoss_Dark_Archon )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( targetUnit->getType() == UnitTypes::Spell_Scanner_Sweep ||
           targetUnit->getType() == UnitTypes::Spell_Dark_Swarm ||
           targetUnit->getType() == UnitTypes::Spell_Disruption_Web ||
           targetUnit->getType() == UnitTypes::Special_Map_Revealer )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canTargetUnit(Unit thisUnit, Unit targetUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canTargetUnit(targetUnit) )
        return false;

      return true;
    }
    //------------------------------------------- CAN ATTACK -------------------------------------------------
    static inline bool canAttack(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canAttackMove(thisUnit, false) && !canAttackUnit(thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canAttack(Unit thisUnit, PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( target.isUnit() )
      {
        if ( target.getUnit() == nullptr )
          return Broodwar->setLastError(Errors::Invalid_Parameter);
        if ( !canAttackUnit(thisUnit, target.getUnit(), checkCanTargetUnit, checkCanIssueCommandType, false) )
          return false;
      }
      else
      {
        if ( !canAttackMove(thisUnit, false) )
          return false;
      }

      return true;
    }
    static inline bool canAttackGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( !canAttackMoveGrouped(thisUnit, false, false) && !canAttackUnitGrouped(thisUnit, false, false) )
        return false;

      return true;
    }
    static inline bool canAttackGrouped(Unit thisUnit, PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( target.isUnit() )
      {
        if ( target.getUnit() == nullptr )
          return Broodwar->setLastError(Errors::Invalid_Parameter);
        if ( !canAttackUnitGrouped(thisUnit, target.getUnit(), checkCanTargetUnit, checkCanIssueCommandTypeGrouped, false, false) )
          return false;
      }
      else
      {
        if ( !canAttackMoveGrouped(thisUnit, false, false) )
          return false;
      }

      return true;
    }
    //------------------------------------------- CAN ATTACK MOVE --------------------------------------------
    static inline bool canAttackMove(Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( ( thisUnit->getType() != UnitTypes::Terran_Medic && !canAttackUnit(thisUnit, false) ) || !canMove(thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canAttackMoveGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( !thisUnit->getType().canMove() &&
           thisUnit->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode &&
           thisUnit->getType() != UnitTypes::Zerg_Cocoon &&
           thisUnit->getType() != UnitTypes::Zerg_Lurker_Egg )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN ATTACK UNIT --------------------------------------------
    static inline bool canAttackUnit(Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType().groundWeapon() == WeaponTypes::None && thisUnit->getType().airWeapon() == WeaponTypes::None )
      {
        if ( thisUnit->getType() == UnitTypes::Protoss_Carrier || thisUnit->getType() == UnitTypes::Hero_Gantrithor )
        {
          if ( thisUnit->getInterceptorCount() <= 0 )
            return Broodwar->setLastError(Errors::Unable_To_Hit);
        }
        else if ( thisUnit->getType() == UnitTypes::Protoss_Reaver || thisUnit->getType() == UnitTypes::Hero_Warbringer )
        {
          if ( thisUnit->getScarabCount() <= 0 )
            return Broodwar->setLastError(Errors::Unable_To_Hit);
        }
        else
          return Broodwar->setLastError(Errors::Unable_To_Hit);
      }
      if ( thisUnit->getType() == UnitTypes::Zerg_Lurker )
      {
        if ( !thisUnit->isBurrowed() )
          return Broodwar->setLastError(Errors::Unable_To_Hit);
      }
      else if ( thisUnit->isBurrowed() )
        return Broodwar->setLastError(Errors::Unable_To_Hit);
      if ( !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return Broodwar->setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canAttackUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canAttackUnit(thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      if ( targetUnit->isInvincible() )
        return Broodwar->setLastError(Errors::Unable_To_Hit);

      UnitType type = thisUnit->getType();
      bool targetInAir = targetUnit->isFlying();
      WeaponType weapon = targetInAir ? type.airWeapon() : type.groundWeapon();
      
      if (weapon == WeaponTypes::None)
      {
        switch (type)
        {
        case UnitTypes::Enum::Protoss_Carrier:
        case UnitTypes::Enum::Hero_Gantrithor:
          break;
        case UnitTypes::Enum::Protoss_Reaver:
        case UnitTypes::Enum::Hero_Warbringer:
          if (targetInAir)
            Broodwar->setLastError(Errors::Unable_To_Hit);
          break;
        default:
          return Broodwar->setLastError(Errors::Unable_To_Hit);
        }
      }

      if ( !thisUnit->getType().canMove() && !thisUnit->isInWeaponRange(targetUnit) )
        return Broodwar->setLastError(Errors::Out_Of_Range);

      if ( thisUnit->getType() == UnitTypes::Zerg_Lurker && !thisUnit->isInWeaponRange(targetUnit) )
        return Broodwar->setLastError(Errors::Out_Of_Range);

      if ( targetUnit == thisUnit )
        return Broodwar->setLastError(Errors::Invalid_Parameter);

      return true;
    }
    static inline bool canAttackUnitGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);

      if ( !thisUnit->getType().canMove() &&
           thisUnit->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      if ( !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      if ( thisUnit->getType() == UnitTypes::Zerg_Lurker )
      {
        if ( !thisUnit->isBurrowed() )
          return Broodwar->setLastError(Errors::Unable_To_Hit);
      }
      else if ( thisUnit->isBurrowed() )
        return Broodwar->setLastError(Errors::Unable_To_Hit);

      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return Broodwar->setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canAttackUnitGrouped(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandTypeGrouped, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( checkCanIssueCommandTypeGrouped && !canAttackUnitGrouped(thisUnit, false, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      if ( targetUnit->isInvincible() )
        return Broodwar->setLastError(Errors::Unable_To_Hit);

      if ( thisUnit->getType() == UnitTypes::Zerg_Lurker && !thisUnit->isInWeaponRange(targetUnit) )
        return Broodwar->setLastError(Errors::Out_Of_Range);

      if ( thisUnit->getType() == UnitTypes::Zerg_Queen &&
           ( targetUnit->getType() != UnitTypes::Terran_Command_Center ||
             targetUnit->getHitPoints() >= 750 || targetUnit->getHitPoints() <= 0 ) )
        return Broodwar->setLastError(Errors::Invalid_Parameter);

      if ( targetUnit == thisUnit )
        return Broodwar->setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN BUILD --------------------------------------------------
    static inline bool canBuild(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->isConstructing() ||
           !thisUnit->isCompleted()   ||
           (thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canBuild(Unit thisUnit, UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canBuild(thisUnit, false) )
        return false;

      if ( !Broodwar->canMake(uType, thisUnit) )
        return false;

      if ( !uType.isBuilding() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->getAddon() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canBuild(Unit thisUnit, UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canBuild(thisUnit, false) )
        return false;

      if ( checkTargetUnitType && !canBuild(thisUnit, uType, false, false) )
        return false;

      if ( tilePos.isValid() == false )
        return Broodwar->setLastError(Errors::Invalid_Tile_Position);

      if ( !Broodwar->canBuildHere(tilePos, uType, thisUnit, true) )
        return false;

      return true;
    }
    //------------------------------------------- CAN BUILD ADDON --------------------------------------------
    static inline bool canBuildAddon(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->isConstructing() ||
           !thisUnit->isCompleted()   ||
           thisUnit->isLifted() ||
           (thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->getAddon() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( !thisUnit->getType().canBuildAddon() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canBuildAddon(Unit thisUnit, UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canBuildAddon(thisUnit, false) )
        return false;

      if ( !Broodwar->canMake(uType, thisUnit) )
        return false;

      if ( !uType.isAddon() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      if ( !Broodwar->canBuildHere(thisUnit->getTilePosition(), uType, thisUnit) )
        return false;

      return true;
    }
    //------------------------------------------- CAN TRAIN --------------------------------------------------
    static inline bool canTrain(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->getType().producesLarva() )
      {
        if ( !thisUnit->isConstructing() && thisUnit->isCompleted() )
          return Broodwar->setLastError();
        Unitset larvae( thisUnit->getLarva() );
        for (Unit larva : larvae)
        {
          if ( !larva->isConstructing() && larva->isCompleted() && canCommand(larva) )
            return Broodwar->setLastError();
        }
        return Broodwar->setLastError(Errors::Unit_Busy);
      }

      if ( thisUnit->isConstructing() ||
           !thisUnit->isCompleted()   ||
           thisUnit->isLifted() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !thisUnit->getType().canProduce() &&
           thisUnit->getType() != UnitTypes::Enum::Terran_Nuclear_Silo &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Hydralisk &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Mutalisk &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Creep_Colony &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Spire &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Larva )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canTrain(Unit thisUnit, UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canTrain(thisUnit, false) )
        return false;

      if ( thisUnit->getType().producesLarva() )
      {
        if ( uType.whatBuilds().first == UnitTypes::Zerg_Larva )
        {
          bool foundCommandableLarva = false;
          Unitset larvae( thisUnit->getLarva() );
          for (Unit larva : larvae)
          {
            if ( canTrain(larva, true) )
            {
              foundCommandableLarva = true;
              thisUnit = larva;
              break;
            }
          }
          if (!foundCommandableLarva)
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        }
        else if ( thisUnit->isConstructing() ||
                  !thisUnit->isCompleted() )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }

      if ( !Broodwar->canMake(uType, thisUnit) )
        return false;

      if ( uType.isAddon() || ( uType.isBuilding() && !thisUnit->getType().isBuilding() ) )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( uType == UnitTypes::Enum::Zerg_Larva || uType == UnitTypes::Enum::Zerg_Egg || uType == UnitTypes::Enum::Zerg_Cocoon )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN MORPH --------------------------------------------------
    static inline bool canMorph(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->getType().producesLarva() )
      {
        if ( !thisUnit->isConstructing() && thisUnit->isCompleted() && ( !thisUnit->getType().isBuilding() || thisUnit->isIdle() ) )
          return Broodwar->setLastError();
        Unitset larvae( thisUnit->getLarva() );
        for (Unit larva : larvae)
        {
          if ( !larva->isConstructing() && larva->isCompleted() && canCommand(larva) )
            return Broodwar->setLastError();
        }
        return Broodwar->setLastError(Errors::Unit_Busy);
      }

      if ( thisUnit->isConstructing() ||
           !thisUnit->isCompleted()   ||
           (thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType() != UnitTypes::Enum::Zerg_Hydralisk &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Mutalisk &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Creep_Colony &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Spire &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Hatchery &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Lair &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Hive &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Larva )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canMorph(Unit thisUnit, UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canMorph(thisUnit, false) )
        return false;

      if ( thisUnit->getType().producesLarva() )
      {
        if ( uType.whatBuilds().first == UnitTypes::Zerg_Larva )
        {
          bool foundCommandableLarva = false;
          Unitset larvae( thisUnit->getLarva() );
          for (Unit larva : larvae)
          {
            if ( canMorph(larva, true) )
            {
              foundCommandableLarva = true;
              thisUnit = larva;
              break;
            }
          }
          if (!foundCommandableLarva)
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        }
        else if ( thisUnit->isConstructing() ||
                  !thisUnit->isCompleted()   ||
                  (thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }

      if ( !Broodwar->canMake(uType, thisUnit) )
        return false;
      if ( uType == UnitTypes::Enum::Zerg_Larva || uType == UnitTypes::Enum::Zerg_Egg || uType == UnitTypes::Enum::Zerg_Cocoon )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN RESEARCH -----------------------------------------------
    static inline bool canResearch(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canResearch(Unit thisUnit, TechType type, bool checkCanIssueCommandType = true)
    {
      // Error checking
      if ( !Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if ( thisUnit )
      {
        if (thisUnit->getPlayer() != Broodwar->self())
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        if (thisUnit->getType() != type.whatResearches())
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( checkCanIssueCommandType && ( thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted() ) )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }
      if (Broodwar->self()->isResearching(type))
        return Broodwar->setLastError(Errors::Currently_Researching);

      if (Broodwar->self()->hasResearched(type))
        return Broodwar->setLastError(Errors::Already_Researched);

      if ( !Broodwar->self()->isResearchAvailable(type) )
        return Broodwar->setLastError(Errors::Access_Denied);

      if (Broodwar->self()->minerals() < type.mineralPrice())
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      if (Broodwar->self()->gas() < type.gasPrice())
        return Broodwar->setLastError(Errors::Insufficient_Gas);

      if (!Broodwar->self()->hasUnitTypeRequirement(type.requiredUnit()))
        return Broodwar->setLastError(Errors::Insufficient_Tech);

      return Broodwar->setLastError();
    }
    //------------------------------------------- CAN UPGRADE ------------------------------------------------
    static inline bool canUpgrade(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canUpgrade(Unit thisUnit, UpgradeType type, bool checkCanIssueCommandType = true)
    {
      Player self = Broodwar->self();
      if ( !self )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if ( thisUnit )
      {
        if (thisUnit->getPlayer() != self)
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        if (thisUnit->getType() != type.whatUpgrades())
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( checkCanIssueCommandType && ( thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted() ) )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }
      int nextLvl = self->getUpgradeLevel(type)+1;
      
      if (!self->hasUnitTypeRequirement(type.whatUpgrades()))
        return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
      
      if (!self->hasUnitTypeRequirement(type.whatsRequired(nextLvl)))
        return Broodwar->setLastError(Errors::Insufficient_Tech);
      
      if (self->isUpgrading(type))
        return Broodwar->setLastError(Errors::Currently_Upgrading);

      if ( self->getUpgradeLevel(type) >= self->getMaxUpgradeLevel(type) )
        return Broodwar->setLastError(Errors::Fully_Upgraded);

      if ( self->minerals() < type.mineralPrice(nextLvl) )
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      if ( self->gas() < type.gasPrice(nextLvl) )
        return Broodwar->setLastError(Errors::Insufficient_Gas);

      return Broodwar->setLastError();
    }
    //------------------------------------------- CAN SET RALLY POINT ----------------------------------------
    static inline bool canSetRallyPoint(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canSetRallyPosition(thisUnit, false) && !canSetRallyUnit(thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canSetRallyPoint(Unit thisUnit, PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( target.isUnit() )
      {
        if ( target.getUnit() == nullptr )
          return Broodwar->setLastError(Errors::Invalid_Parameter);
        if ( !canSetRallyUnit(thisUnit, target.getUnit(), checkCanTargetUnit, checkCanIssueCommandType, false) )
          return false;
      }
      else
      {
        if ( !canSetRallyPosition(thisUnit, false) )
          return false;
      }

      return true;
    }
    //------------------------------------------- CAN SET RALLY POSITION -------------------------------------
    static inline bool canSetRallyPosition(Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().canProduce() || !thisUnit->getType().isBuilding() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isLifted() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN SET RALLY UNIT -----------------------------------------
    static inline bool canSetRallyUnit(Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().canProduce() || !thisUnit->getType().isBuilding() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isLifted() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canSetRallyUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canSetRallyUnit(thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN MOVE ---------------------------------------------------
    static inline bool canMove(Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() )
      {
        if ( !thisUnit->isInterruptible() )
          return Broodwar->setLastError(Errors::Unit_Busy);
        if ( !thisUnit->getType().canMove() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        if ( thisUnit->isBurrowed() )
          return Broodwar->setLastError(Errors::Incompatible_State);
        if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
          return Broodwar->setLastError(Errors::Unit_Busy);
        if ( thisUnit->getType() == UnitTypes::Zerg_Larva )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
      }
      else
      {
        if ( !thisUnit->getType().isFlyingBuilding() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        if ( !thisUnit->isLifted() )
          return Broodwar->setLastError(Errors::Incompatible_State);
      }

      if (!thisUnit->isCompleted())
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canMoveGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( !thisUnit->getType().canMove() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isCompleted() && !thisUnit->isMorphing() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN PATROL -------------------------------------------------
    static inline bool canPatrol(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canMove(thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canPatrolGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( !canMoveGrouped(thisUnit, false, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN FOLLOW -------------------------------------------------
    static inline bool canFollow(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canMove(thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canFollow(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canFollow(thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      if ( targetUnit == thisUnit )
        return Broodwar->setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN GATHER -------------------------------------------------
    static inline bool canGather(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !thisUnit->getType().isWorker() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->getPowerUp() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isBurrowed() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return Broodwar->setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canGather(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canGather(thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      UnitType uType = targetUnit->getType();
      if ( !uType.isResourceContainer() || uType == UnitTypes::Resource_Vespene_Geyser )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      if ( !targetUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Unit_Busy);

      if ( !thisUnit->hasPath(targetUnit->getPosition()) )
        return Broodwar->setLastError(Errors::Unreachable_Location);

      if ( uType.isRefinery() && targetUnit->getPlayer() != Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      return true;
    }
    //------------------------------------------- CAN RETURN CARGO -------------------------------------------
    static inline bool canReturnCargo(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !thisUnit->getType().isWorker() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isCarryingGas() && !thisUnit->isCarryingMinerals() )
        return Broodwar->setLastError(Errors::Insufficient_Ammo);
      if ( thisUnit->isBurrowed() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return Broodwar->setLastError(Errors::Unit_Busy);

      return true;
    }
    //------------------------------------------- CAN HOLD POSITION ------------------------------------------
    static inline bool canHoldPosition(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() )
      {
        if ( !thisUnit->getType().canMove() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        if ( thisUnit->isBurrowed() && thisUnit->getType() != UnitTypes::Zerg_Lurker )
          return Broodwar->setLastError(Errors::Incompatible_State);
        if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
          return Broodwar->setLastError(Errors::Unit_Busy);
        if ( thisUnit->getType() == UnitTypes::Zerg_Larva )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
      }
      else
      {
        if ( !thisUnit->getType().isFlyingBuilding() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        if ( !thisUnit->isLifted() )
          return Broodwar->setLastError(Errors::Incompatible_State);
      }

      if (!thisUnit->isCompleted())
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN STOP ---------------------------------------------------
    static inline bool canStop(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->isBurrowed() && thisUnit->getType() != UnitTypes::Zerg_Lurker )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->getType().isBuilding() && !thisUnit->isLifted() &&
           thisUnit->getType() != UnitTypes::Protoss_Photon_Cannon &&
           thisUnit->getType() != UnitTypes::Zerg_Sunken_Colony &&
           thisUnit->getType() != UnitTypes::Zerg_Spore_Colony &&
           thisUnit->getType() != UnitTypes::Terran_Missile_Turret )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN REPAIR -------------------------------------------------
    static inline bool canRepair(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType() != BWAPI::UnitTypes::Terran_SCV )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return Broodwar->setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canRepair(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canRepair(thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      UnitType targType = targetUnit->getType();
      if ( targType.getRace() != BWAPI::Races::Terran || !targType.isMechanical() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( targetUnit->getHitPoints() == targType.maxHitPoints() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( !targetUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( targetUnit == thisUnit )
        return Broodwar->setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN BURROW -------------------------------------------------
    static inline bool canBurrow(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canUseTechWithoutTarget(thisUnit, BWAPI::TechTypes::Burrowing, true, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN UNBURROW -----------------------------------------------
    static inline bool canUnburrow(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBurrowable() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Unburrowing )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CLOAK --------------------------------------------------
    static inline bool canCloak(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canUseTechWithoutTarget(thisUnit, thisUnit->getType().cloakingTech(), true, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN DECLOAK ------------------------------------------------
    static inline bool canDecloak(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->getType().cloakingTech() == TechTypes::None )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->getSecondaryOrder() != Orders::Cloak )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN SIEGE --------------------------------------------------
    static inline bool canSiege(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canUseTechWithoutTarget(thisUnit, BWAPI::TechTypes::Tank_Siege_Mode, true, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN UNSIEGE ------------------------------------------------
    static inline bool canUnsiege(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->isSieged() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN LIFT ---------------------------------------------------
    static inline bool canLift(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isFlyingBuilding() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isLifted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( !thisUnit->isIdle() )
        return Broodwar->setLastError(Errors::Unit_Busy);

      return true;
    }
    //------------------------------------------- CAN LAND ---------------------------------------------------
    static inline bool canLand(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isFlyingBuilding() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isLifted() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canLand(Unit thisUnit, TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if (checkCanIssueCommandType && !canLand(thisUnit, checkCommandibility) )
        return false;

      if ( !canBuildHere(nullptr, target, thisUnit->getType(), true) )
        return false;

      return true;
    }
    //------------------------------------------- CAN LOAD ---------------------------------------------------
    static inline bool canLoad(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType() == UnitTypes::Zerg_Overlord && Broodwar->self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0 )
        return Broodwar->setLastError(Errors::Insufficient_Tech);
      if ( thisUnit->isBurrowed() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType() == UnitTypes::Zerg_Larva )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canLoad(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canLoad(thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      //target must also be owned by self
      if (targetUnit->getPlayer() != Broodwar->self())
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if ( targetUnit->isLoaded() || !targetUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Unit_Busy);

      // verify upgrade for Zerg Overlord
      if ( targetUnit->getType() == UnitTypes::Zerg_Overlord && Broodwar->self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0 )
        return Broodwar->setLastError(Errors::Insufficient_Tech);

      int thisUnitSpaceProvided = thisUnit->getType().spaceProvided();
      int targetSpaceProvided = targetUnit->getType().spaceProvided();
      if ( thisUnitSpaceProvided <= 0 && targetSpaceProvided <= 0 )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      const BWAPI::Unit unitToBeLoaded = ( thisUnitSpaceProvided > 0 ? targetUnit : thisUnit );
      if ( unitToBeLoaded->getType().canMove() == false || unitToBeLoaded->getType().isFlyer() || unitToBeLoaded->getType().spaceRequired() > 8 )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);
      if ( !unitToBeLoaded->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( unitToBeLoaded->isBurrowed() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      const BWAPI::Unit unitThatLoads = ( thisUnitSpaceProvided > 0 ? thisUnit : targetUnit );
      if ( unitThatLoads->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      if ( unitThatLoads->getType() == UnitTypes::Terran_Bunker )
      {
        if ( !unitToBeLoaded->getType().isOrganic() || unitToBeLoaded->getType().getRace() != Races::Terran )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        if ( !unitToBeLoaded->hasPath(unitThatLoads->getPosition()) )
          return Broodwar->setLastError(Errors::Unreachable_Location);
      }

      int freeSpace = ( thisUnitSpaceProvided > 0 ? thisUnitSpaceProvided : targetSpaceProvided );
      Unitset loadedUnits = unitThatLoads->getLoadedUnits();
      for (Unit u : loadedUnits)
      {
        const int requiredSpace = u->getType().spaceRequired();
        if ( requiredSpace > 0 && requiredSpace < 8 )
          freeSpace -= requiredSpace;
      }
      if ( unitToBeLoaded->getType().spaceRequired() > freeSpace )
        return Broodwar->setLastError(Errors::Insufficient_Space);

      return true;
    }
    //------------------------------------------- CAN UNLOAD -------------------------------------------------
    static inline bool canUnloadWithOrWithoutTarget(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);

      if ( thisUnit->getLoadedUnits().size() == 0 )
        return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

      // Check overlord tech
      if ( thisUnit->getType() == UnitTypes::Zerg_Overlord && Broodwar->self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
        return Broodwar->setLastError(Errors::Insufficient_Tech);

      if ( thisUnit->getType().spaceProvided() <= 0 )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canUnloadAtPosition(Unit thisUnit, Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUnloadWithOrWithoutTarget(thisUnit, false) )
        return false;

      if ( thisUnit->getType() != UnitTypes::Terran_Bunker )
      {
        if ( WalkPosition(targDropPos.x/8, targDropPos.y/8).isValid() == false )
          return Broodwar->setLastError(Errors::Invalid_Tile_Position);
        else if ( !Broodwar->isWalkable(targDropPos.x/8, targDropPos.y/8) )
          return Broodwar->setLastError(Errors::Unreachable_Location);
      }

      return true;
    }
    static inline bool canUnload(Unit thisUnit, bool checkCommandibility = true)
    {
      return canUnloadAtPosition(thisUnit, thisUnit->getPosition(), true, checkCommandibility);
    }
    static inline bool canUnload(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUnloadWithOrWithoutTarget(thisUnit, false) )
        return false;

      if ( checkPosition && !canUnloadAtPosition(thisUnit, thisUnit->getPosition(), false, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      if ( !targetUnit->isLoaded() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      if ( targetUnit->getTransport() != thisUnit )
        return Broodwar->setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN UNLOAD ALL ---------------------------------------------
    static inline bool canUnloadAll(Unit thisUnit, bool checkCommandibility = true)
    {
      return canUnloadAtPosition(thisUnit, thisUnit->getPosition(), true, checkCommandibility);
    }
    //------------------------------------------- CAN UNLOAD ALL POSITION ------------------------------------
    static inline bool canUnloadAllPosition(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canUnloadWithOrWithoutTarget(thisUnit, false) )
        return false;

      if ( thisUnit->getType() == UnitTypes::Terran_Bunker )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canUnloadAllPosition(Unit thisUnit, Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUnloadAllPosition(thisUnit, false) )
        return false;

      if ( !canUnloadAtPosition(thisUnit, targDropPos, false, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN RIGHT CLICK --------------------------------------------
    static inline bool canRightClick(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !canRightClickPosition(thisUnit, false) && !canRightClickUnit(thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canRightClick(Unit thisUnit, PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( target.isUnit() )
      {
        if ( target.getUnit() == nullptr )
          return Broodwar->setLastError(Errors::Invalid_Parameter);
        if ( !canRightClickUnit(thisUnit, target.getUnit(), checkCanTargetUnit, checkCanIssueCommandType, false) )
          return false;
      }
      else
      {
        if ( !canRightClickPosition(thisUnit, false) )
          return false;
      }

      return true;
    }
    static inline bool canRightClickGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( !canRightClickPositionGrouped(thisUnit, false, false) && !canRightClickUnitGrouped(thisUnit, false, false) )
        return false;

      return true;
    }
    static inline bool canRightClickGrouped(Unit thisUnit, PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( target.isUnit() )
      {
        if ( target.getUnit() == nullptr )
          return Broodwar->setLastError(Errors::Invalid_Parameter);
        if ( !canRightClickUnitGrouped(thisUnit, target.getUnit(), checkCanTargetUnit, checkCanIssueCommandTypeGrouped, false, false) )
          return false;
      }
      else
      {
        if ( !canRightClickPositionGrouped(thisUnit, false, false) )
          return false;
      }

      return true;
    }
    //------------------------------------------- CAN RIGHT CLICK POSITION -----------------------------------
    static inline bool canRightClickPosition(Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !canMove(thisUnit, false) && !canSetRallyPosition(thisUnit, false) )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canRightClickPositionGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !canMoveGrouped(thisUnit, false, false) )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN RIGHT CLICK UNIT ---------------------------------------
    static inline bool canRightClickUnit(Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !canFollow(thisUnit, false) &&
           !canAttackUnit(thisUnit, false) &&
           !canLoad(thisUnit, false) &&
           !canSetRallyUnit(thisUnit, false) )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canRightClickUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canRightClickUnit(thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      if ( !targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()) &&
           !canAttackUnit(thisUnit, targetUnit, false, true, false) )
        return false;

      if ( !canFollow(thisUnit, targetUnit, false, true, false) &&
           !canLoad(thisUnit, targetUnit, false, true, false) &&
           !canSetRallyUnit(thisUnit, targetUnit, false, true, false) )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canRightClickUnitGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !canFollow(thisUnit, false) &&
           !canAttackUnitGrouped(thisUnit, false, false) &&
           !canLoad(thisUnit, false) )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canRightClickUnitGrouped(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      if ( checkCanIssueCommandType && !canRightClickUnitGrouped(thisUnit, false, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      if ( !targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()) &&
           !canAttackUnitGrouped(thisUnit, targetUnit, false, true, false, false) )
        return false;

      if ( !canFollow(thisUnit, targetUnit, false, true, false) &&
           !canLoad(thisUnit, targetUnit, false, true, false) )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN HALT CONSTRUCTION --------------------------------------
    static inline bool canHaltConstruction(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->getOrder() != Orders::ConstructingBuilding )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL CONSTRUCTION ------------------------------------
    static inline bool canCancelConstruction(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      if ( thisUnit->isCompleted() || (thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit()) )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL ADDON -------------------------------------------
    static inline bool canCancelAddon(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getAddon() || thisUnit->getAddon()->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN CANCEL TRAIN -------------------------------------------
    static inline bool canCancelTrain(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->isTraining() )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL TRAIN SLOT --------------------------------------
    static inline bool canCancelTrainSlot(Unit thisUnit, bool checkCommandibility = true)
    {
      return canCancelTrain(thisUnit, checkCommandibility);
    }
    static inline bool canCancelTrainSlot(Unit thisUnit, int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canCancelTrainSlot(thisUnit, false) )
        return false;

      if ( !thisUnit->isTraining() || (thisUnit->getTrainingQueue().size() <= (unsigned int)slot && slot >= 0) )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL MORPH -------------------------------------------
    static inline bool canCancelMorph(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->isMorphing() || (!thisUnit->isCompleted() && thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit()) )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN CANCEL RESEARCH ----------------------------------------
    static inline bool canCancelResearch(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->getOrder() != Orders::ResearchTech )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL UPGRADE -----------------------------------------
    static inline bool canCancelUpgrade(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( thisUnit->getOrder() != Orders::Upgrade )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN USE TECH -----------------------------------------------
    static inline bool canUseTechWithOrWithoutTarget(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return Broodwar->setLastError(Errors::Unit_Busy);
      if ( !thisUnit->isCompleted() )
        return Broodwar->setLastError(Errors::Incompatible_State);
      if ( thisUnit->isHallucination() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canUseTechWithOrWithoutTarget(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false) )
        return false;

      // researched check
      if ( !thisUnit->getType().isHero() && !Broodwar->self()->hasResearched(tech) && thisUnit->getType() != UnitTypes::Zerg_Lurker )
        return Broodwar->setLastError(Errors::Insufficient_Tech);

      // energy check
      if ( thisUnit->getEnergy() < tech.energyCost() )
        return Broodwar->setLastError(Errors::Insufficient_Energy);

      // unit check
      if ( tech != TechTypes::Burrowing && !tech.whatUses().contains(thisUnit->getType()) )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      switch (tech)
      {
        case TechTypes::Enum::Spider_Mines:
          if ( thisUnit->getSpiderMineCount() <= 0 )
            return Broodwar->setLastError(Errors::Insufficient_Ammo);
          return true;

        case TechTypes::Enum::Tank_Siege_Mode:
          if ( thisUnit->isSieged() )
            return Broodwar->setLastError(Errors::Incompatible_State);
          if ( thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging )
            return Broodwar->setLastError(Errors::Unit_Busy);
          return true;

        case TechTypes::Enum::Cloaking_Field:
        case TechTypes::Enum::Personnel_Cloaking:
          if ( thisUnit->getSecondaryOrder() == Orders::Cloak )
            return Broodwar->setLastError(Errors::Incompatible_State);
          return true;

        case TechTypes::Enum::Burrowing:
          if ( !thisUnit->getType().isBurrowable() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          if ( thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Burrowing || thisUnit->getOrder() == Orders::Unburrowing )
            return Broodwar->setLastError(Errors::Incompatible_State);
          return true;

        case TechTypes::Enum::None:
          return Broodwar->setLastError(Errors::Incompatible_TechType);

        case TechTypes::Enum::Nuclear_Strike:
          if ( thisUnit->getPlayer()->completedUnitCount(UnitTypes::Terran_Nuclear_Missile) == 0 )
            return Broodwar->setLastError(Errors::Insufficient_Ammo);
          return true;

        case TechTypes::Enum::Unknown:
          return Broodwar->setLastError(Errors::Incompatible_TechType);
      }

      return true;
    }
    static inline bool canUseTech(Unit thisUnit, BWAPI::TechType tech, PositionOrUnit target = nullptr, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( target.isUnit() )
      {
        if ( target.getUnit() == nullptr )
        {
          if ( !canUseTechWithoutTarget(thisUnit, tech, checkCanIssueCommandType, false) )
            return false;
        }
        else
        {
          if ( !canUseTechUnit(thisUnit, tech, target.getUnit(), checkCanTargetUnit, checkTargetsType, checkCanIssueCommandType, false) )
            return false;
        }
      }
      else
      {
        if ( !canUseTechPosition(thisUnit, tech, target.getPosition(), checkTargetsType, checkCanIssueCommandType, false) )
          return false;
      }

      return true;
    }
    static inline bool canUseTechWithoutTarget(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false) )
        return false;

      if ( !canUseTechWithOrWithoutTarget(thisUnit, tech, false, false) )
        return false;
      if ( tech.targetsUnit() || tech.targetsPosition() || tech == TechTypes::None || tech == TechTypes::Unknown || tech == TechTypes::Lurker_Aspect)
        return Broodwar->setLastError(Errors::Incompatible_TechType);

      return true;
    }
    //------------------------------------------- CAN USE TECH UNIT ------------------------------------------
    static inline bool canUseTechUnit(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false) )
        return false;

      if ( !canUseTechWithOrWithoutTarget(thisUnit, tech, false, false) )
        return false;
      if ( !tech.targetsUnit() )
        return Broodwar->setLastError(Errors::Incompatible_TechType);

      return true;
    }
    static inline bool canUseTechUnit(Unit thisUnit, BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit, bool checkTargetsUnits, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false) )
        return false;

      if ( checkTargetsUnits && !canUseTechUnit(thisUnit, tech, false, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false) )
        return false;

      UnitType targetType = targetUnit->getType();

      switch (tech)
      {
        case TechTypes::Enum::Archon_Warp:
          if ( targetType != UnitTypes::Protoss_High_Templar )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          if ( targetUnit->getPlayer() != thisUnit->getPlayer() )
            return Broodwar->setLastError(Errors::Unit_Not_Owned);
          break;

        case TechTypes::Enum::Dark_Archon_Meld:
          if ( targetType != UnitTypes::Protoss_Dark_Templar )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          if ( targetUnit->getPlayer() != thisUnit->getPlayer() )
            return Broodwar->setLastError(Errors::Unit_Not_Owned);
          break;

        case TechTypes::Enum::Consume:
          if ( targetUnit->getPlayer() != thisUnit->getPlayer() )
            return Broodwar->setLastError(Errors::Unit_Not_Owned);
          if ( targetType.getRace() != Races::Zerg || targetType == UnitTypes::Zerg_Larva )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Spawn_Broodlings:
          if ( ( !targetType.isOrganic() && !targetType.isMechanical() ) ||
               targetType.isRobotic() ||
               targetType.isFlyer() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Lockdown:
          if ( !targetType.isMechanical() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Healing:
          if ( targetUnit->getHitPoints() == targetType.maxHitPoints() )
            return Broodwar->setLastError(Errors::Incompatible_State);
          if ( !targetType.isOrganic() ||
               targetType.isFlyer() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          if ( !targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()) )
            return Broodwar->setLastError(Errors::Invalid_Parameter);
          break;

        case TechTypes::Enum::Mind_Control:
          if ( targetUnit->getPlayer() == thisUnit->getPlayer() )
            return Broodwar->setLastError(Errors::Invalid_Parameter);
          if ( targetType == UnitTypes::Protoss_Interceptor ||
               targetType == UnitTypes::Terran_Vulture_Spider_Mine ||
               targetType == UnitTypes::Zerg_Lurker_Egg ||
               targetType == UnitTypes::Zerg_Cocoon ||
               targetType == UnitTypes::Zerg_Larva ||
               targetType == UnitTypes::Zerg_Egg )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Feedback:
          if ( !targetType.isSpellcaster() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Infestation:
          if ( targetType != UnitTypes::Terran_Command_Center ||
               targetUnit->getHitPoints() >= 750 || targetUnit->getHitPoints() <= 0 )
            return Broodwar->setLastError(Errors::Invalid_Parameter);
          break;
      }

      switch (tech)
      {
        case TechTypes::Enum::Archon_Warp:
        case TechTypes::Enum::Dark_Archon_Meld:
          if ( !thisUnit->hasPath(targetUnit->getPosition()) )
            return Broodwar->setLastError(Errors::Unreachable_Location);
          if ( targetUnit->isHallucination() )
            return Broodwar->setLastError(Errors::Invalid_Parameter);
          if ( targetUnit->isMaelstrommed() )
            return Broodwar->setLastError(Errors::Incompatible_State);
          // Fall through (don't break).
        case TechTypes::Enum::Parasite:
        case TechTypes::Enum::Irradiate:
        case TechTypes::Enum::Optical_Flare:
        case TechTypes::Enum::Spawn_Broodlings:
        case TechTypes::Enum::Lockdown:
        case TechTypes::Enum::Defensive_Matrix:
        case TechTypes::Enum::Hallucination:
        case TechTypes::Enum::Healing:
        case TechTypes::Enum::Restoration:
        case TechTypes::Enum::Mind_Control:
        case TechTypes::Enum::Consume:
        case TechTypes::Enum::Feedback:
        case TechTypes::Enum::Yamato_Gun:
          if ( targetUnit->isStasised() )
            return Broodwar->setLastError(Errors::Incompatible_State);
          break;
      }

      switch (tech)
      {
        case TechTypes::Enum::Yamato_Gun:
          if ( targetUnit->isInvincible() )
            return Broodwar->setLastError(Errors::Invalid_Parameter);
          break;

        case TechTypes::Enum::Parasite:
        case TechTypes::Enum::Irradiate:
        case TechTypes::Enum::Optical_Flare:
        case TechTypes::Enum::Spawn_Broodlings:
        case TechTypes::Enum::Lockdown:
        case TechTypes::Enum::Defensive_Matrix:
        case TechTypes::Enum::Hallucination:
        case TechTypes::Enum::Healing:
        case TechTypes::Enum::Restoration:
        case TechTypes::Enum::Mind_Control:
          if ( targetUnit->isInvincible() )
            return Broodwar->setLastError(Errors::Invalid_Parameter);
          // Fall through (don't break).
        case TechTypes::Enum::Consume:
        case TechTypes::Enum::Feedback:
          if ( targetType.isBuilding() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          break;
      }

      if ( targetUnit == thisUnit )
        return Broodwar->setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN USE TECH POSITION --------------------------------------
    static inline bool canUseTechPosition(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false) )
        return false;

      if ( !canUseTechWithOrWithoutTarget(thisUnit, tech, false, false) )
        return false;
      if ( !tech.targetsPosition() )
        return Broodwar->setLastError(Errors::Incompatible_TechType);

      return true;
    }
    static inline bool canUseTechPosition(Unit thisUnit, BWAPI::TechType tech, Position target, bool checkTargetsPositions, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false) )
        return false;

      if ( checkTargetsPositions && !canUseTechPosition(thisUnit, tech, false, false) )
        return false;

      if ( tech == TechTypes::Enum::Spider_Mines && !thisUnit->hasPath(target) )
        return Broodwar->setLastError(Errors::Unreachable_Location);

      return true;
    }
    //------------------------------------------- CAN PLACE COP ----------------------------------------------
    static inline bool canPlaceCOP(Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( !thisUnit->getType().isFlagBeacon() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      if ( static_cast<UnitImpl*>(thisUnit)->self->buttonset == 228 || thisUnit->getOrder() != BWAPI::Orders::CTFCOPInit )
        return Broodwar->setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canPlaceCOP(Unit thisUnit, TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if (checkCanIssueCommandType && !canPlaceCOP(thisUnit, checkCommandibility) )
        return false;

      if ( !canBuildHere(thisUnit, target, thisUnit->getType(), true) )
        return false;

      return true;
    }
    //------------------------------------------- CAN ISSUE COMMAND TYPE -------------------------------------
    static inline bool canIssueCommandType(Unit thisUnit, BWAPI::UnitCommandType ct, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      switch (ct)
      {
        case UnitCommandTypes::Enum::Attack_Move:
          return canAttackMove(thisUnit, false);

        case UnitCommandTypes::Enum::Attack_Unit:
          return canAttackUnit(thisUnit, false);

        case UnitCommandTypes::Enum::Build:
          return canBuild(thisUnit, false);

        case UnitCommandTypes::Enum::Build_Addon:
          return canBuildAddon(thisUnit, false);

        case UnitCommandTypes::Enum::Train:
          return canTrain(thisUnit, false);

        case UnitCommandTypes::Enum::Morph:
          return canMorph(thisUnit, false);

        case UnitCommandTypes::Enum::Research:
          return canResearch(thisUnit, false);

        case UnitCommandTypes::Enum::Upgrade:
          return canUpgrade(thisUnit, false);

        case UnitCommandTypes::Enum::Set_Rally_Position:
          return canSetRallyPosition(thisUnit, false);

        case UnitCommandTypes::Enum::Set_Rally_Unit:
          return canSetRallyUnit(thisUnit, false);

        case UnitCommandTypes::Enum::Move:
          return canMove(thisUnit, false);

        case UnitCommandTypes::Enum::Patrol:
          return canPatrol(thisUnit, false);

        case UnitCommandTypes::Enum::Hold_Position:
          return canHoldPosition(thisUnit, false);

        case UnitCommandTypes::Enum::Stop:
          return canStop(thisUnit, false);

        case UnitCommandTypes::Enum::Follow:
          return canFollow(thisUnit, false);

        case UnitCommandTypes::Enum::Gather:
          return canGather(thisUnit, false);

        case UnitCommandTypes::Enum::Return_Cargo:
          return canReturnCargo(thisUnit, false);

        case UnitCommandTypes::Enum::Repair:
          return canRepair(thisUnit, false);

        case UnitCommandTypes::Enum::Burrow:
          return canBurrow(thisUnit, false);

        case UnitCommandTypes::Enum::Unburrow:
          return canUnburrow(thisUnit, false);

        case UnitCommandTypes::Enum::Cloak:
          return canCloak(thisUnit, false);

        case UnitCommandTypes::Enum::Decloak:
          return canDecloak(thisUnit, false);

        case UnitCommandTypes::Enum::Siege:
          return canSiege(thisUnit, false);

        case UnitCommandTypes::Enum::Unsiege:
          return canUnsiege(thisUnit, false);

        case UnitCommandTypes::Enum::Lift:
          return canLift(thisUnit, false);

        case UnitCommandTypes::Enum::Land:
          return canLand(thisUnit, false);

        case UnitCommandTypes::Enum::Load:
          return canLoad(thisUnit, false);

        case UnitCommandTypes::Enum::Unload:
          return canUnload(thisUnit, false);

        case UnitCommandTypes::Enum::Unload_All:
          return canUnloadAll(thisUnit, false);

        case UnitCommandTypes::Enum::Unload_All_Position:
          return canUnloadAllPosition(thisUnit, false);

        case UnitCommandTypes::Enum::Right_Click_Position:
          return canRightClickPosition(thisUnit, false);

        case UnitCommandTypes::Enum::Right_Click_Unit:
          return canRightClickUnit(thisUnit, false);

        case UnitCommandTypes::Enum::Halt_Construction:
          return canHaltConstruction(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Construction:
          return canCancelConstruction(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Addon:
          return canCancelAddon(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Train:
          return canCancelTrain(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Train_Slot:
          return canCancelTrainSlot(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Morph:
          return canCancelMorph(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Research:
          return canCancelResearch(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Upgrade:
          return canCancelUpgrade(thisUnit, false);

        case UnitCommandTypes::Enum::Use_Tech:
        case UnitCommandTypes::Enum::Use_Tech_Unit:
        case UnitCommandTypes::Enum::Use_Tech_Position:
          return canUseTechWithOrWithoutTarget(thisUnit, false);

        case UnitCommandTypes::Enum::Place_COP:
          return canPlaceCOP(thisUnit, false);
      }

      return true;
    }
    static inline bool canIssueCommandTypeGrouped(Unit thisUnit, BWAPI::UnitCommandType ct, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      switch (ct)
      {
        case UnitCommandTypes::Enum::Attack_Move:
          return canAttackMoveGrouped(thisUnit, false, false);

        case UnitCommandTypes::Enum::Attack_Unit:
          return canAttackUnitGrouped(thisUnit, false, false);

        case UnitCommandTypes::Enum::Build:
          return false;

        case UnitCommandTypes::Enum::Build_Addon:
          return false;

        case UnitCommandTypes::Enum::Train:
          return canTrain(thisUnit, false);

        case UnitCommandTypes::Enum::Morph:
          return canMorph(thisUnit, false);

        case UnitCommandTypes::Enum::Research:
          return false;

        case UnitCommandTypes::Enum::Upgrade:
          return false;

        case UnitCommandTypes::Enum::Set_Rally_Position:
          return false;

        case UnitCommandTypes::Enum::Set_Rally_Unit:
          return false;

        case UnitCommandTypes::Enum::Move:
          return canMoveGrouped(thisUnit, false, false);

        case UnitCommandTypes::Enum::Patrol:
          return canPatrolGrouped(thisUnit, false, false);

        case UnitCommandTypes::Enum::Hold_Position:
          return canHoldPosition(thisUnit, false);

        case UnitCommandTypes::Enum::Stop:
          return canStop(thisUnit, false);

        case UnitCommandTypes::Enum::Follow:
          return canFollow(thisUnit, false);

        case UnitCommandTypes::Enum::Gather:
          return canGather(thisUnit, false);

        case UnitCommandTypes::Enum::Return_Cargo:
          return canReturnCargo(thisUnit, false);

        case UnitCommandTypes::Enum::Repair:
          return canRepair(thisUnit, false);

        case UnitCommandTypes::Enum::Burrow:
          return canBurrow(thisUnit, false);

        case UnitCommandTypes::Enum::Unburrow:
          return canUnburrow(thisUnit, false);

        case UnitCommandTypes::Enum::Cloak:
          return canCloak(thisUnit, false);

        case UnitCommandTypes::Enum::Decloak:
          return canDecloak(thisUnit, false);

        case UnitCommandTypes::Enum::Siege:
          return canSiege(thisUnit, false);

        case UnitCommandTypes::Enum::Unsiege:
          return canUnsiege(thisUnit, false);

        case UnitCommandTypes::Enum::Lift:
          return false;

        case UnitCommandTypes::Enum::Land:
          return false;

        case UnitCommandTypes::Enum::Load:
          return canLoad(thisUnit, false);

        case UnitCommandTypes::Enum::Unload:
          return false;

        case UnitCommandTypes::Enum::Unload_All:
          return false;

        case UnitCommandTypes::Enum::Unload_All_Position:
          return canUnloadAllPosition(thisUnit, false);

        case UnitCommandTypes::Enum::Right_Click_Position:
          return canRightClickPositionGrouped(thisUnit, false, false);

        case UnitCommandTypes::Enum::Right_Click_Unit:
          return canRightClickUnitGrouped(thisUnit, false, false);

        case UnitCommandTypes::Enum::Halt_Construction:
          return canHaltConstruction(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Construction:
          return false;

        case UnitCommandTypes::Enum::Cancel_Addon:
          return false;

        case UnitCommandTypes::Enum::Cancel_Train:
          return false;

        case UnitCommandTypes::Enum::Cancel_Train_Slot:
          return false;

        case UnitCommandTypes::Enum::Cancel_Morph:
          return canCancelMorph(thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Research:
          return false;

        case UnitCommandTypes::Enum::Cancel_Upgrade:
          return false;

        case UnitCommandTypes::Enum::Use_Tech:
        case UnitCommandTypes::Enum::Use_Tech_Unit:
        case UnitCommandTypes::Enum::Use_Tech_Position:
          return canUseTechWithOrWithoutTarget(thisUnit, false);

        case UnitCommandTypes::Enum::Place_COP:
          return false;
      }

      return true;
    }
    //------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
    static inline bool canIssueCommand(Unit thisUnit, UnitCommand c, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      BWAPI::UnitCommandType ct = c.type;
      if ( checkCanIssueCommandType && !canIssueCommandType(thisUnit, ct, false) )
        return false;

      switch (ct)
      {
        case UnitCommandTypes::Enum::Attack_Move:
          return true;

        case UnitCommandTypes::Enum::Attack_Unit:
          return canAttackUnit(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Build:
          return canBuild(thisUnit, c.getUnitType(), BWAPI::TilePosition(c.x, c.y), checkCanBuildUnitType, false, false);

        case UnitCommandTypes::Enum::Build_Addon:
          return canBuildAddon(thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Train:
          return canTrain(thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Morph:
          return canMorph(thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Research:
          return Broodwar->canResearch(c.getTechType(), thisUnit, false);

        case UnitCommandTypes::Enum::Upgrade:
          return Broodwar->canUpgrade(c.getUpgradeType(), thisUnit, false);

        case UnitCommandTypes::Enum::Set_Rally_Position:
          return true;

        case UnitCommandTypes::Enum::Set_Rally_Unit:
          return canSetRallyUnit(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Move:
          return true;

        case UnitCommandTypes::Enum::Patrol:
          return true;

        case UnitCommandTypes::Enum::Hold_Position:
          return true;

        case UnitCommandTypes::Enum::Stop:
          return true;

        case UnitCommandTypes::Enum::Follow:
          return canFollow(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Gather:
          return canGather(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Return_Cargo:
          return true;

        case UnitCommandTypes::Enum::Repair:
          return canRepair(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Burrow:
          return true;

        case UnitCommandTypes::Enum::Unburrow:
          return true;

        case UnitCommandTypes::Enum::Cloak:
          return true;

        case UnitCommandTypes::Enum::Decloak:
          return true;

        case UnitCommandTypes::Enum::Siege:
          return true;

        case UnitCommandTypes::Enum::Unsiege:
          return true;

        case UnitCommandTypes::Enum::Lift:
          return true;

        case UnitCommandTypes::Enum::Land:
          return canLand(thisUnit, BWAPI::TilePosition(c.x, c.y), false, false);

        case UnitCommandTypes::Enum::Load:
          return canLoad(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Unload:
          return canUnload(thisUnit, c.target, checkCanTargetUnit, false, false, false);

        case UnitCommandTypes::Enum::Unload_All:
          return true;

        case UnitCommandTypes::Enum::Unload_All_Position:
          return canUnloadAllPosition(thisUnit, c.getTargetPosition(), false, false);

        case UnitCommandTypes::Enum::Right_Click_Position:
          return true;

        case UnitCommandTypes::Enum::Right_Click_Unit:
          return canRightClickUnit(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Halt_Construction:
          return true;

        case UnitCommandTypes::Enum::Cancel_Construction:
          return true;

        case UnitCommandTypes::Enum::Cancel_Addon:
          return true;

        case UnitCommandTypes::Enum::Cancel_Train:
          return true;

        case UnitCommandTypes::Enum::Cancel_Train_Slot:
          return canCancelTrainSlot(thisUnit, c.extra, false, false);

        case UnitCommandTypes::Enum::Cancel_Morph:
          return true;

        case UnitCommandTypes::Enum::Cancel_Research:
          return true;

        case UnitCommandTypes::Enum::Cancel_Upgrade:
          return true;

        case UnitCommandTypes::Enum::Use_Tech:
          return canUseTechWithoutTarget(thisUnit, c.extra, false, false);

        case UnitCommandTypes::Enum::Use_Tech_Unit:
          return canUseTechUnit(thisUnit, c.extra, c.target, checkCanTargetUnit, checkCanUseTechUnitOnUnits, false, false);

        case UnitCommandTypes::Enum::Use_Tech_Position:
          return canUseTechPosition(thisUnit, c.extra, c.getTargetPosition(), checkCanUseTechPositionOnPositions, false, false);

        case UnitCommandTypes::Enum::Place_COP:
          return canPlaceCOP(thisUnit, BWAPI::TilePosition(c.x, c.y), false, false);
      }

      return true;
    }
    static inline bool canIssueCommandGrouped(Unit thisUnit, UnitCommand c, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        Broodwar->setLastError();
      else if ( !canCommand(thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false) )
        return false;

      BWAPI::UnitCommandType ct = c.type;
      if ( checkCanIssueCommandType && !canIssueCommandTypeGrouped(thisUnit, ct, false, false) )
        return false;

      switch (ct)
      {
        case UnitCommandTypes::Enum::Attack_Move:
          return true;

        case UnitCommandTypes::Enum::Attack_Unit:
          return canAttackUnitGrouped(thisUnit, c.target, checkCanTargetUnit, false, false, false);

        case UnitCommandTypes::Enum::Build:
          return false;

        case UnitCommandTypes::Enum::Build_Addon:
          return false;

        case UnitCommandTypes::Enum::Train:
          return canTrain(thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Morph:
          return canMorph(thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Research:
          return false;

        case UnitCommandTypes::Enum::Upgrade:
          return false;

        case UnitCommandTypes::Enum::Set_Rally_Position:
          return false;

        case UnitCommandTypes::Enum::Set_Rally_Unit:
          return false;

        case UnitCommandTypes::Enum::Move:
          return true;

        case UnitCommandTypes::Enum::Patrol:
          return true;

        case UnitCommandTypes::Enum::Hold_Position:
          return true;

        case UnitCommandTypes::Enum::Stop:
          return true;

        case UnitCommandTypes::Enum::Follow:
          return canFollow(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Gather:
          return canGather(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Return_Cargo:
          return true;

        case UnitCommandTypes::Enum::Repair:
          return canRepair(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Burrow:
          return true;

        case UnitCommandTypes::Enum::Unburrow:
          return true;

        case UnitCommandTypes::Enum::Cloak:
          return true;

        case UnitCommandTypes::Enum::Decloak:
          return true;

        case UnitCommandTypes::Enum::Siege:
          return true;

        case UnitCommandTypes::Enum::Unsiege:
          return true;

        case UnitCommandTypes::Enum::Lift:
          return false;

        case UnitCommandTypes::Enum::Land:
          return false;

        case UnitCommandTypes::Enum::Load:
          return canLoad(thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Unload:
          return false;

        case UnitCommandTypes::Enum::Unload_All:
          return false;

        case UnitCommandTypes::Enum::Unload_All_Position:
          return canUnloadAllPosition(thisUnit, c.getTargetPosition(), false, false);

        case UnitCommandTypes::Enum::Right_Click_Position:
          return true;

        case UnitCommandTypes::Enum::Right_Click_Unit:
          return canRightClickUnitGrouped(thisUnit, c.target, checkCanTargetUnit, false, false, false);

        case UnitCommandTypes::Enum::Halt_Construction:
          return true;

        case UnitCommandTypes::Enum::Cancel_Construction:
          return false;

        case UnitCommandTypes::Enum::Cancel_Addon:
          return false;

        case UnitCommandTypes::Enum::Cancel_Train:
          return false;

        case UnitCommandTypes::Enum::Cancel_Train_Slot:
          return false;

        case UnitCommandTypes::Enum::Cancel_Morph:
          return true;

        case UnitCommandTypes::Enum::Cancel_Research:
          return false;

        case UnitCommandTypes::Enum::Cancel_Upgrade:
          return false;

        case UnitCommandTypes::Enum::Use_Tech:
          return canUseTechWithoutTarget(thisUnit, c.extra, false, false);

        case UnitCommandTypes::Enum::Use_Tech_Unit:
          return canUseTechUnit(thisUnit, c.extra, c.target, checkCanTargetUnit, checkCanUseTechUnitOnUnits, false, false);

        case UnitCommandTypes::Enum::Use_Tech_Position:
          return canUseTechPosition(thisUnit, c.extra, c.getTargetPosition(), checkCanUseTechPositionOnPositions, false, false);

        case UnitCommandTypes::Enum::Place_COP:
          return false;
      }

      return true;
    }
  }
}
