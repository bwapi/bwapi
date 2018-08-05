#pragma once
#include <BWAPI.h>
#include <algorithm>

namespace BWAPI
{
  using namespace Filter;
  namespace Templates
  {
    //--------------------------------------------- FORWARD DECLARATIONS -------------------------------------
    static inline bool canAttackMove(Game const &game, Unit thisUnit, bool checkCommandibility = true);
    static inline bool canAttackMoveGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canAttackUnit(Game const &game, Unit thisUnit, bool checkCommandibility = true);
    static inline bool canAttackUnit(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canAttackUnitGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canAttackUnitGrouped(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canSetRallyPosition(Game const &game, Unit thisUnit, bool checkCommandibility = true);
    static inline bool canSetRallyUnit(Game const &game, Unit thisUnit, bool checkCommandibility = true);
    static inline bool canSetRallyUnit(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canMove(Game const &game, Unit thisUnit, bool checkCommandibility = true);
    static inline bool canRightClickPosition(Game const &game, Unit thisUnit, bool checkCommandibility = true);
    static inline bool canRightClickPositionGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canRightClickUnit(Game const &game, Unit thisUnit, bool checkCommandibility = true);
    static inline bool canRightClickUnit(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canRightClickUnitGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canRightClickUnitGrouped(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    static inline bool canUseTechWithoutTarget(Game const &game, Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canUseTechUnit(Game const &game, Unit thisUnit, BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnits = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    static inline bool canUseTechPosition(Game const &game, Unit thisUnit, BWAPI::TechType tech, Position target, bool checkTargetsPositions = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
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
    void iterateUnitFinder(Game const &game, finder *finder_x, finder *finder_y, int finderCount, int left, int top, int right, int bottom, const _T &callback)
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
            Unit u = game.getUnit(iUnitIndex);
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
            Unit u = game.getUnit(iUnitIndex);
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
          Unit u = game.getUnit(iUnitIndex);
          if ( u && u->exists() )
            callback(u);
        }
        // Reset finderFlags so that callback isn't called for duplicates
        finderFlags[iUnitIndex] = 0;
      }
    }
    //------------------------------------------- CAN BUILD HERE ---------------------------------------------
    static inline bool canBuildHere(Game const &game, Unit builder, TilePosition position, UnitType type, bool checkExplored)
    {
      game.setLastError(Errors::Unbuildable_Location);

      if (builder && type.isAddon())
      {
        position += TilePosition(4, 1); // addon build offset
      }

      // lt = left top, rb = right bottom
      TilePosition lt = position;
      TilePosition rb = lt + type.tileSize();

      // Map limit check
      if ( !game.isValid(lt) || !game.isValid(Position(rb) - Position(1,1)) )
        return false;

      //if the unit is a refinery, we just need to check the set of geysers to see if the position
      //matches one of them (and the type is still vespene geyser)
      if ( type.isRefinery() )
      {
        for (Unit g : game.getGeysers())
        {
          if (g->getTilePosition() == position)
          {
            if (g->isVisible() && g->getType() != UnitTypes::Resource_Vespene_Geyser)
              return false;
            return game.setLastError();
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
          if ( !game.isBuildable(x, y) || ( checkExplored && !game.isExplored(x,y)) )
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
        Unitset unitsInRect( game.getUnitsInRectangle(Position(lt), Position(rb), !IsFlying    &&
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
              if (needsCreep != game.hasCreep(x, y))
                return false;
            }
          }
        }

        // Power Check
        if ( type.requiresPsi() && !game.hasPower(lt, type) )
          return false;

      } //don't ignore units

      // Resource Check (CC, Nex, Hatch)
      if ( type.isResourceDepot() )
      {
        for (BWAPI::Unit m : game.getStaticMinerals())
        {
          TilePosition tp = m->getInitialTilePosition();
          if ( (game.isVisible(tp) || game.isVisible(tp.x + 1, tp.y)) && !m->isVisible() )
              continue; // tile position is visible, but mineral is not => mineral does not exist
          if (tp.x > lt.x - 5 &&
              tp.y > lt.y - 4 &&
              tp.x < lt.x + 7 &&
              tp.y < lt.y + 6)
            return false;
        }
        for (BWAPI::Unit g : game.getStaticGeysers())
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
        if (!canBuildHere(game, builder, lt - TilePosition(4, 1), builder->getType(), checkExplored))
          return false;
      }

      //if the build site passes all these tests, return true.
      return game.setLastError();
    }
    //------------------------------------------- CAN MAKE ---------------------------------------------------
    static inline bool canMake(Game const &game, Unit builder, UnitType type)
    {
      // Error checking
      game.setLastError();
      if ( !game.self() )
        return game.setLastError(Errors::Unit_Not_Owned);

      // Check if the unit type is available (UMS game)
      if ( !game.self()->isUnitAvailable(type) )
        return game.setLastError(Errors::Access_Denied);

      // Get the required UnitType
      BWAPI::UnitType requiredType = type.whatBuilds().first;

      Player pSelf = game.self();
      if ( builder != nullptr ) // do checks if a builder is provided
      {
        // Check if the owner of the unit is you
        if (builder->getPlayer() != pSelf)
          return game.setLastError(Errors::Unit_Not_Owned);

        BWAPI::UnitType builderType = builder->getType();
        if ( type == UnitTypes::Zerg_Nydus_Canal && builderType == UnitTypes::Zerg_Nydus_Canal )
        {
          if ( !builder->isCompleted() )
            return game.setLastError(Errors::Unit_Busy);

          if ( builder->getNydusExit() )
            return game.setLastError(Errors::Unknown);

          return true;
        }

        // Check if this unit can actually build the unit type
        if ( requiredType == UnitTypes::Zerg_Larva && builderType.producesLarva() )
        {
          if ( builder->getLarva().size() == 0 )
            return game.setLastError(Errors::Unit_Does_Not_Exist);
        }
        else if ( builderType != requiredType )
        {
          return game.setLastError(Errors::Incompatible_UnitType);
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
            return game.setLastError(Errors::Insufficient_Space);
          break;
        case UnitTypes::Enum::Protoss_Reaver:
        case UnitTypes::Enum::Hero_Warbringer:
          // Get max scarabs
          max_amt = 5;
          if ( pSelf->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0 || builderType == UnitTypes::Hero_Warbringer )
            max_amt += 5;

          // check if there is room
          if (builder->getScarabCount() + static_cast<int>(builder->getTrainingQueue().size()) >= max_amt)
            return game.setLastError(Errors::Insufficient_Space);
          break;
        }
      } // if builder != nullptr

      // Check if player has enough minerals
      if ( pSelf->minerals() < type.mineralPrice() )
        return game.setLastError(Errors::Insufficient_Minerals);

      // Check if player has enough gas
      if ( pSelf->gas() < type.gasPrice() )
        return game.setLastError(Errors::Insufficient_Gas);
      
      // Check if player has enough supplies
      BWAPI::Race typeRace = type.getRace();
      const int supplyRequired = type.supplyRequired() * (type.isTwoUnitsInOneEgg() ? 2 : 1);
      if (supplyRequired > 0 && pSelf->supplyTotal(typeRace) < pSelf->supplyUsed(typeRace) + supplyRequired - (requiredType.getRace() == typeRace ? requiredType.supplyRequired() : 0))
        return game.setLastError(Errors::Insufficient_Supply);

      UnitType addon = UnitTypes::None;
      for (auto &it : type.requiredUnits())
      {
        if (it.first.isAddon())
          addon = it.first;

        if (!pSelf->hasUnitTypeRequirement(it.first, it.second))
          return game.setLastError(Errors::Insufficient_Tech);
      }

      if (type.requiredTech() != TechTypes::None && !pSelf->hasResearched(type.requiredTech()))
        return game.setLastError(Errors::Insufficient_Tech);

      if ( builder && 
           addon != UnitTypes::None &&
           addon.whatBuilds().first == type.whatBuilds().first &&
           (!builder->getAddon() || builder->getAddon()->getType() != addon) )
        return game.setLastError(Errors::Insufficient_Tech);

      return true;
    }
    //------------------------------------------- CAN COMMAND ------------------------------------------------
    static inline bool canCommand(Game const &game, Unit thisUnit)
    {
      // Basic header
      game.setLastError();
      if ( thisUnit->getPlayer() != game.self() )
        return game.setLastError(Errors::Unit_Not_Owned);

      if ( !thisUnit->exists() )
        return game.setLastError(Errors::Unit_Does_Not_Exist);

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
          return game.setLastError(Errors::Unit_Busy);
        }
        else
        {
          Unitset larvae( thisUnit->getLarva() );
          for (Unit larva : larvae)
          {
            if ( canCommand(game, larva) )
              return game.setLastError();
          }
          return game.setLastError(Errors::Unit_Busy);
        }
      }

      UnitType uType = thisUnit->getType();
      if ( uType == UnitTypes::Protoss_Interceptor ||
           uType == UnitTypes::Terran_Vulture_Spider_Mine ||
           uType == UnitTypes::Spell_Scanner_Sweep ||
           uType == UnitTypes::Special_Map_Revealer )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isCompleted() &&
           ( uType == UnitTypes::Protoss_Pylon ||
             uType == UnitTypes::Terran_Supply_Depot ||
             uType.isResourceContainer() ||
             uType == UnitTypes::Protoss_Shield_Battery ||
             uType == UnitTypes::Terran_Nuclear_Missile ||
             uType.isPowerup() ||
             ( uType.isSpecialBuilding() && !uType.isFlagBeacon() ) ) )
        return game.setLastError(Errors::Incompatible_State);
      if ( !thisUnit->isCompleted() &&
           !uType.isBuilding() &&
           !thisUnit->isMorphing() )
        return game.setLastError(Errors::Incompatible_State);

      return game.setLastError();
    }
    static inline bool canCommandGrouped(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->getType().isBuilding() || thisUnit->getType().isCritter() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN TARGET -------------------------------------------------
    static inline bool canTargetUnit(Game const &game, Unit targetUnit)
    {
      if ( !targetUnit || !targetUnit->exists() )
        return targetUnit->getGame().setLastError(Errors::Unit_Does_Not_Exist);
      if ( !targetUnit->isCompleted() &&
           !targetUnit->getType().isBuilding() &&
           !targetUnit->isMorphing() &&
           targetUnit->getType() != UnitTypes::Protoss_Archon &&
           targetUnit->getType() != UnitTypes::Protoss_Dark_Archon )
        return targetUnit->getGame().setLastError(Errors::Incompatible_State);
      if ( targetUnit->getType() == UnitTypes::Spell_Scanner_Sweep ||
           targetUnit->getType() == UnitTypes::Spell_Dark_Swarm ||
           targetUnit->getType() == UnitTypes::Spell_Disruption_Web ||
           targetUnit->getType() == UnitTypes::Special_Map_Revealer )
        return targetUnit->getGame().setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canTargetUnit(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canTargetUnit(game, targetUnit) )
        return false;

      return true;
    }
    //------------------------------------------- CAN ATTACK -------------------------------------------------
    static inline bool canAttack(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canAttackMove(game, thisUnit, false) && !canAttackUnit(game, thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canAttack(Game const &game, Unit thisUnit, Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (!canAttackMove(game, thisUnit, false))
        return false;
      
      return true;
    }
    static inline bool canAttack(Game const &game, Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( target == nullptr )
        return game.setLastError(Errors::Invalid_Parameter);
      if ( !canAttackUnit(game, thisUnit, target, checkCanTargetUnit, checkCanIssueCommandType, false) )
        return false;
      
      return true;
    }
    static inline bool canAttackGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( !canAttackMoveGrouped(game, thisUnit, false, false) && !canAttackUnitGrouped(game, thisUnit, false, false) )
        return false;

      return true;
    }
    static inline bool canAttackGrouped(Game const &game, Unit thisUnit, Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false))
        return false;

      if (!canAttackMoveGrouped(game, thisUnit, false, false))
        return false;

      return true;
    }
    static inline bool canAttackGrouped(Game const &game, Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( target == nullptr )
        return game.setLastError(Errors::Invalid_Parameter);
      if ( !canAttackUnitGrouped(game, thisUnit, target, checkCanTargetUnit, checkCanIssueCommandTypeGrouped, false, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN ATTACK MOVE --------------------------------------------
    static inline bool canAttackMove(Game const &game, Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( ( thisUnit->getType() != UnitTypes::Terran_Medic && !canAttackUnit(game, thisUnit, false) ) || !canMove(game, thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canAttackMoveGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( !thisUnit->getType().canMove() &&
           thisUnit->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode &&
           thisUnit->getType() != UnitTypes::Zerg_Cocoon &&
           thisUnit->getType() != UnitTypes::Zerg_Lurker_Egg )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN ATTACK UNIT --------------------------------------------
    static inline bool canAttackUnit(Game const &game, Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType().groundWeapon() == WeaponTypes::None && thisUnit->getType().airWeapon() == WeaponTypes::None )
      {
        if ( thisUnit->getType() == UnitTypes::Protoss_Carrier || thisUnit->getType() == UnitTypes::Hero_Gantrithor )
        {
          if ( thisUnit->getInterceptorCount() <= 0 )
            return game.setLastError(Errors::Unable_To_Hit);
        }
        else if ( thisUnit->getType() == UnitTypes::Protoss_Reaver || thisUnit->getType() == UnitTypes::Hero_Warbringer )
        {
          if ( thisUnit->getScarabCount() <= 0 )
            return game.setLastError(Errors::Unable_To_Hit);
        }
        else
          return game.setLastError(Errors::Unable_To_Hit);
      }
      if ( thisUnit->getType() == UnitTypes::Zerg_Lurker )
      {
        if ( !thisUnit->isBurrowed() )
          return game.setLastError(Errors::Unable_To_Hit);
      }
      else if ( thisUnit->isBurrowed() )
        return game.setLastError(Errors::Unable_To_Hit);
      if ( !thisUnit->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return game.setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canAttackUnit(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canAttackUnit(game, thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      if ( targetUnit->isInvincible() )
        return game.setLastError(Errors::Unable_To_Hit);

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
            game.setLastError(Errors::Unable_To_Hit);
          break;
        default:
          return game.setLastError(Errors::Unable_To_Hit);
        }
      }

      if ( !thisUnit->getType().canMove() && !thisUnit->isInWeaponRange(targetUnit) )
        return game.setLastError(Errors::Out_Of_Range);

      if ( thisUnit->getType() == UnitTypes::Zerg_Lurker && !thisUnit->isInWeaponRange(targetUnit) )
        return game.setLastError(Errors::Out_Of_Range);

      if ( targetUnit == thisUnit )
        return game.setLastError(Errors::Invalid_Parameter);

      return true;
    }
    static inline bool canAttackUnitGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);

      if ( !thisUnit->getType().canMove() &&
           thisUnit->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode )
        return game.setLastError(Errors::Incompatible_UnitType);

      if ( !thisUnit->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);

      if ( thisUnit->getType() == UnitTypes::Zerg_Lurker )
      {
        if ( !thisUnit->isBurrowed() )
          return game.setLastError(Errors::Unable_To_Hit);
      }
      else if ( thisUnit->isBurrowed() )
        return game.setLastError(Errors::Unable_To_Hit);

      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return game.setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canAttackUnitGrouped(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandTypeGrouped, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( checkCanIssueCommandTypeGrouped && !canAttackUnitGrouped(game, thisUnit, false, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      if ( targetUnit->isInvincible() )
        return game.setLastError(Errors::Unable_To_Hit);

      if ( thisUnit->getType() == UnitTypes::Zerg_Lurker && !thisUnit->isInWeaponRange(targetUnit) )
        return game.setLastError(Errors::Out_Of_Range);

      if ( thisUnit->getType() == UnitTypes::Zerg_Queen &&
           ( targetUnit->getType() != UnitTypes::Terran_Command_Center ||
             targetUnit->getHitPoints() >= 750 || targetUnit->getHitPoints() <= 0 ) )
        return game.setLastError(Errors::Invalid_Parameter);

      if ( targetUnit == thisUnit )
        return game.setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN BUILD --------------------------------------------------
    static inline bool canBuild(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->isConstructing() ||
           !thisUnit->isCompleted()   ||
           (thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canBuild(Game const &game, Unit thisUnit, UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canBuild(game, thisUnit, false) )
        return false;

      if ( !game.canMake(uType, thisUnit) )
        return false;

      if ( !uType.isBuilding() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->getAddon() )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canBuild(Game const &game, Unit thisUnit, UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canBuild(game, thisUnit, false) )
        return false;

      if ( checkTargetUnitType && !canBuild(game, thisUnit, uType, false, false) )
        return false;

      if ( game.isValid(tilePos) == false )
        return game.setLastError(Errors::Invalid_Tile_Position);

      if ( !game.canBuildHere(tilePos, uType, thisUnit, true) )
        return false;

      return true;
    }
    //------------------------------------------- CAN BUILD ADDON --------------------------------------------
    static inline bool canBuildAddon(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->isConstructing() ||
           !thisUnit->isCompleted()   ||
           thisUnit->isLifted() ||
           (thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->getAddon() )
        return game.setLastError(Errors::Incompatible_State);
      if ( !thisUnit->getType().canBuildAddon() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canBuildAddon(Game const &game, Unit thisUnit, UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canBuildAddon(game, thisUnit, false) )
        return false;

      if ( !game.canMake(uType, thisUnit) )
        return false;

      if ( !uType.isAddon() )
        return game.setLastError(Errors::Incompatible_UnitType);

      if ( !game.canBuildHere(thisUnit->getTilePosition(), uType, thisUnit) )
        return false;

      return true;
    }
    //------------------------------------------- CAN TRAIN --------------------------------------------------
    static inline bool canTrain(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->getType().producesLarva() )
      {
        if ( !thisUnit->isConstructing() && thisUnit->isCompleted() )
          return game.setLastError();
        Unitset larvae( thisUnit->getLarva() );
        for (Unit larva : larvae)
        {
          if ( !larva->isConstructing() && larva->isCompleted() && canCommand(game, larva) )
            return game.setLastError();
        }
        return game.setLastError(Errors::Unit_Busy);
      }

      if ( thisUnit->isConstructing() ||
           !thisUnit->isCompleted()   ||
           thisUnit->isLifted() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !thisUnit->getType().canProduce() &&
           thisUnit->getType() != UnitTypes::Enum::Terran_Nuclear_Silo &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Hydralisk &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Mutalisk &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Creep_Colony &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Spire &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Larva )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canTrain(Game const &game, Unit thisUnit, UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canTrain(game, thisUnit, false) )
        return false;

      if ( thisUnit->getType().producesLarva() )
      {
        if ( uType.whatBuilds().first == UnitTypes::Zerg_Larva )
        {
          bool foundCommandableLarva = false;
          Unitset larvae( thisUnit->getLarva() );
          for (Unit larva : larvae)
          {
            if ( canTrain(game, larva, true) )
            {
              foundCommandableLarva = true;
              thisUnit = larva;
              break;
            }
          }
          if (!foundCommandableLarva)
            return game.setLastError(Errors::Unit_Does_Not_Exist);
        }
        else if ( thisUnit->isConstructing() ||
                  !thisUnit->isCompleted() )
          return game.setLastError(Errors::Unit_Busy);
      }

      if ( !game.canMake(uType, thisUnit) )
        return false;

      if ( uType.isAddon() || ( uType.isBuilding() && !thisUnit->getType().isBuilding() ) )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( uType == UnitTypes::Enum::Zerg_Larva || uType == UnitTypes::Enum::Zerg_Egg || uType == UnitTypes::Enum::Zerg_Cocoon )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN MORPH --------------------------------------------------
    static inline bool canMorph(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->getType().producesLarva() )
      {
        if ( !thisUnit->isConstructing() && thisUnit->isCompleted() && ( !thisUnit->getType().isBuilding() || thisUnit->isIdle() ) )
          return game.setLastError();
        Unitset larvae( thisUnit->getLarva() );
        for (Unit larva : larvae)
        {
          if ( !larva->isConstructing() && larva->isCompleted() && canCommand(game, larva) )
            return game.setLastError();
        }
        return game.setLastError(Errors::Unit_Busy);
      }

      if ( thisUnit->isConstructing() ||
           !thisUnit->isCompleted()   ||
           (thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType() != UnitTypes::Enum::Zerg_Hydralisk &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Mutalisk &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Creep_Colony &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Spire &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Hatchery &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Lair &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Hive &&
           thisUnit->getType() != UnitTypes::Enum::Zerg_Larva )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canMorph(Game const &game, Unit thisUnit, UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canMorph(game, thisUnit, false) )
        return false;

      if ( thisUnit->getType().producesLarva() )
      {
        if ( uType.whatBuilds().first == UnitTypes::Zerg_Larva )
        {
          bool foundCommandableLarva = false;
          Unitset larvae( thisUnit->getLarva() );
          for (Unit larva : larvae)
          {
            if ( canMorph(game, larva, true) )
            {
              foundCommandableLarva = true;
              thisUnit = larva;
              break;
            }
          }
          if (!foundCommandableLarva)
            return game.setLastError(Errors::Unit_Does_Not_Exist);
        }
        else if ( thisUnit->isConstructing() ||
                  !thisUnit->isCompleted()   ||
                  (thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
          return game.setLastError(Errors::Unit_Busy);
      }

      if ( !game.canMake(uType, thisUnit) )
        return false;
      if ( uType == UnitTypes::Enum::Zerg_Larva || uType == UnitTypes::Enum::Zerg_Egg || uType == UnitTypes::Enum::Zerg_Cocoon )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN RESEARCH -----------------------------------------------
    static inline bool canResearch(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted() )
        return game.setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canResearch(Game const &game, Unit thisUnit, TechType type, bool checkCanIssueCommandType = true)
    {
      // Error checking
      if ( !game.self() )
        return game.setLastError(Errors::Unit_Not_Owned);

      if ( thisUnit )
      {
        if (thisUnit->getPlayer() != game.self())
          return game.setLastError(Errors::Unit_Not_Owned);

        if (!thisUnit->getType().isSuccessorOf(type.whatResearches()))
          return game.setLastError(Errors::Incompatible_UnitType);

        if ( checkCanIssueCommandType && ( thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted() ) )
          return game.setLastError(Errors::Unit_Busy);
      }
      if (game.self()->isResearching(type))
        return game.setLastError(Errors::Currently_Researching);

      if (game.self()->hasResearched(type))
        return game.setLastError(Errors::Already_Researched);

      if ( !game.self()->isResearchAvailable(type) )
        return game.setLastError(Errors::Access_Denied);

      if (game.self()->minerals() < type.mineralPrice())
        return game.setLastError(Errors::Insufficient_Minerals);

      if (game.self()->gas() < type.gasPrice())
        return game.setLastError(Errors::Insufficient_Gas);

      if (!game.self()->hasUnitTypeRequirement(type.requiredUnit()))
        return game.setLastError(Errors::Insufficient_Tech);

      return game.setLastError();
    }
    //------------------------------------------- CAN UPGRADE ------------------------------------------------
    static inline bool canUpgrade(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted() )
        return game.setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canUpgrade(Game const &game, Unit thisUnit, UpgradeType type, bool checkCanIssueCommandType = true)
    {
      Player self = game.self();
      if ( !self )
        return game.setLastError(Errors::Unit_Not_Owned);

      if ( thisUnit )
      {
        if (thisUnit->getPlayer() != self)
          return game.setLastError(Errors::Unit_Not_Owned);

        if (!thisUnit->getType().isSuccessorOf(type.whatUpgrades()))
          return game.setLastError(Errors::Incompatible_UnitType);

        if ( checkCanIssueCommandType && ( thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted() ) )
          return game.setLastError(Errors::Unit_Busy);
      }
      int nextLvl = self->getUpgradeLevel(type) + 1;
      
      if (!self->hasUnitTypeRequirement(type.whatUpgrades()))
        return game.setLastError(Errors::Unit_Does_Not_Exist);
      
      if (!self->hasUnitTypeRequirement(type.whatsRequired(nextLvl)))
        return game.setLastError(Errors::Insufficient_Tech);
      
      if (self->isUpgrading(type))
        return game.setLastError(Errors::Currently_Upgrading);

      if ( self->getUpgradeLevel(type) >= self->getMaxUpgradeLevel(type) )
        return game.setLastError(Errors::Fully_Upgraded);

      if ( self->minerals() < type.mineralPrice(nextLvl) )
        return game.setLastError(Errors::Insufficient_Minerals);

      if ( self->gas() < type.gasPrice(nextLvl) )
        return game.setLastError(Errors::Insufficient_Gas);

      return game.setLastError();
    }
    //------------------------------------------- CAN SET RALLY POINT ----------------------------------------
    static inline bool canSetRallyPoint(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canSetRallyPosition(game, thisUnit, false) && !canSetRallyUnit(game, thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canSetRallyPoint(Game const &game, Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (target == nullptr)
        return game.setLastError(Errors::Invalid_Parameter);
      if (!canSetRallyUnit(game, thisUnit, target, checkCanTargetUnit, checkCanIssueCommandType, false))
        return false;

      return true;
    }
    static inline bool canSetRallyPoint(Game const &game, Unit thisUnit, Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (!canSetRallyPosition(game, thisUnit, false))
        return false;

      return true;
    }
    //------------------------------------------- CAN SET RALLY POSITION -------------------------------------
    static inline bool canSetRallyPosition(Game const &game, Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().canProduce() || !thisUnit->getType().isBuilding() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isLifted() )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN SET RALLY UNIT -----------------------------------------
    static inline bool canSetRallyUnit(Game const &game, Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().canProduce() || !thisUnit->getType().isBuilding() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isLifted() )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canSetRallyUnit(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canSetRallyUnit(game, thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN MOVE ---------------------------------------------------
    static inline bool canMove(Game const &game, Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() )
      {
        if ( !thisUnit->isInterruptible() )
          return game.setLastError(Errors::Unit_Busy);
        if ( !thisUnit->getType().canMove() )
          return game.setLastError(Errors::Incompatible_UnitType);
        if ( thisUnit->isBurrowed() )
          return game.setLastError(Errors::Incompatible_State);
        if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
          return game.setLastError(Errors::Unit_Busy);
        if ( thisUnit->getType() == UnitTypes::Zerg_Larva )
          return game.setLastError(Errors::Incompatible_UnitType);
      }
      else
      {
        if ( !thisUnit->getType().isFlyingBuilding() )
          return game.setLastError(Errors::Incompatible_UnitType);
        if ( !thisUnit->isLifted() )
          return game.setLastError(Errors::Incompatible_State);
      }

      if (!thisUnit->isCompleted())
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canMoveGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( !thisUnit->getType().canMove() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isCompleted() && !thisUnit->isMorphing() )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN PATROL -------------------------------------------------
    static inline bool canPatrol(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canMove(game, thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canPatrolGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( !canMoveGrouped(game, thisUnit, false, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN FOLLOW -------------------------------------------------
    static inline bool canFollow(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canMove(game, thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canFollow(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canFollow(game, thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      if ( targetUnit == thisUnit )
        return game.setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN GATHER -------------------------------------------------
    static inline bool canGather(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !thisUnit->getType().isWorker() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->getPowerUp() )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isBurrowed() )
        return game.setLastError(Errors::Incompatible_State);
      if ( !thisUnit->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return game.setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canGather(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canGather(game, thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      UnitType uType = targetUnit->getType();
      if ( !uType.isResourceContainer() || uType == UnitTypes::Resource_Vespene_Geyser )
        return game.setLastError(Errors::Incompatible_UnitType);

      if ( !targetUnit->isCompleted() )
        return game.setLastError(Errors::Unit_Busy);

      if ( !thisUnit->hasPath(targetUnit->getPosition()) )
        return game.setLastError(Errors::Unreachable_Location);

      if ( uType.isRefinery() && targetUnit->getPlayer() != game.self() )
        return game.setLastError(Errors::Unit_Not_Owned);

      return true;
    }
    //------------------------------------------- CAN RETURN CARGO -------------------------------------------
    static inline bool canReturnCargo(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !thisUnit->getType().isWorker() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isCarryingGas() && !thisUnit->isCarryingMinerals() )
        return game.setLastError(Errors::Insufficient_Ammo);
      if ( thisUnit->isBurrowed() )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return game.setLastError(Errors::Unit_Busy);

      return true;
    }
    //------------------------------------------- CAN HOLD POSITION ------------------------------------------
    static inline bool canHoldPosition(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() )
      {
        if ( !thisUnit->getType().canMove() )
          return game.setLastError(Errors::Incompatible_UnitType);
        if ( thisUnit->isBurrowed() && thisUnit->getType() != UnitTypes::Zerg_Lurker )
          return game.setLastError(Errors::Incompatible_State);
        if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
          return game.setLastError(Errors::Unit_Busy);
        if ( thisUnit->getType() == UnitTypes::Zerg_Larva )
          return game.setLastError(Errors::Incompatible_UnitType);
      }
      else
      {
        if ( !thisUnit->getType().isFlyingBuilding() )
          return game.setLastError(Errors::Incompatible_UnitType);
        if ( !thisUnit->isLifted() )
          return game.setLastError(Errors::Incompatible_State);
      }

      if (!thisUnit->isCompleted())
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN STOP ---------------------------------------------------
    static inline bool canStop(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->isBurrowed() && thisUnit->getType() != UnitTypes::Zerg_Lurker )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->getType().isBuilding() && !thisUnit->isLifted() &&
           thisUnit->getType() != UnitTypes::Protoss_Photon_Cannon &&
           thisUnit->getType() != UnitTypes::Zerg_Sunken_Colony &&
           thisUnit->getType() != UnitTypes::Zerg_Spore_Colony &&
           thisUnit->getType() != UnitTypes::Terran_Missile_Turret )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN REPAIR -------------------------------------------------
    static inline bool canRepair(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType() != BWAPI::UnitTypes::Terran_SCV )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return game.setLastError(Errors::Unit_Busy);

      return true;
    }
    static inline bool canRepair(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canRepair(game, thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      UnitType targType = targetUnit->getType();
      if ( targType.getRace() != BWAPI::Races::Terran || !targType.isMechanical() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( targetUnit->getHitPoints() == targType.maxHitPoints() )
        return game.setLastError(Errors::Incompatible_State);
      if ( !targetUnit->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( targetUnit == thisUnit )
        return game.setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN BURROW -------------------------------------------------
    static inline bool canBurrow(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canUseTechWithoutTarget(game, thisUnit, BWAPI::TechTypes::Burrowing, true, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN UNBURROW -----------------------------------------------
    static inline bool canUnburrow(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBurrowable() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Unburrowing )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CLOAK --------------------------------------------------
    static inline bool canCloak(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canUseTechWithoutTarget(game, thisUnit, thisUnit->getType().cloakingTech(), true, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN DECLOAK ------------------------------------------------
    static inline bool canDecloak(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->getType().cloakingTech() == TechTypes::None )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->getSecondaryOrder() != Orders::Cloak )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN SIEGE --------------------------------------------------
    static inline bool canSiege(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canUseTechWithoutTarget(game, thisUnit, BWAPI::TechTypes::Tank_Siege_Mode, true, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN UNSIEGE ------------------------------------------------
    static inline bool canUnsiege(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->isSieged() )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN LIFT ---------------------------------------------------
    static inline bool canLift(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isFlyingBuilding() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( thisUnit->isLifted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( !thisUnit->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( !thisUnit->isIdle() )
        return game.setLastError(Errors::Unit_Busy);

      return true;
    }
    //------------------------------------------- CAN LAND ---------------------------------------------------
    static inline bool canLand(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isFlyingBuilding() )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( !thisUnit->isLifted() )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canLand(Game const &game, Unit thisUnit, TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if (checkCanIssueCommandType && !canLand(game, thisUnit, checkCommandibility) )
        return false;

      if ( !canBuildHere(game, nullptr, target, thisUnit->getType(), true) )
        return false;

      return true;
    }
    //------------------------------------------- CAN LOAD ---------------------------------------------------
    static inline bool canLoad(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !thisUnit->isCompleted() )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType() == UnitTypes::Zerg_Overlord && game.self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0 )
        return game.setLastError(Errors::Insufficient_Tech);
      if ( thisUnit->isBurrowed() )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->getOrder() == Orders::ConstructingBuilding )
        return game.setLastError(Errors::Unit_Busy);
      if ( thisUnit->getType() == UnitTypes::Zerg_Larva )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canLoad(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canLoad(game, thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      //target must also be owned by self
      if (targetUnit->getPlayer() != game.self())
        return game.setLastError(Errors::Unit_Not_Owned);

      if ( targetUnit->isLoaded() || !targetUnit->isCompleted() )
        return game.setLastError(Errors::Unit_Busy);

      // verify upgrade for Zerg Overlord
      if ( targetUnit->getType() == UnitTypes::Zerg_Overlord && game.self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0 )
        return game.setLastError(Errors::Insufficient_Tech);

      int thisUnitSpaceProvided = thisUnit->getType().spaceProvided();
      int targetSpaceProvided = targetUnit->getType().spaceProvided();
      if ( thisUnitSpaceProvided <= 0 && targetSpaceProvided <= 0 )
        return game.setLastError(Errors::Incompatible_UnitType);

      const BWAPI::Unit unitToBeLoaded = ( thisUnitSpaceProvided > 0 ? targetUnit : thisUnit );
      if ( unitToBeLoaded->getType().canMove() == false || unitToBeLoaded->getType().isFlyer() || unitToBeLoaded->getType().spaceRequired() > 8 )
        return game.setLastError(Errors::Incompatible_UnitType);
      if ( !unitToBeLoaded->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( unitToBeLoaded->isBurrowed() )
        return game.setLastError(Errors::Incompatible_State);

      const BWAPI::Unit unitThatLoads = ( thisUnitSpaceProvided > 0 ? thisUnit : targetUnit );
      if ( unitThatLoads->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);

      if ( unitThatLoads->getType() == UnitTypes::Terran_Bunker )
      {
        if ( !unitToBeLoaded->getType().isOrganic() || unitToBeLoaded->getType().getRace() != Races::Terran )
          return game.setLastError(Errors::Incompatible_UnitType);
        if ( !unitToBeLoaded->hasPath(unitThatLoads->getPosition()) )
          return game.setLastError(Errors::Unreachable_Location);
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
        return game.setLastError(Errors::Insufficient_Space);

      return true;
    }
    //------------------------------------------- CAN UNLOAD -------------------------------------------------
    static inline bool canUnloadWithOrWithoutTarget(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);

      if ( thisUnit->getLoadedUnits().size() == 0 )
        return game.setLastError(Errors::Unit_Does_Not_Exist);

      // Check overlord tech
      if ( thisUnit->getType() == UnitTypes::Zerg_Overlord && game.self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
        return game.setLastError(Errors::Insufficient_Tech);

      if ( thisUnit->getType().spaceProvided() <= 0 )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canUnloadAtPosition(Game const &game, Unit thisUnit, Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUnloadWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      if ( thisUnit->getType() != UnitTypes::Terran_Bunker )
      {
        if (!game.isValid(WalkPosition(targDropPos.x/8, targDropPos.y/8)))
          return game.setLastError(Errors::Invalid_Tile_Position);
        else if ( !game.isWalkable(targDropPos.x/8, targDropPos.y/8) )
          return game.setLastError(Errors::Unreachable_Location);
      }

      return true;
    }
    static inline bool canUnload(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      return canUnloadAtPosition(game, thisUnit, thisUnit->getPosition(), true, checkCommandibility);
    }
    static inline bool canUnload(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUnloadWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      if ( checkPosition && !canUnloadAtPosition(game, thisUnit, thisUnit->getPosition(), false, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      if ( !targetUnit->isLoaded() )
        return game.setLastError(Errors::Incompatible_State);

      if ( targetUnit->getTransport() != thisUnit )
        return game.setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN UNLOAD ALL ---------------------------------------------
    static inline bool canUnloadAll(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      return canUnloadAtPosition(game, thisUnit, thisUnit->getPosition(), true, checkCommandibility);
    }
    //------------------------------------------- CAN UNLOAD ALL POSITION ------------------------------------
    static inline bool canUnloadAllPosition(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canUnloadWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      if ( thisUnit->getType() == UnitTypes::Terran_Bunker )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canUnloadAllPosition(Game const &game, Unit thisUnit, Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUnloadAllPosition(game, thisUnit, false) )
        return false;

      if ( !canUnloadAtPosition(game, thisUnit, targDropPos, false, false) )
        return false;

      return true;
    }
    //------------------------------------------- CAN RIGHT CLICK --------------------------------------------
    static inline bool canRightClick(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !canRightClickPosition(game, thisUnit, false) && !canRightClickUnit(game, thisUnit, false) )
        return false;

      return true;
    }
    static inline bool canRightClick(Game const &game, Unit thisUnit, Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (!canRightClickPosition(game, thisUnit, false))
        return false;

      return true;
    }
    static inline bool canRightClick(Game const &game, Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (target == nullptr)
        return game.setLastError(Errors::Invalid_Parameter);
      if (!canRightClickUnit(game, thisUnit, target, checkCanTargetUnit, checkCanIssueCommandType, false))
        return false;

      return true;
    }
    static inline bool canRightClickGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( !canRightClickPositionGrouped(game, thisUnit, false, false) && !canRightClickUnitGrouped(game, thisUnit, false, false) )
        return false;

      return true;
    }
    static inline bool canRightClickGrouped(Game const &game, Unit thisUnit, Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false))
        return false;

      if (!canRightClickPositionGrouped(game, thisUnit, false, false))
        return false;

      return true;
    }
    static inline bool canRightClickGrouped(Game const &game, Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false))
        return false;

      if (target == nullptr)
        return game.setLastError(Errors::Invalid_Parameter);
      if (!canRightClickUnitGrouped(game, thisUnit, target, checkCanTargetUnit, checkCanIssueCommandTypeGrouped, false, false))
        return false;

      return true;
    }
    //------------------------------------------- CAN RIGHT CLICK POSITION -----------------------------------
    static inline bool canRightClickPosition(Game const &game, Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !canMove(game, thisUnit, false) && !canSetRallyPosition(game, thisUnit, false) )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canRightClickPositionGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !canMoveGrouped(game, thisUnit, false, false) )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN RIGHT CLICK UNIT ---------------------------------------
    static inline bool canRightClickUnit(Game const &game, Unit thisUnit, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !canFollow(game, thisUnit, false) &&
           !canAttackUnit(game, thisUnit, false) &&
           !canLoad(game, thisUnit, false) &&
           !canSetRallyUnit(game, thisUnit, false) )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canRightClickUnit(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canRightClickUnit(game, thisUnit, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      if ( !targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()) &&
           !canAttackUnit(game, thisUnit, targetUnit, false, true, false) )
        return false;

      if ( !canFollow(game, thisUnit, targetUnit, false, true, false) &&
           !canLoad(game, thisUnit, targetUnit, false, true, false) &&
           !canSetRallyUnit(game, thisUnit, targetUnit, false, true, false) )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canRightClickUnitGrouped(Game const &game, Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !canFollow(game, thisUnit, false) &&
           !canAttackUnitGrouped(game, thisUnit, false, false) &&
           !canLoad(game, thisUnit, false) )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canRightClickUnitGrouped(Game const &game, Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      if ( checkCanIssueCommandType && !canRightClickUnitGrouped(game, thisUnit, false, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      if ( !targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()) &&
           !canAttackUnitGrouped(game, thisUnit, targetUnit, false, true, false, false) )
        return false;

      if ( !canFollow(game, thisUnit, targetUnit, false, true, false) &&
           !canLoad(game, thisUnit, targetUnit, false, true, false) )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN HALT CONSTRUCTION --------------------------------------
    static inline bool canHaltConstruction(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->getOrder() != Orders::ConstructingBuilding )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL CONSTRUCTION ------------------------------------
    static inline bool canCancelConstruction(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() )
        return game.setLastError(Errors::Incompatible_UnitType);

      if ( thisUnit->isCompleted() || (thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit()) )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL ADDON -------------------------------------------
    static inline bool canCancelAddon(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getAddon() || thisUnit->getAddon()->isCompleted() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN CANCEL TRAIN -------------------------------------------
    static inline bool canCancelTrain(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->isTraining() )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL TRAIN SLOT --------------------------------------
    static inline bool canCancelTrainSlot(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      return canCancelTrain(game, thisUnit, checkCommandibility);
    }
    static inline bool canCancelTrainSlot(Game const &game, Unit thisUnit, int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canCancelTrainSlot(game, thisUnit, false) )
        return false;

      if ( !thisUnit->isTraining() || (thisUnit->getTrainingQueue().size() <= (unsigned int)slot && slot >= 0) )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL MORPH -------------------------------------------
    static inline bool canCancelMorph(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->isMorphing() || (!thisUnit->isCompleted() && thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit()) )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    //------------------------------------------- CAN CANCEL RESEARCH ----------------------------------------
    static inline bool canCancelResearch(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->getOrder() != Orders::ResearchTech )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN CANCEL UPGRADE -----------------------------------------
    static inline bool canCancelUpgrade(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( thisUnit->getOrder() != Orders::Upgrade )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    //------------------------------------------- CAN USE TECH -----------------------------------------------
    static inline bool canUseTechWithOrWithoutTarget(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isBuilding() && !thisUnit->isInterruptible() )
        return game.setLastError(Errors::Unit_Busy);
      if ( !thisUnit->isCompleted() )
        return game.setLastError(Errors::Incompatible_State);
      if ( thisUnit->isHallucination() )
        return game.setLastError(Errors::Incompatible_UnitType);

      return true;
    }
    static inline bool canUseTechWithOrWithoutTarget(Game const &game, Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      // researched check
      if ( !thisUnit->getType().isHero() && !game.self()->hasResearched(tech) && thisUnit->getType() != UnitTypes::Zerg_Lurker )
        return game.setLastError(Errors::Insufficient_Tech);

      // energy check
      if ( thisUnit->getEnergy() < tech.energyCost() )
        return game.setLastError(Errors::Insufficient_Energy);

      // unit check
      if ( tech != TechTypes::Burrowing && !tech.whatUses().contains(thisUnit->getType()) )
        return game.setLastError(Errors::Incompatible_UnitType);

      switch (tech)
      {
        case TechTypes::Enum::Spider_Mines:
          if ( thisUnit->getSpiderMineCount() <= 0 )
            return game.setLastError(Errors::Insufficient_Ammo);
          return true;

        case TechTypes::Enum::Tank_Siege_Mode:
          if ( thisUnit->isSieged() )
            return game.setLastError(Errors::Incompatible_State);
          if ( thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging )
            return game.setLastError(Errors::Unit_Busy);
          return true;

        case TechTypes::Enum::Cloaking_Field:
        case TechTypes::Enum::Personnel_Cloaking:
          if ( thisUnit->getSecondaryOrder() == Orders::Cloak )
            return game.setLastError(Errors::Incompatible_State);
          return true;

        case TechTypes::Enum::Burrowing:
          if ( !thisUnit->getType().isBurrowable() )
            return game.setLastError(Errors::Incompatible_UnitType);
          if ( thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Burrowing || thisUnit->getOrder() == Orders::Unburrowing )
            return game.setLastError(Errors::Incompatible_State);
          return true;

        case TechTypes::Enum::None:
          return game.setLastError(Errors::Incompatible_TechType);

        case TechTypes::Enum::Nuclear_Strike:
          if ( thisUnit->getPlayer()->completedUnitCount(UnitTypes::Terran_Nuclear_Missile) == 0 )
            return game.setLastError(Errors::Insufficient_Ammo);
          return true;

        case TechTypes::Enum::Unknown:
          return game.setLastError(Errors::Incompatible_TechType);
      }

      return true;
    }
    static inline bool canUseTech(Game const &game, Unit thisUnit, BWAPI::TechType tech, Position target, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (!canUseTechPosition(game, thisUnit, tech, target, checkTargetsType, checkCanIssueCommandType, false))
        return false;

      return true;
    }
    static inline bool canUseTech(Game const &game, Unit thisUnit, BWAPI::TechType tech, Unit target = nullptr, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if (!checkCommandibility)
        game.setLastError();
      else if (!canCommand(game, thisUnit))
        return false;

      if (target == nullptr)
      {
        if (!canUseTechWithoutTarget(game, thisUnit, tech, checkCanIssueCommandType, false))
          return false;
      }
      else
      {
        if (!canUseTechUnit(game, thisUnit, tech, target, checkCanTargetUnit, checkTargetsType, checkCanIssueCommandType, false))
          return false;
      }

      return true;
    }
    static inline bool canUseTechWithoutTarget(Game const &game, Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      if ( !canUseTechWithOrWithoutTarget(game, thisUnit, tech, false, false) )
        return false;
      if ( tech.targetsUnit() || tech.targetsPosition() || tech == TechTypes::None || tech == TechTypes::Unknown || tech == TechTypes::Lurker_Aspect)
        return game.setLastError(Errors::Incompatible_TechType);

      return true;
    }
    //------------------------------------------- CAN USE TECH UNIT ------------------------------------------
    static inline bool canUseTechUnit(Game const &game, Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      if ( !canUseTechWithOrWithoutTarget(game, thisUnit, tech, false, false) )
        return false;
      if ( !tech.targetsUnit() )
        return game.setLastError(Errors::Incompatible_TechType);

      return true;
    }
    static inline bool canUseTechUnit(Game const &game, Unit thisUnit, BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit, bool checkTargetsUnits, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      if ( checkTargetsUnits && !canUseTechUnit(game, thisUnit, tech, false, false) )
        return false;

      if ( checkCanTargetUnit && !canTargetUnit(game, thisUnit, targetUnit, false) )
        return false;

      UnitType targetType = targetUnit->getType();

      switch (tech)
      {
        case TechTypes::Enum::Archon_Warp:
          if ( targetType != UnitTypes::Protoss_High_Templar )
            return game.setLastError(Errors::Incompatible_UnitType);
          if ( targetUnit->getPlayer() != thisUnit->getPlayer() )
            return game.setLastError(Errors::Unit_Not_Owned);
          break;

        case TechTypes::Enum::Dark_Archon_Meld:
          if ( targetType != UnitTypes::Protoss_Dark_Templar )
            return game.setLastError(Errors::Incompatible_UnitType);
          if ( targetUnit->getPlayer() != thisUnit->getPlayer() )
            return game.setLastError(Errors::Unit_Not_Owned);
          break;

        case TechTypes::Enum::Consume:
          if ( targetUnit->getPlayer() != thisUnit->getPlayer() )
            return game.setLastError(Errors::Unit_Not_Owned);
          if ( targetType.getRace() != Races::Zerg || targetType == UnitTypes::Zerg_Larva )
            return game.setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Spawn_Broodlings:
          if ( ( !targetType.isOrganic() && !targetType.isMechanical() ) ||
               targetType.isRobotic() ||
               targetType.isFlyer() )
            return game.setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Lockdown:
          if ( !targetType.isMechanical() )
            return game.setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Healing:
          if ( targetUnit->getHitPoints() == targetType.maxHitPoints() )
            return game.setLastError(Errors::Incompatible_State);
          if ( !targetType.isOrganic() ||
               targetType.isFlyer() )
            return game.setLastError(Errors::Incompatible_UnitType);
          if ( !targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()) )
            return game.setLastError(Errors::Invalid_Parameter);
          break;

        case TechTypes::Enum::Mind_Control:
          if ( targetUnit->getPlayer() == thisUnit->getPlayer() )
            return game.setLastError(Errors::Invalid_Parameter);
          if ( targetType == UnitTypes::Protoss_Interceptor ||
               targetType == UnitTypes::Terran_Vulture_Spider_Mine ||
               targetType == UnitTypes::Zerg_Lurker_Egg ||
               targetType == UnitTypes::Zerg_Cocoon ||
               targetType == UnitTypes::Zerg_Larva ||
               targetType == UnitTypes::Zerg_Egg )
            return game.setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Feedback:
          if ( !targetType.isSpellcaster() )
            return game.setLastError(Errors::Incompatible_UnitType);
          break;

        case TechTypes::Enum::Infestation:
          if ( targetType != UnitTypes::Terran_Command_Center ||
               targetUnit->getHitPoints() >= 750 || targetUnit->getHitPoints() <= 0 )
            return game.setLastError(Errors::Invalid_Parameter);
          break;
      }

      switch (tech)
      {
        case TechTypes::Enum::Archon_Warp:
        case TechTypes::Enum::Dark_Archon_Meld:
          if ( !thisUnit->hasPath(targetUnit->getPosition()) )
            return game.setLastError(Errors::Unreachable_Location);
          if ( targetUnit->isHallucination() )
            return game.setLastError(Errors::Invalid_Parameter);
          if ( targetUnit->isMaelstrommed() )
            return game.setLastError(Errors::Incompatible_State);
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
            return game.setLastError(Errors::Incompatible_State);
          break;
      }

      switch (tech)
      {
        case TechTypes::Enum::Yamato_Gun:
          if ( targetUnit->isInvincible() )
            return game.setLastError(Errors::Invalid_Parameter);
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
            return game.setLastError(Errors::Invalid_Parameter);
          // Fall through (don't break).
        case TechTypes::Enum::Consume:
        case TechTypes::Enum::Feedback:
          if ( targetType.isBuilding() )
            return game.setLastError(Errors::Incompatible_UnitType);
          break;
      }

      if ( targetUnit == thisUnit )
        return game.setLastError(Errors::Invalid_Parameter);

      return true;
    }
    //------------------------------------------- CAN USE TECH POSITION --------------------------------------
    static inline bool canUseTechPosition(Game const &game, Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      if ( !canUseTechWithOrWithoutTarget(game, thisUnit, tech, false, false) )
        return false;
      if ( !tech.targetsPosition() )
        return game.setLastError(Errors::Incompatible_TechType);

      return true;
    }
    static inline bool canUseTechPosition(Game const &game, Unit thisUnit, BWAPI::TechType tech, Position target, bool checkTargetsPositions, bool checkCanIssueCommandType, bool checkCommandibility)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(game, thisUnit, false) )
        return false;

      if ( checkTargetsPositions && !canUseTechPosition(game, thisUnit, tech, false, false) )
        return false;

      if ( tech == TechTypes::Enum::Spider_Mines && !thisUnit->hasPath(target) )
        return game.setLastError(Errors::Unreachable_Location);

      return true;
    }
    //------------------------------------------- CAN PLACE COP ----------------------------------------------
    static inline bool canPlaceCOP(Game const &game, Unit thisUnit, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( !thisUnit->getType().isFlagBeacon() )
        return game.setLastError(Errors::Incompatible_UnitType);

      if ( /*@TODO: Put real condition here ||*/ thisUnit->getOrder() != BWAPI::Orders::CTFCOPInit )
        return game.setLastError(Errors::Incompatible_State);

      return true;
    }
    static inline bool canPlaceCOP(Game const &game, Unit thisUnit, TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if (checkCanIssueCommandType && !canPlaceCOP(game, thisUnit, checkCommandibility) )
        return false;

      if ( !canBuildHere(game, thisUnit, target, thisUnit->getType(), true) )
        return false;

      return true;
    }
    //------------------------------------------- CAN ISSUE COMMAND TYPE -------------------------------------
    static inline bool canIssueCommandType(Game const &game, Unit thisUnit, BWAPI::UnitCommandType ct, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      switch (ct)
      {
        case UnitCommandTypes::Enum::Attack_Move:
          return canAttackMove(game, thisUnit, false);

        case UnitCommandTypes::Enum::Attack_Unit:
          return canAttackUnit(game, thisUnit, false);

        case UnitCommandTypes::Enum::Build:
          return canBuild(game, thisUnit, false);

        case UnitCommandTypes::Enum::Build_Addon:
          return canBuildAddon(game, thisUnit, false);

        case UnitCommandTypes::Enum::Train:
          return canTrain(game, thisUnit, false);

        case UnitCommandTypes::Enum::Morph:
          return canMorph(game, thisUnit, false);

        case UnitCommandTypes::Enum::Research:
          return canResearch(game, thisUnit, false);

        case UnitCommandTypes::Enum::Upgrade:
          return canUpgrade(game, thisUnit, false);

        case UnitCommandTypes::Enum::Set_Rally_Position:
          return canSetRallyPosition(game, thisUnit, false);

        case UnitCommandTypes::Enum::Set_Rally_Unit:
          return canSetRallyUnit(game, thisUnit, false);

        case UnitCommandTypes::Enum::Move:
          return canMove(game, thisUnit, false);

        case UnitCommandTypes::Enum::Patrol:
          return canPatrol(game, thisUnit, false);

        case UnitCommandTypes::Enum::Hold_Position:
          return canHoldPosition(game, thisUnit, false);

        case UnitCommandTypes::Enum::Stop:
          return canStop(game, thisUnit, false);

        case UnitCommandTypes::Enum::Follow:
          return canFollow(game, thisUnit, false);

        case UnitCommandTypes::Enum::Gather:
          return canGather(game, thisUnit, false);

        case UnitCommandTypes::Enum::Return_Cargo:
          return canReturnCargo(game, thisUnit, false);

        case UnitCommandTypes::Enum::Repair:
          return canRepair(game, thisUnit, false);

        case UnitCommandTypes::Enum::Burrow:
          return canBurrow(game, thisUnit, false);

        case UnitCommandTypes::Enum::Unburrow:
          return canUnburrow(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cloak:
          return canCloak(game, thisUnit, false);

        case UnitCommandTypes::Enum::Decloak:
          return canDecloak(game, thisUnit, false);

        case UnitCommandTypes::Enum::Siege:
          return canSiege(game, thisUnit, false);

        case UnitCommandTypes::Enum::Unsiege:
          return canUnsiege(game, thisUnit, false);

        case UnitCommandTypes::Enum::Lift:
          return canLift(game, thisUnit, false);

        case UnitCommandTypes::Enum::Land:
          return canLand(game, thisUnit, false);

        case UnitCommandTypes::Enum::Load:
          return canLoad(game, thisUnit, false);

        case UnitCommandTypes::Enum::Unload:
          return canUnload(game, thisUnit, false);

        case UnitCommandTypes::Enum::Unload_All:
          return canUnloadAll(game, thisUnit, false);

        case UnitCommandTypes::Enum::Unload_All_Position:
          return canUnloadAllPosition(game, thisUnit, false);

        case UnitCommandTypes::Enum::Right_Click_Position:
          return canRightClickPosition(game, thisUnit, false);

        case UnitCommandTypes::Enum::Right_Click_Unit:
          return canRightClickUnit(game, thisUnit, false);

        case UnitCommandTypes::Enum::Halt_Construction:
          return canHaltConstruction(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Construction:
          return canCancelConstruction(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Addon:
          return canCancelAddon(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Train:
          return canCancelTrain(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Train_Slot:
          return canCancelTrainSlot(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Morph:
          return canCancelMorph(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Research:
          return canCancelResearch(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Upgrade:
          return canCancelUpgrade(game, thisUnit, false);

        case UnitCommandTypes::Enum::Use_Tech:
        case UnitCommandTypes::Enum::Use_Tech_Unit:
        case UnitCommandTypes::Enum::Use_Tech_Position:
          return canUseTechWithOrWithoutTarget(game, thisUnit, false);

        case UnitCommandTypes::Enum::Place_COP:
          return canPlaceCOP(game, thisUnit, false);
      }

      return true;
    }
    static inline bool canIssueCommandTypeGrouped(Game const &game, Unit thisUnit, BWAPI::UnitCommandType ct, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      switch (ct)
      {
        case UnitCommandTypes::Enum::Attack_Move:
          return canAttackMoveGrouped(game, thisUnit, false, false);

        case UnitCommandTypes::Enum::Attack_Unit:
          return canAttackUnitGrouped(game, thisUnit, false, false);

        case UnitCommandTypes::Enum::Build:
          return false;

        case UnitCommandTypes::Enum::Build_Addon:
          return false;

        case UnitCommandTypes::Enum::Train:
          return canTrain(game, thisUnit, false);

        case UnitCommandTypes::Enum::Morph:
          return canMorph(game, thisUnit, false);

        case UnitCommandTypes::Enum::Research:
          return false;

        case UnitCommandTypes::Enum::Upgrade:
          return false;

        case UnitCommandTypes::Enum::Set_Rally_Position:
          return false;

        case UnitCommandTypes::Enum::Set_Rally_Unit:
          return false;

        case UnitCommandTypes::Enum::Move:
          return canMoveGrouped(game, thisUnit, false, false);

        case UnitCommandTypes::Enum::Patrol:
          return canPatrolGrouped(game, thisUnit, false, false);

        case UnitCommandTypes::Enum::Hold_Position:
          return canHoldPosition(game, thisUnit, false);

        case UnitCommandTypes::Enum::Stop:
          return canStop(game, thisUnit, false);

        case UnitCommandTypes::Enum::Follow:
          return canFollow(game, thisUnit, false);

        case UnitCommandTypes::Enum::Gather:
          return canGather(game, thisUnit, false);

        case UnitCommandTypes::Enum::Return_Cargo:
          return canReturnCargo(game, thisUnit, false);

        case UnitCommandTypes::Enum::Repair:
          return canRepair(game, thisUnit, false);

        case UnitCommandTypes::Enum::Burrow:
          return canBurrow(game, thisUnit, false);

        case UnitCommandTypes::Enum::Unburrow:
          return canUnburrow(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cloak:
          return canCloak(game, thisUnit, false);

        case UnitCommandTypes::Enum::Decloak:
          return canDecloak(game, thisUnit, false);

        case UnitCommandTypes::Enum::Siege:
          return canSiege(game, thisUnit, false);

        case UnitCommandTypes::Enum::Unsiege:
          return canUnsiege(game, thisUnit, false);

        case UnitCommandTypes::Enum::Lift:
          return false;

        case UnitCommandTypes::Enum::Land:
          return false;

        case UnitCommandTypes::Enum::Load:
          return canLoad(game, thisUnit, false);

        case UnitCommandTypes::Enum::Unload:
          return false;

        case UnitCommandTypes::Enum::Unload_All:
          return false;

        case UnitCommandTypes::Enum::Unload_All_Position:
          return canUnloadAllPosition(game, thisUnit, false);

        case UnitCommandTypes::Enum::Right_Click_Position:
          return canRightClickPositionGrouped(game, thisUnit, false, false);

        case UnitCommandTypes::Enum::Right_Click_Unit:
          return canRightClickUnitGrouped(game, thisUnit, false, false);

        case UnitCommandTypes::Enum::Halt_Construction:
          return canHaltConstruction(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Construction:
          return false;

        case UnitCommandTypes::Enum::Cancel_Addon:
          return false;

        case UnitCommandTypes::Enum::Cancel_Train:
          return false;

        case UnitCommandTypes::Enum::Cancel_Train_Slot:
          return false;

        case UnitCommandTypes::Enum::Cancel_Morph:
          return canCancelMorph(game, thisUnit, false);

        case UnitCommandTypes::Enum::Cancel_Research:
          return false;

        case UnitCommandTypes::Enum::Cancel_Upgrade:
          return false;

        case UnitCommandTypes::Enum::Use_Tech:
        case UnitCommandTypes::Enum::Use_Tech_Unit:
        case UnitCommandTypes::Enum::Use_Tech_Position:
          return canUseTechWithOrWithoutTarget(game, thisUnit, false);

        case UnitCommandTypes::Enum::Place_COP:
          return false;
      }

      return true;
    }
    //------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
    static inline bool canIssueCommand(Game const &game, Unit thisUnit, UnitCommand c, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      BWAPI::UnitCommandType ct = c.type;
      if ( checkCanIssueCommandType && !canIssueCommandType(game, thisUnit, ct, false) )
        return false;

      switch (ct)
      {
        case UnitCommandTypes::Enum::Attack_Move:
          return true;

        case UnitCommandTypes::Enum::Attack_Unit:
          return canAttackUnit(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Build:
          return canBuild(game, thisUnit, c.getUnitType(), BWAPI::TilePosition(c.x, c.y), checkCanBuildUnitType, false, false);

        case UnitCommandTypes::Enum::Build_Addon:
          return canBuildAddon(game, thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Train:
          return canTrain(game, thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Morph:
          return canMorph(game, thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Research:
          return game.canResearch(c.getTechType(), thisUnit, false);

        case UnitCommandTypes::Enum::Upgrade:
          return game.canUpgrade(c.getUpgradeType(), thisUnit, false);

        case UnitCommandTypes::Enum::Set_Rally_Position:
          return true;

        case UnitCommandTypes::Enum::Set_Rally_Unit:
          return canSetRallyUnit(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Move:
          return true;

        case UnitCommandTypes::Enum::Patrol:
          return true;

        case UnitCommandTypes::Enum::Hold_Position:
          return true;

        case UnitCommandTypes::Enum::Stop:
          return true;

        case UnitCommandTypes::Enum::Follow:
          return canFollow(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Gather:
          return canGather(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Return_Cargo:
          return true;

        case UnitCommandTypes::Enum::Repair:
          return canRepair(game, thisUnit, c.target, checkCanTargetUnit, false, false);

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
          return canLand(game, thisUnit, BWAPI::TilePosition(c.x, c.y), false, false);

        case UnitCommandTypes::Enum::Load:
          return canLoad(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Unload:
          return canUnload(game, thisUnit, c.target, checkCanTargetUnit, false, false, false);

        case UnitCommandTypes::Enum::Unload_All:
          return true;

        case UnitCommandTypes::Enum::Unload_All_Position:
          return canUnloadAllPosition(game, thisUnit, c.getTargetPosition(), false, false);

        case UnitCommandTypes::Enum::Right_Click_Position:
          return true;

        case UnitCommandTypes::Enum::Right_Click_Unit:
          return canRightClickUnit(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Halt_Construction:
          return true;

        case UnitCommandTypes::Enum::Cancel_Construction:
          return true;

        case UnitCommandTypes::Enum::Cancel_Addon:
          return true;

        case UnitCommandTypes::Enum::Cancel_Train:
          return true;

        case UnitCommandTypes::Enum::Cancel_Train_Slot:
          return canCancelTrainSlot(game, thisUnit, c.extra, false, false);

        case UnitCommandTypes::Enum::Cancel_Morph:
          return true;

        case UnitCommandTypes::Enum::Cancel_Research:
          return true;

        case UnitCommandTypes::Enum::Cancel_Upgrade:
          return true;

        case UnitCommandTypes::Enum::Use_Tech:
          return canUseTechWithoutTarget(game, thisUnit, c.extra, false, false);

        case UnitCommandTypes::Enum::Use_Tech_Unit:
          return canUseTechUnit(game, thisUnit, c.extra, c.target, checkCanTargetUnit, checkCanUseTechUnitOnUnits, false, false);

        case UnitCommandTypes::Enum::Use_Tech_Position:
          return canUseTechPosition(game, thisUnit, c.extra, c.getTargetPosition(), checkCanUseTechPositionOnPositions, false, false);

        case UnitCommandTypes::Enum::Place_COP:
          return canPlaceCOP(game, thisUnit, BWAPI::TilePosition(c.x, c.y), false, false);
      }

      return true;
    }
    static inline bool canIssueCommandGrouped(Game const &game, Unit thisUnit, UnitCommand c, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true)
    {
      if ( !checkCommandibility )
        game.setLastError();
      else if ( !canCommand(game, thisUnit) )
        return false;

      if ( checkCommandibilityGrouped && !canCommandGrouped(game, thisUnit, false) )
        return false;

      BWAPI::UnitCommandType ct = c.type;
      if ( checkCanIssueCommandType && !canIssueCommandTypeGrouped(game, thisUnit, ct, false, false) )
        return false;

      switch (ct)
      {
        case UnitCommandTypes::Enum::Attack_Move:
          return true;

        case UnitCommandTypes::Enum::Attack_Unit:
          return canAttackUnitGrouped(game, thisUnit, c.target, checkCanTargetUnit, false, false, false);

        case UnitCommandTypes::Enum::Build:
          return false;

        case UnitCommandTypes::Enum::Build_Addon:
          return false;

        case UnitCommandTypes::Enum::Train:
          return canTrain(game, thisUnit, c.getUnitType(), false, false);

        case UnitCommandTypes::Enum::Morph:
          return canMorph(game, thisUnit, c.getUnitType(), false, false);

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
          return canFollow(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Gather:
          return canGather(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Return_Cargo:
          return true;

        case UnitCommandTypes::Enum::Repair:
          return canRepair(game, thisUnit, c.target, checkCanTargetUnit, false, false);

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
          return canLoad(game, thisUnit, c.target, checkCanTargetUnit, false, false);

        case UnitCommandTypes::Enum::Unload:
          return false;

        case UnitCommandTypes::Enum::Unload_All:
          return false;

        case UnitCommandTypes::Enum::Unload_All_Position:
          return canUnloadAllPosition(game, thisUnit, c.getTargetPosition(), false, false);

        case UnitCommandTypes::Enum::Right_Click_Position:
          return true;

        case UnitCommandTypes::Enum::Right_Click_Unit:
          return canRightClickUnitGrouped(game, thisUnit, c.target, checkCanTargetUnit, false, false, false);

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
          return canUseTechWithoutTarget(game, thisUnit, c.extra, false, false);

        case UnitCommandTypes::Enum::Use_Tech_Unit:
          return canUseTechUnit(game, thisUnit, c.extra, c.target, checkCanTargetUnit, checkCanUseTechUnitOnUnits, false, false);

        case UnitCommandTypes::Enum::Use_Tech_Position:
          return canUseTechPosition(game, thisUnit, c.extra, c.getTargetPosition(), checkCanUseTechPositionOnPositions, false, false);

        case UnitCommandTypes::Enum::Place_COP:
          return false;
      }

      return true;
    }
  }
}
