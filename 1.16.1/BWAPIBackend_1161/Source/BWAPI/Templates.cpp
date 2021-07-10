#include "Templates.h"

#include "GameImpl.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"

#include <BWAPI/WeaponType.h>

namespace BWAPI4::Templates {
  //--------------------------------------------- HAS POWER ------------------------------------------------
  constexpr bool bPsiFieldMask[10][16] = {
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
  bool hasPower(int x, int y, BWAPI::UnitType unitType, const Unitset& pylons)
  {
    if (unitType >= 0 && unitType < BWAPI::UnitTypes::None && (!unitType.requiresPsi() || !unitType.isBuilding()))
      return true;

    // Loop through all pylons for the current player
    for (Unit i : pylons)
    {
      if (!i->exists() || !i->isCompleted())
        continue;

      BWAPI::Position p = i->getPosition();
      if (abs(p.x - x) >= 256)
        continue;

      if (abs(p.y - y) >= 160)
        continue;

      if (bPsiFieldMask[(y - p.y + 160) / 32][(x - p.x + 256) / 32])
        return true;
    }
    return false;
  }

  //------------------------------------------- CAN BUILD HERE ---------------------------------------------
  bool canBuildHere(Unit builder, TilePosition position, UnitType type, bool checkExplored)
  {
    BroodwarImpl.setLastError(Errors::Unbuildable_Location);

    if (builder && type.isAddon())
    {
      position += TilePosition(4, 1); // addon build offset
    }

    // lt = left top, rb = right bottom
    TilePosition lt = position;
    TilePosition rb = lt + type.tileSize();

    // Map limit check
    if (!BroodwarImpl.isValid(lt) || !BroodwarImpl.isValid(Position(rb) - Position(1, 1)))
      return false;

    //if the unit is a refinery, we just need to check the set of geysers to see if the position
    //matches one of them (and the type is still vespene geyser)
    if (type.isRefinery())
    {
      for (Unit g : BroodwarImpl.getGeysers())
      {
        if (g->getTilePosition() == position)
        {
          if (g->isVisible() && g->getType() != UnitTypes::Resource_Vespene_Geyser)
            return false;
          return BroodwarImpl.setLastError();
        }
      }
      return false;
    }

    // Tile buildability check
    for (int x = lt.x; x < rb.x; ++x)
    {
      for (int y = lt.y; y < rb.y; ++y)
      {
        // Check if tile is buildable/unoccupied and explored.
        if (!BroodwarImpl.isBuildable(x, y) || (checkExplored && !BroodwarImpl.isExplored(x, y)))
          return false; // @TODO: Error code for !isExplored ??
      }
    }

    // Check if builder is capable of reaching the building site
    if (builder)
    {
      if (!builder->getType().isBuilding())
      {
        if (!builder->hasPath(Position(lt) + Position(type.tileSize()) / 2))
          return false;
      }
      else if (!builder->getType().isFlyingBuilding() && type != UnitTypes::Zerg_Nydus_Canal && !type.isFlagBeacon())
      {
        return false;
      }
    }

    // Ground unit dimension check
    if (type != UnitTypes::Special_Start_Location)
    {
      Position targPos = Position(lt) + Position(type.tileSize()) / 2;

      auto collisionUnitsCondition = [&](Unit u) {
        if (u->isFlying() || u->isLoaded()) return false;

        // We do a nydus check because a nydus canal cannot build on itself
        if (u == builder && type != UnitTypes::Zerg_Nydus_Canal) return false;

        return u->getLeft() <= targPos.x + type.dimensionRight() &&
          u->getTop() <= targPos.y + type.dimensionDown() &&
          u->getRight() >= targPos.x - type.dimensionLeft() &&
          u->getBottom() >= targPos.y - type.dimensionUp();
      };

      Unitset unitsInRect = BroodwarImpl.getUnitsInRectangle(Position(lt), Position(rb), collisionUnitsCondition);
      for (Unit u : unitsInRect)
      {
        BWAPI::UnitType iterType = u->getType();
        // Addons can be placed over units that can move, pushing them out of the way
        if (!(type.isAddon() && iterType.canMove()))
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
            if (needsCreep != BroodwarImpl.hasCreep(x, y))
              return false;
          }
        }
      }

      // Power Check
      if (type.requiresPsi() && !BroodwarImpl.hasPower(lt, type))
        return false;

    } //don't ignore units

    // Resource Check (CC, Nex, Hatch)
    if (type.isResourceDepot())
    {
      for (BWAPI4::Unit m : BroodwarImpl.getStaticMinerals())
      {
        TilePosition tp = m->getInitialTilePosition();
        if ((BroodwarImpl.isVisible(tp) || BroodwarImpl.isVisible(tp.x + 1, tp.y)) && !m->exists())
          continue; // tile position is visible, but mineral is not => mineral does not exist
        if (tp.x > lt.x - 5 &&
          tp.y > lt.y - 4 &&
          tp.x < lt.x + 7 &&
          tp.y < lt.y + 6)
          return false;
      }
      for (BWAPI4::Unit g : BroodwarImpl.getStaticGeysers())
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
    if (builder && !builder->getType().isAddon() && type.isAddon())
    {
      if (!canBuildHere(builder, lt - TilePosition(4, 1), builder->getType(), checkExplored))
        return false;
    }

    //if the build site passes all these tests, return true.
    return BroodwarImpl.setLastError();
  }
  //------------------------------------------- CAN MAKE ---------------------------------------------------
  bool canMake(Unit builder, UnitType type)
  {
    // Error checking
    BroodwarImpl.setLastError();
    if (!BroodwarImpl.self())
      return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

    // Check if the unit type is available (UMS game)
    if (!BroodwarImpl.self()->isUnitAvailable(type))
      return BroodwarImpl.setLastError(Errors::Access_Denied);

    // Get the required UnitType
    BWAPI::UnitType requiredType = type.whatBuilds().first;

    Player pSelf = BroodwarImpl.self();
    if (builder != nullptr) // do checks if a builder is provided
    {
      // Check if the owner of the unit is you
      if (builder->getPlayer() != pSelf)
        return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

      BWAPI::UnitType builderType = builder->getType();
      if (type == UnitTypes::Zerg_Nydus_Canal && builderType == UnitTypes::Zerg_Nydus_Canal)
      {
        if (!builder->isCompleted())
          return BroodwarImpl.setLastError(Errors::Unit_Busy);

        if (builder->getNydusExit())
          return BroodwarImpl.setLastError(Errors::Unknown);

        return true;
      }

      // Check if this unit can actually build the unit type
      if (requiredType == UnitTypes::Zerg_Larva && builderType.producesLarva())
      {
        if (builder->getLarva().size() == 0)
          return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
      }
      else if (builderType != requiredType)
      {
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      }

      // Carrier/Reaver space checking
      int max_amt;
      switch (builderType)
      {
      case UnitTypes::Enum::Protoss_Carrier:
      case UnitTypes::Enum::Hero_Gantrithor:
        // Get max interceptors
        max_amt = 4;
        if (pSelf->getUpgradeLevel(UpgradeTypes::Carrier_Capacity) > 0 || builderType == UnitTypes::Hero_Gantrithor)
          max_amt += 4;

        // Check if there is room
        if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= max_amt)
          return BroodwarImpl.setLastError(Errors::Insufficient_Space);
        break;
      case UnitTypes::Enum::Protoss_Reaver:
      case UnitTypes::Enum::Hero_Warbringer:
        // Get max scarabs
        max_amt = 5;
        if (pSelf->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0 || builderType == UnitTypes::Hero_Warbringer)
          max_amt += 5;

        // check if there is room
        if (builder->getScarabCount() + static_cast<int>(builder->getTrainingQueue().size()) >= max_amt)
          return BroodwarImpl.setLastError(Errors::Insufficient_Space);
        break;
      }
    } // if builder != nullptr

