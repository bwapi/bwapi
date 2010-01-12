#include "UnitType.h"

#include <math.h>
#include <Util/Logger.h>

#include "Unit.h"
#include "WeaponID.h"
#include "Offsets.h"
#include "Position.h"
#include "UnitTypeID.h"

namespace BW
{
  static std::pair< BW::UnitType, int > whatBuildsData[BW::UNIT_TYPE_COUNT];
  static std::map< BW::UnitType, int > requiredUnitsData[BW::UNIT_TYPE_COUNT];
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitType::UnitType()
      : id(BW::UnitTypeIDs::None)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitType::UnitType(const u16& id)
      : id(id)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UnitType::operator ==(const u16& id) const
  {
    return id == this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UnitType::operator !=(const u16& id) const
  {
    return id != this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UnitType::operator ==(const UnitType& type) const
  {
    return this->id == type.id;
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool UnitType::operator <(const UnitType& type) const
  {
    return this->id < type.id;
  }
  //------------------------------------------------- GET ID -------------------------------------------------
  u16 UnitType::getID() const
  {
    return this->id;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  const char* UnitType::getName() const
  {
    if (this->getID() == BW::UnitTypeIDs::None)
      return "None";
    else if (this->getID() < BW::UNIT_TYPE_COUNT)
      return (char*)(*((u16*)(*(u32*)BW::BWDATA_StringTableOff + this->getID() * 2 + 2)) + *((u32*)BW::BWDATA_StringTableOff));
    else
      return "Invalid";
  }
  //---------------------------------------------- GET SUB LABEL ---------------------------------------------
  const char* UnitType::getSubLabel() const
  {
    if (this->getID() == BW::UnitTypeIDs::None)
      return "None";
    else if (this->getID() < BW::UNIT_TYPE_COUNT)
    {
      u16 label = BW::BWDATA_UnitSubLabel->unitType[this->getID()];
      if (label == 0)
      {
        return "";
      }
      return (char*)(*((u16*)(*(u32*)BW::BWDATA_StringTableOff + (label + 1302) * 2)) + *((u32*)BW::BWDATA_StringTableOff));
    }
    else
      return "Invalid";
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  u8 UnitType::getRace() const
  {
    if (this->isZerg())
      return BW::RaceIDs::Zerg;
    else if (this->isProtoss())
      return BW::RaceIDs::Protoss;
    else if (this->isTerran())
      return BW::RaceIDs::Terran;
    else
      return BW::RaceIDs::Other;
  }
  //---------------------------------------------- WHAT BUILDS -----------------------------------------------
  std::pair<BW::UnitType, int> UnitType::whatBuilds() const
  {
    return whatBuildsData[this->getID()];
  }
  //--------------------------------------------- REQUIRED UNITS ---------------------------------------------
  const std::map< BW::UnitType, int >& UnitType::requiredUnits() const
  {
    return requiredUnitsData[this->id];
  }
  //--------------------------------------------- REQUIRED TECH ----------------------------------------------
  u8 UnitType::requiredTech() const
  {
    if (this->getID() == BW::UnitTypeIDs::Zerg_Lurker)
      return BW::TechIDs::LurkerAspect;
    return BW::TechIDs::None;
  }

  //--------------------------------------------- ARMOR UPGRADE ----------------------------------------------
  u8 UnitType::armorUpgrade() const
  {
    return BW::BWDATA_UnitUpgrade->unitType[this->getID()];
  }
  //--------------------------------------------- MAX HIT POINTS ---------------------------------------------
  s32 UnitType::maxHitPoints() const
  {
    return BW::BWDATA_MaxHitPoints->unitType[this->getID()];
  }
  //----------------------------------------------- MAX SHIELDS ----------------------------------------------
  s16 UnitType::maxShields() const
  {
    if (BW::BWDATA_ShieldsEnabled->unitType[this->getID()] != 0)
    {
      return BW::BWDATA_MaxShieldPoints->unitType[this->getID()];
    }
    return 0;
  }
  //------------------------------------------------ MAX ENERGY ----------------------------------------------
  u8 UnitType::maxEnergy() const
  {
    if (this->isSpellcaster())
    {
      return 200;
    }
    return 0;
  }
  //------------------------------------------------ MAX ARMOR -----------------------------------------------
  u8 UnitType::armor() const
  {
    return BW::BWDATA_Armor->unitType[this->getID()];
  }
  //--------------------------------------------- MINERAL PRICE ----------------------------------------------
  u16 UnitType::mineralPrice() const
  {
    return BW::BWDATA_MineralPrices->unitType[this->getID()];
  }
  //----------------------------------------------- GAS PRICE ------------------------------------------------
  u16 UnitType::gasPrice() const
  {
    return BW::BWDATA_GasPrices->unitType[this->getID()];
  }
  //----------------------------------------------- BUILD TIME -----------------------------------------------
  u16 UnitType::buildTime() const
  {
    return BW::BWDATA_BuildTime->unitType[this->getID()];
  }

  //--------------------------------------------- SUPPLY REQUIRED --------------------------------------------
  u8 UnitType::supplyRequired() const
  {
    return BW::BWDATA_SupplyRequired->unitType[this->getID()];
  }
  //--------------------------------------------- SUPPLY PROVIDED --------------------------------------------
  u8 UnitType::supplyProvided() const
  {
    return BW::BWDATA_SupplyProvided->unitType[this->getID()];
  }
  //--------------------------------------------- SPACE REQUIRED ---------------------------------------------
  u8 UnitType::spaceRequired() const
  {
    return BW::BWDATA_SpaceRequired->unitType[this->getID()];
  }
  //--------------------------------------------- SPACE PROVIDED ---------------------------------------------
  u8 UnitType::spaceProvided() const
  {
    return BW::BWDATA_SpaceProvided->unitType[this->getID()];
  }
  //----------------------------------------------- BUILD SCORE ----------------------------------------------
  u16 UnitType::buildScore() const
  {
    return BW::BWDATA_BuildScore->unitType[this->getID()];
  }
  //---------------------------------------------- DESTROY SCORE ---------------------------------------------
  u16 UnitType::destroyScore() const
  {
    return BW::BWDATA_DestroyScore->unitType[this->getID()];
  }

  //-------------------------------------------------- SIZE --------------------------------------------------
  u8 UnitType::size() const
  {
    return BW::BWDATA_UnitSize->unitType[this->getID()];
  }
  //----------------------------------------------- TILE WIDTH -----------------------------------------------
  u16 UnitType::tileWidth() const
  {
    return (this->dimensionLeft() + this->dimensionRight() + BW::TILE_SIZE - 1) / BW::TILE_SIZE;
  }
  //---------------------------------------------- TILE HEIGHT -----------------------------------------------
  u16 UnitType::tileHeight() const
  {
    return (this->dimensionUp() + this->dimensionDown() + BW::TILE_SIZE - 1) / BW::TILE_SIZE;
  }
  //--------------------------------------------- DIMENSION LEFT ---------------------------------------------
  u16 UnitType::dimensionLeft() const
  {
    return BW::BWDATA_UnitDimensions->units[this->getID()].left;
  }
  //---------------------------------------------- DIMENSION UP ----------------------------------------------
  u16 UnitType::dimensionUp() const
  {
    return BW::BWDATA_UnitDimensions->units[this->getID()].up;
  }
  //-------------------------------------------- DIMENSION RIGHT ---------------------------------------------
  u16 UnitType::dimensionRight() const
  {
    return BW::BWDATA_UnitDimensions->units[this->getID()].right;
  }
  //--------------------------------------------- DIMENSION DOWN ---------------------------------------------
  u16 UnitType::dimensionDown() const
  {
    return BW::BWDATA_UnitDimensions->units[this->getID()].down;
  }

  //----------------------------------------------- SEEK RANGE -----------------------------------------------
  u8 UnitType::seekRange() const
  {
    return BW::BWDATA_UnitSeekRange->unitType[this->getID()];
  }
  //----------------------------------------------- SIGHT RANGE ----------------------------------------------
  u8 UnitType::sightRange() const
  {
    return BW::BWDATA_UnitSightRange->unitType[this->getID()];
  }
  //--------------------------------------------- GROUND WEAPON ----------------------------------------------
  u8 UnitType::groundWeapon() const
  {
    u8 w = BW::BWDATA_UnitGroundWeapon->unitType[this->getID()];
    if (w == BW::WeaponID::None && BW::BWDATA_SubUnit1->unitType[this->getID()] != BW::UnitTypeIDs::None)
    {
      w = BW::BWDATA_UnitGroundWeapon->unitType[BW::BWDATA_SubUnit1->unitType[this->getID()]];
    }
    return w;
  }
  //-------------------------------------------- MAX GROUND HITS ---------------------------------------------
  u8 UnitType::maxGroundHits() const
  {
    return BW::BWDATA_MaxGroundHits->unitType[this->getID()];
  }
  //---------------------------------------------- AIR WEAPON ------------------------------------------------
  u8 UnitType::airWeapon() const
  {
    u8 w = BW::BWDATA_UnitAirWeapon->unitType[this->getID()];
    if (w == BW::WeaponID::None && BW::BWDATA_SubUnit1->unitType[this->getID()] != BW::UnitTypeIDs::None)
    {
      w = BW::BWDATA_UnitAirWeapon->unitType[BW::BWDATA_SubUnit1->unitType[this->getID()]];
    }
    return w;
  }
  //--------------------------------------------- MAX AIR HITS -----------------------------------------------
  u8 UnitType::maxAirHits() const
  {
    return BW::BWDATA_MaxAirHits->unitType[this->getID()];
  }

  //----------------------------------------------- TOP SPEED ------------------------------------------------
  u32 UnitType::topSpeed() const
  {
    return BW::BWDATA_FlingyTopSpeed->flingyType[this->graphics()];
  }
  //---------------------------------------------- ACCELERATION ----------------------------------------------
  u16 UnitType::acceleration() const
  {
    return BW::BWDATA_FlingyAcceleration->flingyType[this->graphics()];
  }
  //---------------------------------------------- HALT DISTANCE ---------------------------------------------
  u32 UnitType::haltDistance() const
  {
    return BW::BWDATA_FlingyHaltDistance->flingyType[this->graphics()];
  }
  //---------------------------------------------- TURN RADIUS -----------------------------------------------
  u8 UnitType::turnRadius() const
  {
    return BW::BWDATA_FlingyTurnRadius->flingyType[this->graphics()];
  }

  //---------------------------------------------- CAN PRODUCE -----------------------------------------------
  bool UnitType::canProduce() const
  {
    return this->getGroupFlags().getBit<BW::GroupFlags::Factory>();
  }
  //----------------------------------------------- CAN ATTACK -----------------------------------------------
  bool UnitType::canAttack() const
  {
    return BW::BWDATA_UnitPrototypeFlags->unit[this->getID()].getBit<BW::UnitPrototypeFlags::Attack>()
           && (this->groundWeapon() != BW::WeaponID::None || this->airWeapon() != BW::WeaponID::None);
  }
  //------------------------------------------------ CAN MOVE ------------------------------------------------
  bool UnitType::canMove() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()].getBit<BW::GroupFlags::Men>();
  }
  //----------------------------------------------- IS FLYER -------------------------------------------------
  bool UnitType::isFlyer() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Flyer>();
  }
  //---------------------------------------------- REGENERATES HP --------------------------------------------
  bool UnitType::regeneratesHP() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::RegeneratesHP>();
  }
  //---------------------------------------------- IS SPELL CASTER -------------------------------------------
  bool UnitType::isSpellcaster() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Spellcaster>();
  }
  //-------------------------------------------- HAS PERMANENT CLOAK -----------------------------------------
  bool UnitType::hasPermanentCloak() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::PermanentCloak>();
  }
  //----------------------------------------------- IS INVINCIBLE --------------------------------------------
  bool UnitType::isInvincible() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Invincible>();
  }
  //----------------------------------------------- IS ORGANIC -----------------------------------------------
  bool UnitType::isOrganic() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Organicunit>();
  }
  //--------------------------------------------- IS MECHANACAL ----------------------------------------------
  bool UnitType::isMechanical() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Mechanical>();
  }
  //---------------------------------------------- IS ROBOTIC ------------------------------------------------
  bool UnitType::isRobotic() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::RoboticUnit>();
  }
  //---------------------------------------------- IS DETECTOR -----------------------------------------------
  bool UnitType::isDetector() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Detector>();
  }
  //----------------------------------------- IS RESOURCE CONTAINER ------------------------------------------
  bool UnitType::isResourceContainer() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::ResourceContainer>();
  }
  //----------------------------------------- IS RESOURCE CONTAINER ------------------------------------------
  bool UnitType::isResourceDepot() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::ResourceDepot>();
  }
  //---------------------------------------------- IS REFINERY -----------------------------------------------
  bool UnitType::isRefinery() const
  {
    return (this->getID()==BW::UnitTypeIDs::Terran_Refinery ||
            this->getID()==BW::UnitTypeIDs::Protoss_Assimilator ||
            this->getID()==BW::UnitTypeIDs::Zerg_Extractor);
  }
  //----------------------------------------------- IS WORKER ------------------------------------------------
  bool UnitType::isWorker() const
  {
    return BW::BWDATA_UnitPrototypeFlags->unit[this->getID()].getBit<BW::UnitPrototypeFlags::Worker>();
  }
  //---------------------------------------------- REQUIRES PSI ----------------------------------------------
  bool UnitType::requiresPsi() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::RequiredPsi>();
  }
  //--------------------------------------------- REQUIRES CREEP ---------------------------------------------
  bool UnitType::requiresCreep() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::CreepBuilding>();
  }
  //----------------------------------------- IS TWO UNITS IN ONE EGG ----------------------------------------
  bool UnitType::isTwoUnitsInOneEgg() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::TwoUnitsIn1Egg>();
  }
  //--------------------------------------------- IS BURROWABLE ----------------------------------------------
  bool UnitType::isBurrowable() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Burrowable>();
  }
  //--------------------------------------------- IS CLOAKABLE ----------------------------------------------
  bool UnitType::isCloakable() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Cloakable>();
  }
  //---------------------------------------------- IS BUILDING -----------------------------------------------
  bool UnitType::isBuilding() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Building>();
  }
  //------------------------------------------------ IS ADDON ------------------------------------------------
  bool UnitType::isAddon() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::Addon>();
  }
  //------------------------------------------------ IS ADDON ------------------------------------------------
  bool UnitType::isFlyingBuilding() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::FlyingBuilding>();
  }
  //----------------------------------------------- IS NEUTRAL -----------------------------------------------
  bool UnitType::isNeutral() const
  {
    return this->getGroupFlags().getBit<BW::GroupFlags::Neutral>();
  }

  //------------------------------------------------ GRAPHICS ------------------------------------------------
  u8 UnitType::graphics() const
  {
    return BW::BWDATA_UnitGraphics->unitType[this->getID()];
  }
  //----------------------------------------- IS NEUTRAL ACCESORIES ------------------------------------------
  bool UnitType::isNeutralAccesories() const
  {
    return this->getFlags().getBit<BW::UnitPrototypeFlags::NeutralAccessories>();
  }
  //------------------------------------------------ IS ZERG -------------------------------------------------
  bool UnitType::isZerg() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()].getBit<BW::GroupFlags::Zerg>();
  }
  //----------------------------------------------- IS TERRAN ------------------------------------------------
  bool UnitType::isTerran() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()].getBit<BW::GroupFlags::Terran>();
  }
  //----------------------------------------------- IS PROTOSS -----------------------------------------------
  bool UnitType::isProtoss() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()].getBit<BW::GroupFlags::Protoss>();
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool UnitType::isValid() const
  {
    return this->id < BW::UNIT_TYPE_COUNT;
  }

  //---------------------------------------------- INITIALIZE ------------------------------------------------
  void UnitType::initialize()
  {
    for(int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
    {
      whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::None), 0);
    }
    for(int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
    {
      switch (i)
      {
          // Terran Basic Buildings
        case BW::UnitTypeIDs::Terran_CommandCenter :
        case BW::UnitTypeIDs::Terran_SupplyDepot   :
        case BW::UnitTypeIDs::Terran_Refinery      :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1));
          break;
        case BW::UnitTypeIDs::Terran_Barracks :
        case BW::UnitTypeIDs::Terran_EngineeringBay :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_CommandCenter), 1));
          break;
        case BW::UnitTypeIDs::Terran_Academy :
        case BW::UnitTypeIDs::Terran_Bunker  :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Barracks), 1));
          break;
        case BW::UnitTypeIDs::Terran_MissileTurret :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_EngineeringBay), 1));
          break;

          // Terran Advanced Buildings
        case BW::UnitTypeIDs::Terran_Factory :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Barracks), 1));
          break;
        case BW::UnitTypeIDs::Terran_Armory   :
        case BW::UnitTypeIDs::Terran_Starport :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Factory), 1));
          break;
        case BW::UnitTypeIDs::Terran_ScienceFacility :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1));
          break;

          // Terran Add-ons
        case BW::UnitTypeIDs::Terran_ComsatStation :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_CommandCenter), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Academy), 1));
          break;
        case BW::UnitTypeIDs::Terran_NuclearSilo :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_CommandCenter), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_ScienceFacility), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_CovertOps), 1));
          break;
        case BW::UnitTypeIDs::Terran_MachineShop :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Factory), 1));
          break;
        case BW::UnitTypeIDs::Terran_ControlTower :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1));
          break;
        case BW::UnitTypeIDs::Terran_PhysicsLab :
        case BW::UnitTypeIDs::Terran_CovertOps  :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_ScienceFacility), 1));
          break;

          // Terran Units
        case BW::UnitTypeIDs::Terran_SCV :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_CommandCenter), 1));
          break;
        case BW::UnitTypeIDs::Terran_Marine :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Barracks), 1));
          break;
        case BW::UnitTypeIDs::Terran_Firebat :
        case BW::UnitTypeIDs::Terran_Medic   :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Barracks), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Academy), 1));
          break;
        case BW::UnitTypeIDs::Terran_Ghost :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Barracks), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Academy), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_CovertOps), 1));
          break;
        case BW::UnitTypeIDs::Terran_Vulture :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Factory), 1));
          break;
        case BW::UnitTypeIDs::Terran_SiegeTankSiegeMode :
        case BW::UnitTypeIDs::Terran_SiegeTankTankMode  :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Factory), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_MachineShop), 1));
          break;
        case BW::UnitTypeIDs::Terran_Goliath :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Factory), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Armory), 1));
          break;
        case BW::UnitTypeIDs::Terran_Wraith :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1));
          break;
        case BW::UnitTypeIDs::Terran_Dropship :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_ControlTower), 1));
          break;
        case BW::UnitTypeIDs::Terran_Battlecruiser :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_ControlTower), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_PhysicsLab), 1));
          break;
        case BW::UnitTypeIDs::Terran_ScienceVessel :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_ControlTower), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_ScienceFacility), 1));
          break;
        case BW::UnitTypeIDs::Terran_Valkyrie :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_ControlTower), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Armory), 1));
          break;

          // Protoss Basic Buildings
        case BW::UnitTypeIDs::Protoss_Nexus       :
        case BW::UnitTypeIDs::Protoss_Pylon       :
        case BW::UnitTypeIDs::Protoss_Assimilator :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Gateway :
        case BW::UnitTypeIDs::Protoss_Forge   :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Nexus), 1));
          break;
        case BW::UnitTypeIDs::Protoss_CyberneticsCore :
        case BW::UnitTypeIDs::Protoss_ShieldBattery   :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Gateway), 1));
          break;
        case BW::UnitTypeIDs::Protoss_PhotonCannon :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Forge), 1));
          break;

          // Protoss Advanced Buildings
        case BW::UnitTypeIDs::Protoss_RoboticsFacility :
        case BW::UnitTypeIDs::Protoss_Stargate         :
        case BW::UnitTypeIDs::Protoss_CitadelOfAdun    :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_CyberneticsCore), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Observatory        :
        case BW::UnitTypeIDs::Protoss_RoboticsSupportBay :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_RoboticsFacility), 1));
          break;
        case BW::UnitTypeIDs::Protoss_FleetBeacon :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Stargate), 1));
          break;
        case BW::UnitTypeIDs::Protoss_TemplarArchives :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_CitadelOfAdun), 1));
          break;
        case BW::UnitTypeIDs::Protoss_ArbiterTribunal :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_TemplarArchives), 1));
          break;

          // Protoss Units
        case BW::UnitTypeIDs::Protoss_Probe :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Nexus), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Zealot :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Gateway), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Dragoon :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Gateway), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_CyberneticsCore), 1));
          break;
        case BW::UnitTypeIDs::Protoss_HighTemplar :
        case BW::UnitTypeIDs::Protoss_DarkTemplar :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Gateway), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_TemplarArchives), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Archon :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_HighTemplar), 2));
          break;
        case BW::UnitTypeIDs::Protoss_DarkArchon :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_DarkTemplar), 2));
          break;
        case BW::UnitTypeIDs::Protoss_Shuttle :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_RoboticsFacility), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Reaver :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_RoboticsFacility), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_RoboticsSupportBay), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Observer :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_RoboticsFacility), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Observatory), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Scout :
        case BW::UnitTypeIDs::Protoss_Corsair :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Stargate), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Carrier :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Stargate), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_FleetBeacon), 1));
          break;
        case BW::UnitTypeIDs::Protoss_Arbiter :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Stargate), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_ArbiterTribunal), 1));
          break;

          // Zerg Basic Buildings
        case BW::UnitTypeIDs::Zerg_Hatchery    :
        case BW::UnitTypeIDs::Zerg_CreepColony :
        case BW::UnitTypeIDs::Zerg_Extractor   :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Drone), 1));
          break;
        case BW::UnitTypeIDs::Zerg_SpawningPool     :
        case BW::UnitTypeIDs::Zerg_EvolutionChamber :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Drone), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hatchery), 1));
          break;
        case BW::UnitTypeIDs::Zerg_HydraliskDen :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Drone), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_SpawningPool), 1));
          break;

          // Zerg Advanced Buildings
        case BW::UnitTypeIDs::Zerg_Spire      :
        case BW::UnitTypeIDs::Zerg_QueensNest :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Drone), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Lair), 1));
          break;
        case BW::UnitTypeIDs::Zerg_DefilerMound    :
        case BW::UnitTypeIDs::Zerg_NydusCanal      :
        case BW::UnitTypeIDs::Zerg_UltraliskCavern :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Drone), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hive), 1));
          break;

          // Zerg Mutated Buildings
        case BW::UnitTypeIDs::Zerg_Lair :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hatchery), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_SpawningPool), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Hive :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Lair), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_QueensNest), 1));
          break;
        case BW::UnitTypeIDs::Zerg_SunkenColony :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_CreepColony), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_SpawningPool), 1));
          break;
        case BW::UnitTypeIDs::Zerg_SporeColony :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_CreepColony), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_EvolutionChamber), 1));
          break;
        case BW::UnitTypeIDs::Zerg_GreaterSpire :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Spire), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hive), 1));
          break;

          // Zerg Units
        case BW::UnitTypeIDs::Zerg_Larva :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hatchery), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Egg :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Larva), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Drone    :
        case BW::UnitTypeIDs::Zerg_Overlord :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Egg), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Zergling :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Egg), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_SpawningPool), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Hydralisk :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Egg), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_HydraliskDen), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Scourge  :
        case BW::UnitTypeIDs::Zerg_Mutalisk :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Egg), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Spire), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Cocoon :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Mutalisk), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Devourer :
        case BW::UnitTypeIDs::Zerg_Guardian :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Cocoon), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_GreaterSpire), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Queen :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Egg), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_QueensNest), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Defiler :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Egg), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_DefilerMound), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Ultralisk :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Egg), 1));
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_UltraliskCavern), 1));
          break;
        case BW::UnitTypeIDs::Zerg_InfestedTerran :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_InfestedCommandCenter), 1));
          break;
        case BW::UnitTypeIDs::Zerg_LurkerEgg :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hydralisk), 1));
          break;
        case BW::UnitTypeIDs::Zerg_Lurker :
          requiredUnitsData[i].insert(std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_LurkerEgg), 1));
          break;
      }

      switch (i)
      {
          // Terran Basic Buildings
        case BW::UnitTypeIDs::Terran_CommandCenter :
        case BW::UnitTypeIDs::Terran_SupplyDepot   :
        case BW::UnitTypeIDs::Terran_Refinery      :
        case BW::UnitTypeIDs::Terran_Barracks :
        case BW::UnitTypeIDs::Terran_EngineeringBay :
        case BW::UnitTypeIDs::Terran_Academy :
        case BW::UnitTypeIDs::Terran_Bunker  :
        case BW::UnitTypeIDs::Terran_MissileTurret :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1);
          break;

          // Terran Advanced Buildings
        case BW::UnitTypeIDs::Terran_Factory :
        case BW::UnitTypeIDs::Terran_Armory   :
        case BW::UnitTypeIDs::Terran_Starport :
        case BW::UnitTypeIDs::Terran_ScienceFacility :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_SCV), 1);
          break;

          // Terran Add-ons
        case BW::UnitTypeIDs::Terran_ComsatStation :
        case BW::UnitTypeIDs::Terran_NuclearSilo :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_CommandCenter), 1);
          break;
        case BW::UnitTypeIDs::Terran_MachineShop :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Factory), 1);
          break;
        case BW::UnitTypeIDs::Terran_ControlTower :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1);
          break;
        case BW::UnitTypeIDs::Terran_PhysicsLab :
        case BW::UnitTypeIDs::Terran_CovertOps  :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_ScienceFacility), 1);
          break;

          // Terran Units
        case BW::UnitTypeIDs::Terran_SCV :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_CommandCenter), 1);
          break;
        case BW::UnitTypeIDs::Terran_Marine :
        case BW::UnitTypeIDs::Terran_Firebat :
        case BW::UnitTypeIDs::Terran_Medic   :
        case BW::UnitTypeIDs::Terran_Ghost :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Barracks), 1);
          break;
        case BW::UnitTypeIDs::Terran_Vulture :
        case BW::UnitTypeIDs::Terran_SiegeTankSiegeMode :
        case BW::UnitTypeIDs::Terran_SiegeTankTankMode  :
        case BW::UnitTypeIDs::Terran_Goliath :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Factory), 1);
          break;
        case BW::UnitTypeIDs::Terran_Wraith :
        case BW::UnitTypeIDs::Terran_Dropship :
        case BW::UnitTypeIDs::Terran_Battlecruiser :
        case BW::UnitTypeIDs::Terran_ScienceVessel :
        case BW::UnitTypeIDs::Terran_Valkyrie :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Terran_Starport), 1);
          break;

          // Protoss Basic Buildings
        case BW::UnitTypeIDs::Protoss_Nexus       :
        case BW::UnitTypeIDs::Protoss_Pylon       :
        case BW::UnitTypeIDs::Protoss_Assimilator :
        case BW::UnitTypeIDs::Protoss_Gateway :
        case BW::UnitTypeIDs::Protoss_Forge   :
        case BW::UnitTypeIDs::Protoss_CyberneticsCore :
        case BW::UnitTypeIDs::Protoss_ShieldBattery   :
        case BW::UnitTypeIDs::Protoss_PhotonCannon :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1);
          break;

          // Protoss Advanced Buildings
        case BW::UnitTypeIDs::Protoss_RoboticsFacility :
        case BW::UnitTypeIDs::Protoss_Stargate         :
        case BW::UnitTypeIDs::Protoss_CitadelOfAdun    :
        case BW::UnitTypeIDs::Protoss_Observatory        :
        case BW::UnitTypeIDs::Protoss_RoboticsSupportBay :
        case BW::UnitTypeIDs::Protoss_FleetBeacon :
        case BW::UnitTypeIDs::Protoss_TemplarArchives :
        case BW::UnitTypeIDs::Protoss_ArbiterTribunal :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Probe), 1);
          break;

          // Protoss Units
        case BW::UnitTypeIDs::Protoss_Probe :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Nexus), 1);
          break;
        case BW::UnitTypeIDs::Protoss_Zealot :
        case BW::UnitTypeIDs::Protoss_Dragoon :
        case BW::UnitTypeIDs::Protoss_HighTemplar :
        case BW::UnitTypeIDs::Protoss_DarkTemplar :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Gateway), 1);
          break;
        case BW::UnitTypeIDs::Protoss_Archon :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_HighTemplar), 2);
          break;
        case BW::UnitTypeIDs::Protoss_DarkArchon :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_DarkTemplar), 2);
          break;
        case BW::UnitTypeIDs::Protoss_Shuttle :
        case BW::UnitTypeIDs::Protoss_Reaver :
        case BW::UnitTypeIDs::Protoss_Observer :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_RoboticsFacility), 1);
          break;
        case BW::UnitTypeIDs::Protoss_Scout :
        case BW::UnitTypeIDs::Protoss_Corsair :
        case BW::UnitTypeIDs::Protoss_Carrier :
        case BW::UnitTypeIDs::Protoss_Arbiter :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Protoss_Stargate), 1);
          break;

          // Zerg Basic Buildings
        case BW::UnitTypeIDs::Zerg_Hatchery    :
        case BW::UnitTypeIDs::Zerg_CreepColony :
        case BW::UnitTypeIDs::Zerg_Extractor   :
        case BW::UnitTypeIDs::Zerg_SpawningPool     :
        case BW::UnitTypeIDs::Zerg_EvolutionChamber :
        case BW::UnitTypeIDs::Zerg_HydraliskDen :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Drone), 1);
          break;

          // Zerg Advanced Buildings
        case BW::UnitTypeIDs::Zerg_Spire      :
        case BW::UnitTypeIDs::Zerg_QueensNest :
        case BW::UnitTypeIDs::Zerg_DefilerMound    :
        case BW::UnitTypeIDs::Zerg_NydusCanal      :
        case BW::UnitTypeIDs::Zerg_UltraliskCavern :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Drone), 1);
          break;

          // Zerg Mutated Buildings
        case BW::UnitTypeIDs::Zerg_Lair :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hatchery), 1);
          break;
        case BW::UnitTypeIDs::Zerg_Hive :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Lair), 1);
          break;
        case BW::UnitTypeIDs::Zerg_SunkenColony :
        case BW::UnitTypeIDs::Zerg_SporeColony :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_CreepColony), 1);
          break;
        case BW::UnitTypeIDs::Zerg_GreaterSpire :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Spire), 1);
          break;

          // Zerg Units
        case BW::UnitTypeIDs::Zerg_Larva :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hatchery), 1);
          break;
        case BW::UnitTypeIDs::Zerg_Egg :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Larva), 1);
          break;
        case BW::UnitTypeIDs::Zerg_Drone    :
        case BW::UnitTypeIDs::Zerg_Overlord :
        case BW::UnitTypeIDs::Zerg_Zergling :
        case BW::UnitTypeIDs::Zerg_Hydralisk :
        case BW::UnitTypeIDs::Zerg_Scourge  :
        case BW::UnitTypeIDs::Zerg_Mutalisk :
        case BW::UnitTypeIDs::Zerg_Queen :
        case BW::UnitTypeIDs::Zerg_Defiler :
        case BW::UnitTypeIDs::Zerg_Ultralisk :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Egg), 1);
          break;
        case BW::UnitTypeIDs::Zerg_Cocoon :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Mutalisk), 1);
          break;
        case BW::UnitTypeIDs::Zerg_Devourer :
        case BW::UnitTypeIDs::Zerg_Guardian :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Cocoon), 1);
          break;
        case BW::UnitTypeIDs::Zerg_InfestedTerran :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_InfestedCommandCenter), 1);
          break;
        case BW::UnitTypeIDs::Zerg_LurkerEgg :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_Hydralisk), 1);
        case BW::UnitTypeIDs::Zerg_Lurker :
          whatBuildsData[i] = std::make_pair(BW::UnitType(BW::UnitTypeIDs::Zerg_LurkerEgg), 1);
          break;
      }
    }
  }
  //----------------------------------------------- GET FLAGS ------------------------------------------------
  Util::BitMask<u32> UnitType::getFlags() const
  {
    return BW::BWDATA_UnitPrototypeFlags->unit[this->getID()];
  }
  //-------------------------------------------- GET GROUP FLAGS ---------------------------------------------
  Util::BitMask<u8> UnitType::getGroupFlags() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()];
  }
  //----------------------------------------------------------------------------------------------------------
};
