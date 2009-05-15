#include "UnitType.h"

#include <math.h>
#include <Util/Logger.h>
#include <BWAPI/Unit.h>

#include "Unit.h"
#include "Offsets.h"
#include "Position.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitType::UnitType()
  :id(BW::UnitID::None)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitType::UnitType(const BW::UnitID::Enum& id)
  :id(id)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UnitType::operator ==(const BW::UnitID::Enum& id) const
  {
    return id == this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UnitType::operator !=(const BW::UnitID::Enum& id) const
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
  //------------------------------------------------ GET NAME ------------------------------------------------
  const char* UnitType::getName() const
  {
    if (this->getID() == BW::UnitID::None)
      return "None";
    else if (this->getID() == BW::UnitID::All)
      return "All";
    else if (this->getID() == BW::UnitID::Men)
      return "Men";
    else if (this->getID() == BW::UnitID::Buildings)
      return "Buildings";
    else if (this->getID() == BW::UnitID::Factories)
      return "Factories";
   	else if (this->getID() == BW::UnitID::Infantry)
      return "Infantry";
    else if (this->getID() == BW::UnitID::Mech)
      return "Mech";
    else if (this->getID() == BW::UnitID::Zerg_Main)
      return "Zerg_Main";
    else if (this->getID() == BW::UnitID::Zerg_MainLair)
      return "Zerg_MainLair";
    else
      return (char*)(*((u16*)(*(u32*)BW::BWDATA_StringTableOff + this->getID()*2 + 2)) + *((u32*)BW::BWDATA_StringTableOff));
  }
  //---------------------------------------------- GET UNIT ID -----------------------------------------------
  BW::UnitID::Enum UnitType::getID() const
  {
    return this->id;
  }
  //----------------------------------------- GET MAX HEALTH POINTS ------------------------------------------
  u16 UnitType::getMaxHealthPoints() const
  {
    return BW::BWDATA_MaxHealthPoints_NotAttackable_Repairable->raw[this->getID()].maxHealthPoints;
  }
  //----------------------------------------- GET MAX HEALTH POINTS ------------------------------------------
  u16 UnitType::getMaxShieldPoints() const
  {
    return BW::BWDATA_MaxShieldPoints->maxShieldPoints[this->getID()];
  }
  //------------------------------------------- GET MINERAL PRICE --------------------------------------------
  u16 UnitType::getMineralPrice() const
  {
    return BW::BWDATA_MineralPrices->mineralPrice[this->getID()];
  }
  //--------------------------------------------- GET GAS PRICE ----------------------------------------------
  u16 UnitType::getGasPrice() const
  {
    return BW::BWDATA_GasPrices->gasPrice[this->getID()];
  }
  //---------------------------------------------- GET SUPPLIES ----------------------------------------------
  s8 UnitType::getSupplies() const
  {
    return BW::BWDATA_SupplyDemands->supplyDemand[this->getID()];
  }
  //----------------------------------------------- GET ARMOR ------------------------------------------------
  u8 UnitType::getArmor() const
  {
    return BW::BWDATA_Armor->armor[this->getID()];
  }
  //--------------------------------------------- GET BUILD TIME ---------------------------------------------
  u16 UnitType::getBuildTime() const
  {
    return BW::BWDATA_BuildTime->buildTime[this->getID()];
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
  //------------------------------------------- GET DAMAGE FACTOR --------------------------------------------
  u8 UnitType::getDamageFactor() const
  {
    int weaponID = BW::BWDATA_UnitGroundWeapon->unit[this->getID()];
    if (weaponID == BW::NoWeapon)
       return 0;
    else
       return BW::BWDATA_WeaponDamageFactor->weapon[weaponID];
  }
  //------------------------------------------- GET GROUND DAMAGE --------------------------------------------
  u16 UnitType::getGroundDamage() const
  {
    int weaponID = BW::BWDATA_UnitGroundWeapon->unit[this->getID()];
    if (weaponID == BW::NoWeapon)
       return 0;
    else
       return BW::BWDATA_WeaponDamage->weapon[weaponID];
  }
  //----------------------------------------------- GET FLAGS ------------------------------------------------
  Util::BitMask<BW::UnitPrototypeFlags::Enum> UnitType::getFlags() const
  {
    return BW::BWDATA_UnitPrototypeFlags->unit[this->getID()];
  }
  //---------------------------------------------- CAN PRODUCE -----------------------------------------------
  bool UnitType::canProduce() const
  {
    return this->getGroupFlags().getBit(BW::GroupFlags::Factory);
  }
  //-------------------------------------------- GET GROUP FLAGS ---------------------------------------------
  Util::BitMask<BW::GroupFlags::Enum> UnitType::getGroupFlags() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()];
  }
  //--------------------------------------------- GET TILE WIDTH ---------------------------------------------
  u16 UnitType::getTileWidth() const
  {
    return (this->dimensionLeft() + this->dimensionRight() + BW::TILE_SIZE - 1)/BW::TILE_SIZE;
  }
  //-------------------------------------------- GET TILE HEIGHT ---------------------------------------------
  u16 UnitType::getTileHeight() const
  {
    return (this->dimensionUp() + this->dimensionDown() + BW::TILE_SIZE - 1)/BW::TILE_SIZE;
  }
  //------------------------------------------------ IS ZERG -------------------------------------------------
  bool UnitType::isZerg() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()].getBit(BW::GroupFlags::Zerg);
  }
  //----------------------------------------------- IS TERRAN ------------------------------------------------
  bool UnitType::isTerran() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()].getBit(BW::GroupFlags::Terran);
  }
  //----------------------------------------------- IS PROTOSS -----------------------------------------------
  bool UnitType::isProtoss() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()].getBit(BW::GroupFlags::Protoss);
  }
  //----------------------------------------------- IS WORKER ------------------------------------------------
  bool UnitType::isWorker() const
  {
    return BW::BWDATA_UnitPrototypeFlags->unit[this->getID()].getBit(BW::UnitPrototypeFlags::Worker);
  }
  //----------------------------------------------- CAN ATTACK -----------------------------------------------
  bool UnitType::canAttack() const
  {
    return BW::BWDATA_UnitPrototypeFlags->unit[this->getID()].getBit(BW::UnitPrototypeFlags::Attack);
  }
  //------------------------------------------------ CAN MOVE ------------------------------------------------
  bool UnitType::canMove() const
  {
    return BW::BWDATA_PrototypeGroupFlags->unit[this->getID()].getBit(BW::GroupFlags::Men);
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool UnitType::isValid()
  {
    return this->id < 228;
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BW::Race::Enum UnitType::getRace()
  {
    if (this->isZerg())
      return BW::Race::Zerg;
    else if (this->isProtoss())
      return BW::Race::Protoss;
    else if (this->isTerran())
      return BW::Race::Terran;
    else
      return BW::Race::Other;
  }
  //---------------------------------------------- IS BUILDING -----------------------------------------------
  bool UnitType::isBuilding() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::Building);
  }
  //------------------------------------------------ IS ADDON ------------------------------------------------
  bool UnitType::isAddon() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::Addon);
  }
  //----------------------------------------------- IS FLAYER ------------------------------------------------
  bool UnitType::isFlyer() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::Flyer);
  }  
  //----------------------------------------------- IS NEUTRAL -----------------------------------------------
  bool UnitType::isNeutral() const
  {
    return this->getGroupFlags().getBit(BW::GroupFlags::Neutral);
  }  
  //----------------------------------------- IS NEUTRAL ACCESORIES ------------------------------------------
  bool UnitType::isNeutralAccesories() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::NeutralAccessories);
  }
  //----------------------------------------- IS ORGANIC ------------------------------------------
  bool UnitType::isOrganic() const
  {
	  return this->getFlags().getBit(BW::UnitPrototypeFlags::Organicunit);
  }
  //----------------------------------------- IS MECHANACAL ------------------------------------------
  bool UnitType::isMechanical() const
  {
	  return this->getFlags().getBit(BW::UnitPrototypeFlags::Mechanical);
  }
  //--------------------------------------------- WHERE TO BUILD ---------------------------------------------
  BW::UnitType UnitType::whereToBuild()
  {
    switch (this->getID())
    {
      case BW::UnitID::Terran_ComsatStation :
      case BW::UnitID::Terran_NuclearSilo   : return BW::UnitID::Terran_CommandCenter;
      case BW::UnitID::Terran_MachineShop   : return BW::UnitID::Terran_Factory;
      case BW::UnitID::Terran_ControlTower  : return BW::UnitID::Terran_Starport;
      case BW::UnitID::Terran_CovertOps     :
      case BW::UnitID::Terran_PhysicsLab    : return BW::UnitID::Terran_ScienceFacility;
      case BW::UnitID::Zerg_Lair            : return BW::UnitID::Zerg_Hatchery;
      case BW::UnitID::Zerg_Hive            : return BW::UnitID::Zerg_Lair;
      case BW::UnitID::Zerg_SunkenColony    :
      case BW::UnitID::Zerg_SporeColony     : return BW::UnitID::Zerg_CreepColony;
      case BW::UnitID::Zerg_GreaterSpire    : return BW::UnitID::Zerg_Spire;
      default : return BW::UnitID::None;
    }
  }
  static std::map< BW::UnitType, int > requiredUnits[BW::UNIT_TYPE_COUNT];
  const std::map< BW::UnitType,int >& UnitType::getRequiredUnits() const
  {
    return requiredUnits[this->id];
  }
  BW::TechID::Enum UnitType::getRequiredTech() const
  {
    if (this->getID()==BW::UnitID::Zerg_Lurker)
      return BW::TechID::LurkerAspect;
    return BW::TechID::None;
  }
  //------------------------------------------ GET SUPPLY PRODUCED -------------------------------------------
  u16 UnitType::getSupplyProduced()
  {
    return BWDATA_SupplyProduced->unitType[this->getID()];
  }
  //---------------------------------------------- INITIALIZE ------------------------------------------------
  void UnitType::initialize()
  {
    for(int i=0;i<BW::UNIT_TYPE_COUNT;i++)
    {
      switch (i)
      {
        // Terran Basic Buildings
        case BW::UnitID::Terran_CommandCenter :
        case BW::UnitID::Terran_SupplyDepot   :
        case BW::UnitID::Terran_Refinery      :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_SCV),1));
          break;
	      case BW::UnitID::Terran_Barracks :
	      case BW::UnitID::Terran_EngineeringBay :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_SCV),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_CommandCenter),1));
          break;
        case BW::UnitID::Terran_Academy :
        case BW::UnitID::Terran_Bunker  :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_SCV),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Barracks),1));
          break;
        case BW::UnitID::Terran_MissileTurret :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_SCV),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_EngineeringBay),1));
          break;

        // Terran Advanced Buildings
        case BW::UnitID::Terran_Factory :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_SCV),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Barracks),1));
          break;
        case BW::UnitID::Terran_Armory   :
        case BW::UnitID::Terran_Starport :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_SCV),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Factory),1));
          break;
        case BW::UnitID::Terran_ScienceFacility :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_SCV),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Starport),1));
          break;

        // Terran Add-ons
        case BW::UnitID::Terran_ComsatStation :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_CommandCenter),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Academy),1));
          break;
        case BW::UnitID::Terran_NuclearSilo :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_CommandCenter),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_ScienceFacility),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_CovertOps),1));
          break;
        case BW::UnitID::Terran_MachineShop :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Factory),1));
          break;
        case BW::UnitID::Terran_ControlTower :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Starport),1));
          break;
        case BW::UnitID::Terran_PhysicsLab :
        case BW::UnitID::Terran_CovertOps  :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_ScienceFacility),1));
          break;

        // Terran Units
        case BW::UnitID::Terran_SCV :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_CommandCenter),1));
          break;
        case BW::UnitID::Terran_Marine :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Barracks),1));
          break;
        case BW::UnitID::Terran_Firebat :
        case BW::UnitID::Terran_Medic   :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Barracks),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Academy),1));
          break;
        case BW::UnitID::Terran_Ghost :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Barracks),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Academy),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_CovertOps),1));
          break;
        case BW::UnitID::Terran_Vulture :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Factory),1));
          break;
        case BW::UnitID::Terran_SiegeTankSiegeMode :
        case BW::UnitID::Terran_SiegeTankTankMode  :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Factory),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_MachineShop),1));
          break;
        case BW::UnitID::Terran_Goliath :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Factory),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Armory),1));
          break;
        case BW::UnitID::Terran_Wraith :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Starport),1));
          break;
        case BW::UnitID::Terran_Dropship :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Starport),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_ControlTower),1));
          break;
        case BW::UnitID::Terran_Battlecruiser :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Starport),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_ControlTower),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_PhysicsLab),1));
          break;
        case BW::UnitID::Terran_ScienceVessel :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Starport),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_ControlTower),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_ScienceFacility),1));
          break;
        case BW::UnitID::Terran_Valkyrie :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Starport),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_ControlTower),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Terran_Armory),1));
          break;

        // Protoss Basic Buildings
        case BW::UnitID::Protoss_Nexus       :
        case BW::UnitID::Protoss_Pylon       :
        case BW::UnitID::Protoss_Assimilator :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          break;
        case BW::UnitID::Protoss_Gateway :
        case BW::UnitID::Protoss_Forge   :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Nexus),1));
          break;
        case BW::UnitID::Protoss_CyberneticsCore :
        case BW::UnitID::Protoss_ShieldBattery   :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Gateway),1));
          break;
        case BW::UnitID::Protoss_PhotonCannon :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Forge),1));
          break;

        // Protoss Advanced Buildings
        case BW::UnitID::Protoss_RoboticsFacility :
        case BW::UnitID::Protoss_Stargate         :
        case BW::UnitID::Protoss_CitadelOfAdun    :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_CyberneticsCore),1));
          break;
        case BW::UnitID::Protoss_Observatory        :
        case BW::UnitID::Protoss_RoboticsSupportBay :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_RoboticsFacility),1));
          break;
        case BW::UnitID::Protoss_FleetBeacon :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Stargate),1));
          break;
        case BW::UnitID::Protoss_TemplarArchives :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_CitadelOfAdun),1));
          break;
        case BW::UnitID::Protoss_ArbiterTribunal :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Probe),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_TemplarArchives),1));
          break;

        // Protoss Units
        case BW::UnitID::Protoss_Probe :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Nexus),1));
          break;
        case BW::UnitID::Protoss_Zealot :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Gateway),1));
          break;
        case BW::UnitID::Protoss_Dragoon :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Gateway),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_CyberneticsCore),1));
          break;
        case BW::UnitID::Protoss_HighTemplar : 
        case BW::UnitID::Protoss_DarkTemplar :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Gateway),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_TemplarArchives),1));
          break;
        case BW::UnitID::Protoss_Archon :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_HighTemplar),2));
          break;
        case BW::UnitID::Protoss_DarkArchon :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_DarkTemplar),2));
          break;
        case BW::UnitID::Protoss_Shuttle :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_RoboticsFacility),1));
          break;
        case BW::UnitID::Protoss_Reaver :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_RoboticsFacility),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_RoboticsSupportBay),1));
          break;
        case BW::UnitID::Protoss_Observer :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_RoboticsFacility),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Observatory),1));
          break;
        case BW::UnitID::Protoss_Scout :
        case BW::UnitID::Protoss_Corsair :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Stargate),1));
          break;
        case BW::UnitID::Protoss_Carrier :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Stargate),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_FleetBeacon),1));
          break;
        case BW::UnitID::Protoss_Arbiter :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_Stargate),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Protoss_ArbiterTribunal),1));
          break;

        // Zerg Basic Buildings
        case BW::UnitID::Zerg_Hatchery    :
        case BW::UnitID::Zerg_CreepColony :
        case BW::UnitID::Zerg_Extractor   :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Drone),1));
          break;
        case BW::UnitID::Zerg_SpawningPool     :
        case BW::UnitID::Zerg_EvolutionChamber :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Drone),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Hatchery),1));
          break;
        case BW::UnitID::Zerg_HydraliskDen :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Drone),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_SpawningPool),1));
          break;

        // Zerg Advanced Buildings
        case BW::UnitID::Zerg_Spire      :
        case BW::UnitID::Zerg_QueensNest :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Lair),1));
          break;
        case BW::UnitID::Zerg_DefilerMound    :
        case BW::UnitID::Zerg_NydusCanal      :
        case BW::UnitID::Zerg_UltraliskCavern :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Hive),1));
          break;

        // Zerg Mutated Buildings
        case BW::UnitID::Zerg_Lair :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Hatchery),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_SpawningPool),1));
          break;
        case BW::UnitID::Zerg_Hive :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Lair),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_QueensNest),1));
          break;
        case BW::UnitID::Zerg_SunkenColony :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_CreepColony),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_SpawningPool),1));
          break;
        case BW::UnitID::Zerg_SporeColony :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_CreepColony),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_EvolutionChamber),1));
          break;
        case BW::UnitID::Zerg_GreaterSpire :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Spire),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Hive),1));
          break;

        // Zerg Units
        case BW::UnitID::Zerg_Larva :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Hatchery),1));
          break;
        case BW::UnitID::Zerg_Drone    :
        case BW::UnitID::Zerg_Overlord :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Larva),1));
          break;
        case BW::UnitID::Zerg_Zergling :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Larva),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_SpawningPool),1));
          break;
        case BW::UnitID::Zerg_Hydralisk :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Larva),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_HydraliskDen),1));
          break;
        case BW::UnitID::Zerg_Scourge  :
        case BW::UnitID::Zerg_Mutalisk :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Larva),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Spire),1));
          break;
        case BW::UnitID::Zerg_Devourer :
        case BW::UnitID::Zerg_Guardian :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Mutalisk),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_GreaterSpire),1));
          break;
        case BW::UnitID::Zerg_Queen :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Larva),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_QueensNest),1));
          break;
        case BW::UnitID::Zerg_Defiler :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Larva),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_DefilerMound),1));
          break;
        case BW::UnitID::Zerg_Ultralisk :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Larva),1));
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_UltraliskCavern),1));
          break;
        case BW::UnitID::Zerg_InfestedTerran :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_InfestedCommandCenter),1));
          break;
        case BW::UnitID::Zerg_Lurker :
          requiredUnits[i].insert(std::make_pair(BW::UnitType(BW::UnitID::Zerg_Hydralisk),1));
          break;
      }
    }
  }
  //----------------------------------------------------------------------------------------------------------
}