    // Check if player has enough minerals
    if (pSelf->minerals() < type.mineralPrice())
      return BroodwarImpl.setLastError(Errors::Insufficient_Minerals);

    // Check if player has enough gas
    if (pSelf->gas() < type.gasPrice())
      return BroodwarImpl.setLastError(Errors::Insufficient_Gas);

    // Check if player has enough supplies
    BWAPI::Race typeRace = type.getRace();
    const int supplyRequired = type.supplyRequired() * (type.isTwoUnitsInOneEgg() ? 2 : 1);
    if (supplyRequired > 0 && pSelf->supplyTotal(typeRace) < pSelf->supplyUsed(typeRace) + supplyRequired - (requiredType.getRace() == typeRace ? requiredType.supplyRequired() : 0))
      return BroodwarImpl.setLastError(Errors::Insufficient_Supply);

    UnitType addon = UnitTypes::None;
    for (auto& it : type.requiredUnits())
    {
      if (it.first.isAddon())
        addon = it.first;

      if (!pSelf->hasUnitTypeRequirement(it.first, it.second))
        return BroodwarImpl.setLastError(Errors::Insufficient_Tech);
    }

    if (type.requiredTech() != TechTypes::None && !pSelf->hasResearched(type.requiredTech()))
      return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

    if (builder &&
      addon != UnitTypes::None &&
      addon.whatBuilds().first == type.whatBuilds().first &&
      (!builder->getAddon() || builder->getAddon()->getType() != addon))
      return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

    return true;
  }
  //------------------------------------------- CAN COMMAND ------------------------------------------------
  bool canCommand(Unit thisUnit)
  {
    // Basic header
    BroodwarImpl.setLastError();
    if (thisUnit->getPlayer() != BroodwarImpl.self())
      return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

    if (!thisUnit->exists())
      return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);

    // Global can be ordered check
    if (thisUnit->isLockedDown() ||
      thisUnit->isMaelstrommed() ||
      thisUnit->isStasised() ||
      !thisUnit->isPowered() ||
      thisUnit->getOrder() == Orders::ZergBirth ||
      thisUnit->isLoaded())
    {
      if (!thisUnit->getType().producesLarva())
      {
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
      }
      else
      {
        Unitset larvae(thisUnit->getLarva());
        for (Unit larva : larvae)
        {
          if (canCommand(larva))
            return BroodwarImpl.setLastError();
        }
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
      }
    }

    UnitType uType = thisUnit->getType();
    if (uType == UnitTypes::Protoss_Interceptor ||
      uType == UnitTypes::Terran_Vulture_Spider_Mine ||
      uType == UnitTypes::Spell_Scanner_Sweep ||
      uType == UnitTypes::Special_Map_Revealer)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->isCompleted() &&
      (uType == UnitTypes::Protoss_Pylon ||
        uType == UnitTypes::Terran_Supply_Depot ||
        uType.isResourceContainer() ||
        uType == UnitTypes::Protoss_Shield_Battery ||
        uType == UnitTypes::Terran_Nuclear_Missile ||
        uType.isPowerup() ||
        (uType.isSpecialBuilding() && !uType.isFlagBeacon())))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (!thisUnit->isCompleted() &&
      !uType.isBuilding() &&
      !thisUnit->isMorphing())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return BroodwarImpl.setLastError();
  }
  bool canCommandGrouped(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->getType().isBuilding() || thisUnit->getType().isCritter())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  //------------------------------------------- CAN TARGET -------------------------------------------------
  bool canTargetUnit(Unit targetUnit)
  {
    if (!targetUnit || !targetUnit->exists())
      return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
    if (!targetUnit->isVisible() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
      return false;
    if (!targetUnit->isCompleted() &&
      !targetUnit->getType().isBuilding() &&
      !targetUnit->isMorphing() &&
      targetUnit->getType() != UnitTypes::Protoss_Archon &&
      targetUnit->getType() != UnitTypes::Protoss_Dark_Archon)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (targetUnit->getType() == UnitTypes::Spell_Scanner_Sweep ||
      targetUnit->getType() == UnitTypes::Spell_Dark_Swarm ||
      targetUnit->getType() == UnitTypes::Spell_Disruption_Web ||
      targetUnit->getType() == UnitTypes::Special_Map_Revealer)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canTargetUnit(Unit thisUnit, Unit targetUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canTargetUnit(targetUnit))
      return false;

    return true;
  }
  //------------------------------------------- CAN ATTACK -------------------------------------------------
  bool canAttack(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canAttackMove(thisUnit, false) && !canAttackUnit(thisUnit, false))
      return false;

    return true;
  }
  bool canAttack(Unit thisUnit, Position, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canAttackMove(thisUnit, false))
      return false;

    return true;
  }
  bool canAttack(Unit thisUnit, Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (target == nullptr)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
    if (!canAttackUnit(thisUnit, target, checkCanTargetUnit, checkCanIssueCommandType, false))
      return false;

    return true;
  }
  bool canAttackGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!canAttackMoveGrouped(thisUnit, false, false) && !canAttackUnitGrouped(thisUnit, false, false))
      return false;

    return true;
  }
  bool canAttackGrouped(Unit thisUnit, Position, bool checkCanTargetUnit, bool checkCanIssueCommandTypeGrouped, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!canAttackMoveGrouped(thisUnit, false, false))
      return false;

    return true;
  }
  bool canAttackGrouped(Unit thisUnit, Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandTypeGrouped, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (target == nullptr)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
    if (!canAttackUnitGrouped(thisUnit, target, checkCanTargetUnit, checkCanIssueCommandTypeGrouped, false, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN ATTACK MOVE --------------------------------------------
  bool canAttackMove(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if ((thisUnit->getType() != UnitTypes::Terran_Medic && !canAttackUnit(thisUnit, false)) || !canMove(thisUnit, false))
      return false;

    return true;
  }
  bool canAttackMoveGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!thisUnit->getType().canMove() &&
      thisUnit->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode &&
      thisUnit->getType() != UnitTypes::Zerg_Cocoon &&
      thisUnit->getType() != UnitTypes::Zerg_Lurker_Egg)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  //------------------------------------------- CAN ATTACK UNIT --------------------------------------------
  bool canAttackUnit(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->getType().groundWeapon() == WeaponTypes::None && thisUnit->getType().airWeapon() == WeaponTypes::None)
    {
      if (thisUnit->getType() == UnitTypes::Protoss_Carrier || thisUnit->getType() == UnitTypes::Hero_Gantrithor)
      {
        if (thisUnit->getInterceptorCount() <= 0)
          return BroodwarImpl.setLastError(Errors::Unable_To_Hit);
      }
      else if (thisUnit->getType() == UnitTypes::Protoss_Reaver || thisUnit->getType() == UnitTypes::Hero_Warbringer)
      {
        if (thisUnit->getScarabCount() <= 0)
          return BroodwarImpl.setLastError(Errors::Unable_To_Hit);
      }
      else
        return BroodwarImpl.setLastError(Errors::Unable_To_Hit);
    }
    if (thisUnit->getType() == UnitTypes::Zerg_Lurker)
    {
      if (!thisUnit->isBurrowed())
        return BroodwarImpl.setLastError(Errors::Unable_To_Hit);
    }
    else if (thisUnit->isBurrowed())
      return BroodwarImpl.setLastError(Errors::Unable_To_Hit);
    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->getOrder() == Orders::ConstructingBuilding)
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    return true;
  }
  bool canAttackUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canAttackUnit(thisUnit, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    if (targetUnit->isInvincible())
      return BroodwarImpl.setLastError(Errors::Unable_To_Hit);

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
          BroodwarImpl.setLastError(Errors::Unable_To_Hit);
        break;
      default:
        return BroodwarImpl.setLastError(Errors::Unable_To_Hit);
      }
    }

    if (!thisUnit->getType().canMove() && !thisUnit->isInWeaponRange(targetUnit))
      return BroodwarImpl.setLastError(Errors::Out_Of_Range);

    if (thisUnit->getType() == UnitTypes::Zerg_Lurker && !thisUnit->isInWeaponRange(targetUnit))
      return BroodwarImpl.setLastError(Errors::Out_Of_Range);

    if (targetUnit == thisUnit)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    return true;
  }
  bool canAttackUnitGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    if (!thisUnit->getType().canMove() &&
      thisUnit->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    if (thisUnit->getType() == UnitTypes::Zerg_Lurker)
    {
      if (!thisUnit->isBurrowed())
        return BroodwarImpl.setLastError(Errors::Unable_To_Hit);
    }
    else if (thisUnit->isBurrowed())
      return BroodwarImpl.setLastError(Errors::Unable_To_Hit);

    if (thisUnit->getOrder() == Orders::ConstructingBuilding)
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    return true;
  }
  bool canAttackUnitGrouped(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandTypeGrouped, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (checkCanIssueCommandTypeGrouped && !canAttackUnitGrouped(thisUnit, false, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    if (targetUnit->isInvincible())
      return BroodwarImpl.setLastError(Errors::Unable_To_Hit);

    if (thisUnit->getType() == UnitTypes::Zerg_Lurker && !thisUnit->isInWeaponRange(targetUnit))
      return BroodwarImpl.setLastError(Errors::Out_Of_Range);

    if (thisUnit->getType() == UnitTypes::Zerg_Queen &&
      (targetUnit->getType() != UnitTypes::Terran_Command_Center ||
        targetUnit->getHitPoints() >= 750 || targetUnit->getHitPoints() <= 0))
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    if (targetUnit == thisUnit)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    return true;
  }
  //------------------------------------------- CAN BUILD --------------------------------------------------
  bool canBuild(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->isConstructing() ||
      !thisUnit->isCompleted() ||
      (thisUnit->getType().isBuilding() && !thisUnit->isIdle()))
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canBuild(Unit thisUnit, UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canBuild(thisUnit, false))
      return false;

    if (!BroodwarImpl.canMake(uType, thisUnit))
      return false;

    if (!uType.isBuilding())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->getAddon())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canBuild(Unit thisUnit, UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canBuild(thisUnit, false))
      return false;

    if (checkTargetUnitType && !canBuild(thisUnit, uType, false, false))
      return false;

    if (BroodwarImpl.isValid(tilePos) == false)
      return BroodwarImpl.setLastError(Errors::Invalid_Tile_Position);

    if (!BroodwarImpl.canBuildHere(tilePos, uType, thisUnit, true))
      return false;

    return true;
  }
  //------------------------------------------- CAN BUILD ADDON --------------------------------------------
  bool canBuildAddon(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->isConstructing() ||
      !thisUnit->isCompleted() ||
      thisUnit->isLifted() ||
      (thisUnit->getType().isBuilding() && !thisUnit->isIdle()))
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->getAddon())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (!thisUnit->getType().canBuildAddon())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canBuildAddon(Unit thisUnit, UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canBuildAddon(thisUnit, false))
      return false;

    if (!BroodwarImpl.canMake(uType, thisUnit))
      return false;

    if (!uType.isAddon())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    if (!BroodwarImpl.canBuildHere(thisUnit->getTilePosition(), uType, thisUnit))
      return false;

    return true;
  }
  //------------------------------------------- CAN TRAIN --------------------------------------------------
  bool canTrain(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->getType().producesLarva())
    {
      if (!thisUnit->isConstructing() && thisUnit->isCompleted())
        return BroodwarImpl.setLastError();
      Unitset larvae(thisUnit->getLarva());
      for (Unit larva : larvae)
      {
        if (!larva->isConstructing() && larva->isCompleted() && canCommand(larva))
          return BroodwarImpl.setLastError();
      }
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    }

    if (thisUnit->isConstructing() ||
      !thisUnit->isCompleted() ||
      thisUnit->isLifted())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!thisUnit->getType().canProduce() &&
      thisUnit->getType() != UnitTypes::Enum::Terran_Nuclear_Silo &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Hydralisk &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Mutalisk &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Creep_Colony &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Spire &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Larva)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canTrain(Unit thisUnit, UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canTrain(thisUnit, false))
      return false;

    if (thisUnit->getType().producesLarva())
    {
      if (uType.whatBuilds().first == UnitTypes::Zerg_Larva)
      {
        bool foundCommandableLarva = false;
        Unitset larvae(thisUnit->getLarva());
        for (Unit larva : larvae)
        {
          if (canTrain(larva, true))
          {
            foundCommandableLarva = true;
            thisUnit = larva;
            break;
          }
        }
        if (!foundCommandableLarva)
          return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
      }
      else if (thisUnit->isConstructing() ||
        !thisUnit->isCompleted())
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
    }

    if (!BroodwarImpl.canMake(uType, thisUnit))
      return false;

    if (uType.isAddon() || (uType.isBuilding() && !thisUnit->getType().isBuilding()))
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (uType == UnitTypes::Enum::Zerg_Larva || uType == UnitTypes::Enum::Zerg_Egg || uType == UnitTypes::Enum::Zerg_Cocoon)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  //------------------------------------------- CAN MORPH --------------------------------------------------
  bool canMorph(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->getType().producesLarva())
    {
      if (!thisUnit->isConstructing() && thisUnit->isCompleted() && (!thisUnit->getType().isBuilding() || thisUnit->isIdle()))
        return BroodwarImpl.setLastError();
      Unitset larvae(thisUnit->getLarva());
      for (Unit larva : larvae)
      {
        if (!larva->isConstructing() && larva->isCompleted() && canCommand(larva))
          return BroodwarImpl.setLastError();
      }
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    }

    if (thisUnit->isConstructing() ||
      !thisUnit->isCompleted() ||
      (thisUnit->getType().isBuilding() && !thisUnit->isIdle()))
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->getType() != UnitTypes::Enum::Zerg_Hydralisk &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Mutalisk &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Creep_Colony &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Spire &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Hatchery &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Lair &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Hive &&
      thisUnit->getType() != UnitTypes::Enum::Zerg_Larva)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canMorph(Unit thisUnit, UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canMorph(thisUnit, false))
      return false;

    if (thisUnit->getType().producesLarva())
    {
      if (uType.whatBuilds().first == UnitTypes::Zerg_Larva)
      {
        bool foundCommandableLarva = false;
        Unitset larvae(thisUnit->getLarva());
        for (Unit larva : larvae)
        {
          if (canMorph(larva, true))
          {
            foundCommandableLarva = true;
            thisUnit = larva;
            break;
          }
        }
        if (!foundCommandableLarva)
          return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
      }
      else if (thisUnit->isConstructing() ||
        !thisUnit->isCompleted() ||
        (thisUnit->getType().isBuilding() && !thisUnit->isIdle()))
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
    }

    if (!BroodwarImpl.canMake(uType, thisUnit))
      return false;
    if (uType == UnitTypes::Enum::Zerg_Larva || uType == UnitTypes::Enum::Zerg_Egg || uType == UnitTypes::Enum::Zerg_Cocoon)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  //------------------------------------------- CAN RESEARCH -----------------------------------------------
  bool canResearch(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    return true;
  }
  bool canResearch(Unit thisUnit, TechType type, bool checkCanIssueCommandType)
  {
    // Error checking
    if (!BroodwarImpl.self())
      return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

    if (thisUnit)
    {
      if (thisUnit->getPlayer() != BroodwarImpl.self())
        return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

      if (!thisUnit->getType().isSuccessorOf(type.whatResearches()))
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

      if (checkCanIssueCommandType && (thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted()))
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
    }
    if (BroodwarImpl.self()->isResearching(type))
      return BroodwarImpl.setLastError(Errors::Currently_Researching);

    if (BroodwarImpl.self()->hasResearched(type))
      return BroodwarImpl.setLastError(Errors::Already_Researched);

    if (!BroodwarImpl.self()->isResearchAvailable(type))
      return BroodwarImpl.setLastError(Errors::Access_Denied);

    if (BroodwarImpl.self()->minerals() < type.mineralPrice())
      return BroodwarImpl.setLastError(Errors::Insufficient_Minerals);

    if (BroodwarImpl.self()->gas() < type.gasPrice())
      return BroodwarImpl.setLastError(Errors::Insufficient_Gas);

    if (!BroodwarImpl.self()->hasUnitTypeRequirement(type.requiredUnit()))
      return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

    return BroodwarImpl.setLastError();
  }
  //------------------------------------------- CAN UPGRADE ------------------------------------------------
  bool canUpgrade(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    return true;
  }
  bool canUpgrade(Unit thisUnit, UpgradeType type, bool checkCanIssueCommandType)
  {
    Player self = BroodwarImpl.self();
    if (!self)
      return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

    if (thisUnit)
    {
      if (thisUnit->getPlayer() != self)
        return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

      if (!thisUnit->getType().isSuccessorOf(type.whatUpgrades()))
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

      if (checkCanIssueCommandType && (thisUnit->isLifted() || !thisUnit->isIdle() || !thisUnit->isCompleted()))
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
    }
    int nextLvl = self->getUpgradeLevel(type) + 1;

    if (!self->hasUnitTypeRequirement(type.whatUpgrades()))
      return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);

    if (!self->hasUnitTypeRequirement(type.whatsRequired(nextLvl)))
      return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

    if (self->isUpgrading(type))
      return BroodwarImpl.setLastError(Errors::Currently_Upgrading);

    if (self->getUpgradeLevel(type) >= self->getMaxUpgradeLevel(type))
      return BroodwarImpl.setLastError(Errors::Fully_Upgraded);

    if (self->minerals() < type.mineralPrice(nextLvl))
      return BroodwarImpl.setLastError(Errors::Insufficient_Minerals);

    if (self->gas() < type.gasPrice(nextLvl))
      return BroodwarImpl.setLastError(Errors::Insufficient_Gas);

    return BroodwarImpl.setLastError();
  }
  //------------------------------------------- CAN SET RALLY POINT ----------------------------------------
  bool canSetRallyPoint(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canSetRallyPosition(thisUnit, false) && !canSetRallyUnit(thisUnit, false))
      return false;

    return true;
  }
  bool canSetRallyPoint(Unit thisUnit, Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (target == nullptr)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
    if (!canSetRallyUnit(thisUnit, target, checkCanTargetUnit, checkCanIssueCommandType, false))
      return false;

    return true;
  }
  bool canSetRallyPoint(Unit thisUnit, Position, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canSetRallyPosition(thisUnit, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN SET RALLY POSITION -------------------------------------
  bool canSetRallyPosition(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().canProduce() || !thisUnit->getType().isBuilding())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->isLifted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN SET RALLY UNIT -----------------------------------------
  bool canSetRallyUnit(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().canProduce() || !thisUnit->getType().isBuilding())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->isLifted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canSetRallyUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canSetRallyUnit(thisUnit, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN MOVE ---------------------------------------------------
  bool canMove(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding())
    {
      if (!thisUnit->isInterruptible())
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
      if (!thisUnit->getType().canMove())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (thisUnit->isBurrowed())
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
      if (thisUnit->getOrder() == Orders::ConstructingBuilding)
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
      if (thisUnit->getType() == UnitTypes::Zerg_Larva)
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    }
    else
    {
      if (!thisUnit->getType().isFlyingBuilding())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (!thisUnit->isLifted())
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
    }

    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canMoveGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!thisUnit->getType().canMove())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (!thisUnit->isCompleted() && !thisUnit->isMorphing())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN PATROL -------------------------------------------------
  bool canPatrol(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canMove(thisUnit, false))
      return false;

    return true;
  }
  bool canPatrolGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!canMoveGrouped(thisUnit, false, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN FOLLOW -------------------------------------------------
  bool canFollow(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canMove(thisUnit, false))
      return false;

    return true;
  }
  bool canFollow(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canFollow(thisUnit, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    if (targetUnit == thisUnit)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    return true;
  }
  //------------------------------------------- CAN GATHER -------------------------------------------------
  bool canGather(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!thisUnit->getType().isWorker())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->getPowerUp())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->isBurrowed())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->getOrder() == Orders::ConstructingBuilding)
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    return true;
  }
  bool canGather(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canGather(thisUnit, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    UnitType uType = targetUnit->getType();
    if (!uType.isResourceContainer() || uType == UnitTypes::Resource_Vespene_Geyser)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    if (!targetUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    if (!thisUnit->hasPath(targetUnit->getPosition()))
      return BroodwarImpl.setLastError(Errors::Unreachable_Location);

    if (uType.isRefinery() && targetUnit->getPlayer() != BroodwarImpl.self())
      return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

    return true;
  }
  //------------------------------------------- CAN RETURN CARGO -------------------------------------------
  bool canReturnCargo(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!thisUnit->getType().isWorker())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (!thisUnit->isCarryingGas() && !thisUnit->isCarryingMinerals())
      return BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
    if (thisUnit->isBurrowed())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->getOrder() == Orders::ConstructingBuilding)
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    return true;
  }
  //------------------------------------------- CAN HOLD POSITION ------------------------------------------
  bool canHoldPosition(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding())
    {
      if (!thisUnit->getType().canMove())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (thisUnit->isBurrowed() && thisUnit->getType() != UnitTypes::Zerg_Lurker)
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
      if (thisUnit->getOrder() == Orders::ConstructingBuilding)
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
      if (thisUnit->getType() == UnitTypes::Zerg_Larva)
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    }
    else
    {
      if (!thisUnit->getType().isFlyingBuilding())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (!thisUnit->isLifted())
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
    }

    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN STOP ---------------------------------------------------
  bool canStop(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->isBurrowed() && thisUnit->getType() != UnitTypes::Zerg_Lurker)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->getType().isBuilding() && !thisUnit->isLifted() &&
      thisUnit->getType() != UnitTypes::Protoss_Photon_Cannon &&
      thisUnit->getType() != UnitTypes::Zerg_Sunken_Colony &&
      thisUnit->getType() != UnitTypes::Zerg_Spore_Colony &&
      thisUnit->getType() != UnitTypes::Terran_Missile_Turret)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN REPAIR -------------------------------------------------
  bool canRepair(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->getType() != BWAPI::UnitTypes::Terran_SCV)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->getOrder() == Orders::ConstructingBuilding)
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    return true;
  }
  bool canRepair(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canRepair(thisUnit, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    UnitType targType = targetUnit->getType();
    if (targType.getRace() != BWAPI::Races::Terran || !targType.isMechanical())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (targetUnit->getHitPoints() == targType.maxHitPoints())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (!targetUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (targetUnit == thisUnit)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    return true;
  }
  //------------------------------------------- CAN BURROW -------------------------------------------------
  bool canBurrow(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canUseTechWithoutTarget(thisUnit, BWAPI::TechTypes::Burrowing, true, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN UNBURROW -----------------------------------------------
  bool canUnburrow(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBurrowable())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (!thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Unburrowing)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN CLOAK --------------------------------------------------
  bool canCloak(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canUseTechWithoutTarget(thisUnit, thisUnit->getType().cloakingTech(), true, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN DECLOAK ------------------------------------------------
  bool canDecloak(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->getType().cloakingTech() == TechTypes::None)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->getSecondaryOrder() != Orders::Cloak)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN SIEGE --------------------------------------------------
  bool canSiege(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canUseTechWithoutTarget(thisUnit, BWAPI::TechTypes::Tank_Siege_Mode, true, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN UNSIEGE ------------------------------------------------
  bool canUnsiege(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->isSieged())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging)
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  //------------------------------------------- CAN LIFT ---------------------------------------------------
  bool canLift(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isFlyingBuilding())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (thisUnit->isLifted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (!thisUnit->isIdle())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    return true;
  }
  //------------------------------------------- CAN LAND ---------------------------------------------------
  bool canLand(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isFlyingBuilding())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (!thisUnit->isLifted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canLand(Unit thisUnit, TilePosition target, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canLand(thisUnit, checkCommandibility))
      return false;

    if (!canBuildHere(nullptr, target, thisUnit->getType(), true))
      return false;

    return true;
  }
  //------------------------------------------- CAN LOAD ---------------------------------------------------
  bool canLoad(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->getType() == UnitTypes::Zerg_Overlord && BroodwarImpl.self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
      return BroodwarImpl.setLastError(Errors::Insufficient_Tech);
    if (thisUnit->isBurrowed())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->getOrder() == Orders::ConstructingBuilding)
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (thisUnit->getType() == UnitTypes::Zerg_Larva)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canLoad(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canLoad(thisUnit, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    //target must also be owned by self
    if (targetUnit->getPlayer() != BroodwarImpl.self())
      return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);

    if (targetUnit->isLoaded() || !targetUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    // verify upgrade for Zerg Overlord
    if (targetUnit->getType() == UnitTypes::Zerg_Overlord && BroodwarImpl.self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
      return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

    int thisUnitSpaceProvided = thisUnit->getType().spaceProvided();
    int targetSpaceProvided = targetUnit->getType().spaceProvided();
    if (thisUnitSpaceProvided <= 0 && targetSpaceProvided <= 0)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    const BWAPI4::Unit unitToBeLoaded = (thisUnitSpaceProvided > 0 ? targetUnit : thisUnit);
    if (unitToBeLoaded->getType().canMove() == false || unitToBeLoaded->getType().isFlyer() || unitToBeLoaded->getType().spaceRequired() > 8)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    if (!unitToBeLoaded->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (unitToBeLoaded->isBurrowed())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    const BWAPI4::Unit unitThatLoads = (thisUnitSpaceProvided > 0 ? thisUnit : targetUnit);
    if (unitThatLoads->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    if (unitThatLoads->getType() == UnitTypes::Terran_Bunker)
    {
      if (!unitToBeLoaded->getType().isOrganic() || unitToBeLoaded->getType().getRace() != Races::Terran)
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (!unitToBeLoaded->hasPath(unitThatLoads->getPosition()))
        return BroodwarImpl.setLastError(Errors::Unreachable_Location);
    }

    int freeSpace = (thisUnitSpaceProvided > 0 ? thisUnitSpaceProvided : targetSpaceProvided);
    Unitset loadedUnits = unitThatLoads->getLoadedUnits();
    for (Unit u : loadedUnits)
    {
      const int requiredSpace = u->getType().spaceRequired();
      if (requiredSpace > 0 && requiredSpace < 8)
        freeSpace -= requiredSpace;
    }
    if (unitToBeLoaded->getType().spaceRequired() > freeSpace)
      return BroodwarImpl.setLastError(Errors::Insufficient_Space);

    return true;
  }
  //------------------------------------------- CAN UNLOAD -------------------------------------------------
  bool canUnloadWithOrWithoutTarget(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    if (thisUnit->getLoadedUnits().size() == 0)
      return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);

    // Check overlord tech
    if (thisUnit->getType() == UnitTypes::Zerg_Overlord && BroodwarImpl.self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
      return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

    if (thisUnit->getType().spaceProvided() <= 0)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canUnloadAtPosition(Unit thisUnit, Position targDropPos, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUnloadWithOrWithoutTarget(thisUnit, false))
      return false;

    if (thisUnit->getType() != UnitTypes::Terran_Bunker)
    {
      if (BroodwarImpl.isValid(WalkPosition(targDropPos.x / 8, targDropPos.y / 8)) == false)
        return BroodwarImpl.setLastError(Errors::Invalid_Tile_Position);
      else if (!BroodwarImpl.isWalkable(targDropPos.x / 8, targDropPos.y / 8))
        return BroodwarImpl.setLastError(Errors::Unreachable_Location);
    }

    return true;
  }
  bool canUnload(Unit thisUnit, bool checkCommandibility)
  {
    return canUnloadAtPosition(thisUnit, thisUnit->getPosition(), true, checkCommandibility);
  }
  bool canUnload(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkPosition, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUnloadWithOrWithoutTarget(thisUnit, false))
      return false;

    if (checkPosition && !canUnloadAtPosition(thisUnit, thisUnit->getPosition(), false, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    if (!targetUnit->isLoaded())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    if (targetUnit->getTransport() != thisUnit)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    return true;
  }
  //------------------------------------------- CAN UNLOAD ALL ---------------------------------------------
  bool canUnloadAll(Unit thisUnit, bool checkCommandibility)
  {
    return canUnloadAtPosition(thisUnit, thisUnit->getPosition(), true, checkCommandibility);
  }
  //------------------------------------------- CAN UNLOAD ALL POSITION ------------------------------------
  bool canUnloadAllPosition(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canUnloadWithOrWithoutTarget(thisUnit, false))
      return false;

    if (thisUnit->getType() == UnitTypes::Terran_Bunker)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canUnloadAllPosition(Unit thisUnit, Position targDropPos, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUnloadAllPosition(thisUnit, false))
      return false;

    if (!canUnloadAtPosition(thisUnit, targDropPos, false, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN RIGHT CLICK --------------------------------------------
  bool canRightClick(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canRightClickPosition(thisUnit, false) && !canRightClickUnit(thisUnit, false))
      return false;

    return true;
  }
  bool canRightClick(Unit thisUnit, Position, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canRightClickPosition(thisUnit, false))
      return false;

    return true;
  }
  bool canRightClick(Unit thisUnit, Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (target == nullptr)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
    if (!canRightClickUnit(thisUnit, target, checkCanTargetUnit, checkCanIssueCommandType, false))
      return false;

    return true;
  }
  bool canRightClickGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!canRightClickPositionGrouped(thisUnit, false, false) && !canRightClickUnitGrouped(thisUnit, false, false))
      return false;

    return true;
  }
  bool canRightClickGrouped(Unit thisUnit, Position, bool checkCanTargetUnit, bool checkCanIssueCommandTypeGrouped, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!canRightClickPositionGrouped(thisUnit, false, false))
      return false;

    return true;
  }
  bool canRightClickGrouped(Unit thisUnit, Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandTypeGrouped, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (target == nullptr)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
    if (!canRightClickUnitGrouped(thisUnit, target, checkCanTargetUnit, checkCanIssueCommandTypeGrouped, false, false))
      return false;

    return true;
  }
  //------------------------------------------- CAN RIGHT CLICK POSITION -----------------------------------
  bool canRightClickPosition(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!canMove(thisUnit, false) && !canSetRallyPosition(thisUnit, false))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canRightClickPositionGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!canMoveGrouped(thisUnit, false, false))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN RIGHT CLICK UNIT ---------------------------------------
  bool canRightClickUnit(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!canFollow(thisUnit, false) &&
      !canAttackUnit(thisUnit, false) &&
      !canLoad(thisUnit, false) &&
      !canSetRallyUnit(thisUnit, false))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canRightClickUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canRightClickUnit(thisUnit, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    if (!targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()) &&
      !canAttackUnit(thisUnit, targetUnit, false, true, false))
      return false;

    if (!canFollow(thisUnit, targetUnit, false, true, false) &&
      !canLoad(thisUnit, targetUnit, false, true, false) &&
      !canSetRallyUnit(thisUnit, targetUnit, false, true, false))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canRightClickUnitGrouped(Unit thisUnit, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (!thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!canFollow(thisUnit, false) &&
      !canAttackUnitGrouped(thisUnit, false, false) &&
      !canLoad(thisUnit, false))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canRightClickUnitGrouped(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    if (checkCanIssueCommandType && !canRightClickUnitGrouped(thisUnit, false, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    if (!targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()) &&
      !canAttackUnitGrouped(thisUnit, targetUnit, false, true, false, false))
      return false;

    if (!canFollow(thisUnit, targetUnit, false, true, false) &&
      !canLoad(thisUnit, targetUnit, false, true, false))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN HALT CONSTRUCTION --------------------------------------
  bool canHaltConstruction(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->getOrder() != Orders::ConstructingBuilding)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN CANCEL CONSTRUCTION ------------------------------------
  bool canCancelConstruction(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    if (thisUnit->isCompleted() || (thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit()))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN CANCEL ADDON -------------------------------------------
  bool canCancelAddon(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getAddon() || thisUnit->getAddon()->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  //------------------------------------------- CAN CANCEL TRAIN -------------------------------------------
  bool canCancelTrain(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->isTraining())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN CANCEL TRAIN SLOT --------------------------------------
  bool canCancelTrainSlot(Unit thisUnit, bool checkCommandibility)
  {
    return canCancelTrain(thisUnit, checkCommandibility);
  }
  bool canCancelTrainSlot(Unit thisUnit, int slot, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canCancelTrainSlot(thisUnit, false))
      return false;

    if (!thisUnit->isTraining() || (thisUnit->getTrainingQueue().size() <= (unsigned int)slot && slot >= 0))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN CANCEL MORPH -------------------------------------------
  bool canCancelMorph(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->isMorphing() || (!thisUnit->isCompleted() && thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit()))
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  //------------------------------------------- CAN CANCEL RESEARCH ----------------------------------------
  bool canCancelResearch(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->getOrder() != Orders::ResearchTech)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN CANCEL UPGRADE -----------------------------------------
  bool canCancelUpgrade(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (thisUnit->getOrder() != Orders::Upgrade)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  //------------------------------------------- CAN USE TECH -----------------------------------------------
  bool canUseTechWithOrWithoutTarget(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isBuilding() && !thisUnit->isInterruptible())
      return BroodwarImpl.setLastError(Errors::Unit_Busy);
    if (!thisUnit->isCompleted())
      return BroodwarImpl.setLastError(Errors::Incompatible_State);
    if (thisUnit->isHallucination())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    return true;
  }
  bool canUseTechWithOrWithoutTarget(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false))
      return false;

    // researched check
    if (!thisUnit->getType().isHero() && !BroodwarImpl.self()->hasResearched(tech) && thisUnit->getType() != UnitTypes::Zerg_Lurker)
      return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

    // energy check
    if (thisUnit->getEnergy() < tech.energyCost())
      return BroodwarImpl.setLastError(Errors::Insufficient_Energy);

    // unit check
    if (tech != TechTypes::Burrowing && tech.whatUses().count(thisUnit->getType()) == 0)
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    switch (tech)
    {
    case TechTypes::Enum::Spider_Mines:
      if (thisUnit->getSpiderMineCount() <= 0)
        return BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
      return true;

    case TechTypes::Enum::Tank_Siege_Mode:
      if (thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging)
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
      return true;

    case TechTypes::Enum::Cloaking_Field:
    case TechTypes::Enum::Personnel_Cloaking:
      if (thisUnit->getSecondaryOrder() == Orders::Cloak)
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
      return true;

    case TechTypes::Enum::Burrowing:
      if (!thisUnit->getType().isBurrowable())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Burrowing || thisUnit->getOrder() == Orders::Unburrowing)
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
      return true;

    case TechTypes::Enum::None:
      return BroodwarImpl.setLastError(Errors::Incompatible_TechType);

    case TechTypes::Enum::Nuclear_Strike:
      if (thisUnit->getPlayer()->completedUnitCount(UnitTypes::Terran_Nuclear_Missile) == 0)
        return BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
      return true;

    case TechTypes::Enum::Unknown:
      return BroodwarImpl.setLastError(Errors::Incompatible_TechType);
    }

    return true;
  }
  bool canUseTech(Unit thisUnit, BWAPI::TechType tech, Position target, bool checkCanTargetUnit, bool checkTargetsType, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!canUseTechPosition(thisUnit, tech, target, checkTargetsType, checkCanIssueCommandType, false))
      return false;

    return true;
  }
  bool canUseTech(Unit thisUnit, BWAPI::TechType tech, Unit target, bool checkCanTargetUnit, bool checkTargetsType, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (target == nullptr)
    {
      if (!canUseTechWithoutTarget(thisUnit, tech, checkCanIssueCommandType, false))
        return false;
    }
    else
    {
      if (!canUseTechUnit(thisUnit, tech, target, checkCanTargetUnit, checkTargetsType, checkCanIssueCommandType, false))
        return false;
    }

    return true;
  }
  bool canUseTechWithoutTarget(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false))
      return false;

    if (!canUseTechWithOrWithoutTarget(thisUnit, tech, false, false))
      return false;
    if (tech.targetsUnit() || tech.targetsPosition() || tech == TechTypes::None || tech == TechTypes::Unknown || tech == TechTypes::Lurker_Aspect)
      return BroodwarImpl.setLastError(Errors::Incompatible_TechType);

    return true;
  }
  //------------------------------------------- CAN USE TECH UNIT ------------------------------------------
  bool canUseTechUnit(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false))
      return false;

    if (!canUseTechWithOrWithoutTarget(thisUnit, tech, false, false))
      return false;
    if (!tech.targetsUnit())
      return BroodwarImpl.setLastError(Errors::Incompatible_TechType);

    return true;
  }
  bool canUseTechUnit(Unit thisUnit, BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit, bool checkTargetsUnits, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false))
      return false;

    if (checkTargetsUnits && !canUseTechUnit(thisUnit, tech, false, false))
      return false;

    if (checkCanTargetUnit && !canTargetUnit(thisUnit, targetUnit, false))
      return false;

    UnitType targetType = targetUnit->getType();

    switch (tech)
    {
    case TechTypes::Enum::Archon_Warp:
      if (targetType != UnitTypes::Protoss_High_Templar)
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (targetUnit->getPlayer() != thisUnit->getPlayer())
        return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      break;

    case TechTypes::Enum::Dark_Archon_Meld:
      if (targetType != UnitTypes::Protoss_Dark_Templar)
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (targetUnit->getPlayer() != thisUnit->getPlayer())
        return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      break;

    case TechTypes::Enum::Consume:
      if (targetUnit->getPlayer() != thisUnit->getPlayer())
        return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      if (targetType.getRace() != Races::Zerg || targetType == UnitTypes::Zerg_Larva)
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      break;

    case TechTypes::Enum::Spawn_Broodlings:
      if ((!targetType.isOrganic() && !targetType.isMechanical()) ||
        targetType.isRobotic() ||
        targetType.isFlyer())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      break;

    case TechTypes::Enum::Lockdown:
      if (!targetType.isMechanical())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      break;

    case TechTypes::Enum::Healing:
      if (targetUnit->getHitPoints() == targetType.maxHitPoints())
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
      if (!targetType.isOrganic() ||
        targetType.isFlyer())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if (!targetUnit->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(targetUnit->getPlayer()))
        return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
      break;

    case TechTypes::Enum::Mind_Control:
      if (targetUnit->getPlayer() == thisUnit->getPlayer())
        return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
      if (targetType == UnitTypes::Protoss_Interceptor ||
        targetType == UnitTypes::Terran_Vulture_Spider_Mine ||
        targetType == UnitTypes::Zerg_Lurker_Egg ||
        targetType == UnitTypes::Zerg_Cocoon ||
        targetType == UnitTypes::Zerg_Larva ||
        targetType == UnitTypes::Zerg_Egg)
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      break;

    case TechTypes::Enum::Feedback:
      if (!targetType.isSpellcaster())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      break;

    case TechTypes::Enum::Infestation:
      if (targetType != UnitTypes::Terran_Command_Center ||
        targetUnit->getHitPoints() >= 750 || targetUnit->getHitPoints() <= 0)
        return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
      break;
    }

    switch (tech)
    {
    case TechTypes::Enum::Archon_Warp:
    case TechTypes::Enum::Dark_Archon_Meld:
      if (!thisUnit->hasPath(targetUnit->getPosition()))
        return BroodwarImpl.setLastError(Errors::Unreachable_Location);
      if (targetUnit->isHallucination())
        return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
      if (targetUnit->isMaelstrommed())
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
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
      if (targetUnit->isStasised())
        return BroodwarImpl.setLastError(Errors::Incompatible_State);
      break;
    }

    switch (tech)
    {
    case TechTypes::Enum::Yamato_Gun:
      if (targetUnit->isInvincible())
        return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
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
      if (targetUnit->isInvincible())
        return BroodwarImpl.setLastError(Errors::Invalid_Parameter);
      // Fall through (don't break).
    case TechTypes::Enum::Consume:
    case TechTypes::Enum::Feedback:
      if (targetType.isBuilding())
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      break;
    }

    if (targetUnit == thisUnit)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    return true;
  }
  //------------------------------------------- CAN USE TECH POSITION --------------------------------------
  bool canUseTechPosition(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false))
      return false;

    if (!canUseTechWithOrWithoutTarget(thisUnit, tech, false, false))
      return false;
    if (!tech.targetsPosition())
      return BroodwarImpl.setLastError(Errors::Incompatible_TechType);

    return true;
  }
  bool canUseTechPosition(Unit thisUnit, BWAPI::TechType tech, Position target, bool checkTargetsPositions, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(thisUnit, false))
      return false;

    if (checkTargetsPositions && !canUseTechPosition(thisUnit, tech, false, false))
      return false;

    if (tech == TechTypes::Enum::Spider_Mines && !thisUnit->hasPath(target))
      return BroodwarImpl.setLastError(Errors::Unreachable_Location);

    return true;
  }
  //------------------------------------------- CAN PLACE COP ----------------------------------------------
  bool canPlaceCOP(Unit thisUnit, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (!thisUnit->getType().isFlagBeacon())
      return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    if (static_cast<UnitImpl*>(thisUnit)->self->buttonset == 228 || thisUnit->getOrder() != BWAPI::Orders::CTFCOPInit)
      return BroodwarImpl.setLastError(Errors::Incompatible_State);

    return true;
  }
  bool canPlaceCOP(Unit thisUnit, TilePosition target, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCanIssueCommandType && !canPlaceCOP(thisUnit, checkCommandibility))
      return false;

    if (!canBuildHere(thisUnit, target, thisUnit->getType(), true))
      return false;

    return true;
  }
  //------------------------------------------- CAN ISSUE COMMAND TYPE -------------------------------------
  bool canIssueCommandType(Unit thisUnit, BWAPI4::UnitCommandType ct, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
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
  bool canIssueCommandTypeGrouped(Unit thisUnit, BWAPI4::UnitCommandType ct, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
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
  bool canIssueCommand(Unit thisUnit, UnitCommand c, bool checkCanUseTechPositionOnPositions, bool checkCanUseTechUnitOnUnits, bool checkCanBuildUnitType, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    BWAPI4::UnitCommandType ct = c.type;
    if (checkCanIssueCommandType && !canIssueCommandType(thisUnit, ct, false))
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
      return BroodwarImpl.canResearch(c.getTechType(), thisUnit, false);

    case UnitCommandTypes::Enum::Upgrade:
      return BroodwarImpl.canUpgrade(c.getUpgradeType(), thisUnit, false);

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
      return canUseTechWithoutTarget(thisUnit, TechType(c.extra), false, false);

    case UnitCommandTypes::Enum::Use_Tech_Unit:
      return canUseTechUnit(thisUnit, TechType(c.extra), c.target, checkCanTargetUnit, checkCanUseTechUnitOnUnits, false, false);

    case UnitCommandTypes::Enum::Use_Tech_Position:
      return canUseTechPosition(thisUnit, TechType(c.extra), c.getTargetPosition(), checkCanUseTechPositionOnPositions, false, false);

    case UnitCommandTypes::Enum::Place_COP:
      return canPlaceCOP(thisUnit, BWAPI::TilePosition(c.x, c.y), false, false);
    }

    return true;
  }
  bool canIssueCommandGrouped(Unit thisUnit, UnitCommand c, bool checkCanUseTechPositionOnPositions, bool checkCanUseTechUnitOnUnits, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility)
  {
    if (!checkCommandibility)
      BroodwarImpl.setLastError();
    else if (!canCommand(thisUnit))
      return false;

    if (checkCommandibilityGrouped && !canCommandGrouped(thisUnit, false))
      return false;

    BWAPI4::UnitCommandType ct = c.type;
    if (checkCanIssueCommandType && !canIssueCommandTypeGrouped(thisUnit, ct, false, false))
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
      return canUseTechWithoutTarget(thisUnit, TechType(c.extra), false, false);

    case UnitCommandTypes::Enum::Use_Tech_Unit:
      return canUseTechUnit(thisUnit, TechType(c.extra), c.target, checkCanTargetUnit, checkCanUseTechUnitOnUnits, false, false);

    case UnitCommandTypes::Enum::Use_Tech_Position:
      return canUseTechPosition(thisUnit, TechType(c.extra), c.getTargetPosition(), checkCanUseTechPositionOnPositions, false, false);

    case UnitCommandTypes::Enum::Place_COP:
      return false;
    }

    return true;
  }
}