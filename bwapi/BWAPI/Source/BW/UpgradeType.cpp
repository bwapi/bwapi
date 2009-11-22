#include "UpgradeType.h"

#include "Offsets.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UpgradeType::UpgradeType()
      : id(BW::UpgradeID::None)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UpgradeType::UpgradeType(const u8& id)
      : id(id)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UpgradeType::operator == (const u8& id) const
  {
    return this->id == id;
  }
  //------------------------------------------------ GET ID  -------------------------------------------------
  u8 UpgradeType::getID() const
  {
    return this->id;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  const char* UpgradeType::getName() const
  {
    if (this->getID() == BW::UpgradeID::None)
      return "None";
    else if (this->getID() < 60)
      return (*BW::BWDATA_StringTable + (*BW::BWDATA_StringTableIndex)[BW::BWDATA_UpgradeLabelIndex[this->getID()]]);
    else
      return "Invalid";
  }
  //----------------------------------------------------------------------------------------------------------
  bool UpgradeType::isValid() const
  {
    return this->getID()  < UpgradeID::UnusedUpgrade55 &&
           this->getID() != UpgradeID::UnusedUpgrade45 &&
           this->getID() != UpgradeID::UnusedUpgrade46 &&
           this->getID() != UpgradeID::UnusedUpgrade48 &&
           this->getID() != UpgradeID::UnusedUpgrade50 &&
           this->getID() != UpgradeID::BurstLasers;
  }
  //---------------------------------------------- UPGRDES MAX -----------------------------------------------
  u8 UpgradeType::upgradesMax() const
  {
    return BW::BWDATA_UpgradeMax[this->getID()];
  }
  //------------------------------------------- MINERAL COST BASE --------------------------------------------
  u16 UpgradeType::mineralCostBase() const
  {
    return BW::BWDATA_UpgradeMineralCostBase[this->getID()];
  }
  //------------------------------------------ MINERAL COST FACTOR -------------------------------------------
  u16 UpgradeType::mineralCostFactor() const
  {
    return BW::BWDATA_UpgradeMineralCostFactor[this->getID()];
  }
  //---------------- GAS COST BASE ---------------------------------------------------       -----------------
  u16 UpgradeType::gasCostBase() const
  {
    return BW::BWDATA_UpgradeGasCostBase[this->getID()];
  }
  //-------------------------------------------- GAS COST FACTOR ---------------------------------------------
  u16 UpgradeType::gasCostFactor() const
  {
    return BW::BWDATA_UpgradeGasCostFactor[this->getID()];
  }
  //--------------------------------------------- TIME COST BASE ---------------------------------------------
  u16 UpgradeType::timeCostBase() const
  {
    return BW::BWDATA_UpgradeTimeCostBase[this->getID()];
  }
  //-------------------------------------------- TIME COST FACTOR --------------------------------------------
  u16 UpgradeType::timeCostFactor() const
  {
    return BW::BWDATA_UpgradeTimeCostFactor[this->getID()];
  }
  //-------------------------------------------------- RACE --------------------------------------------------
  u8 UpgradeType::race() const
  {
    return BW::BWDATA_UpgradeRace[this->getID()];
  }
  //-------------------------------------------- WHERE TO UPGRADE --------------------------------------------
  UnitType UpgradeType::whereToUpgrade()
  {
    switch (this->getID())
    {
      case BW::UpgradeID::TerranInfantryWeapons :
      case BW::UpgradeID::TerranInfantryArmor   : return BW::UnitID::Terran_EngineeringBay;
      case BW::UpgradeID::TerranVehicleWeapons  :
      case BW::UpgradeID::TerranVehiclePlating  :
      case BW::UpgradeID::TerranShipWeapons     :
      case BW::UpgradeID::TerranShipPlating     : return BW::UnitID::Terran_Armory;
      case BW::UpgradeID::CaduceusReactor       :
      case BW::UpgradeID::U_238Shells           : return BW::UnitID::Terran_Academy;
      case BW::UpgradeID::CharonBooster         :
      case BW::UpgradeID::IonThrusters          : return BW::UnitID::Terran_MachineShop;
      case BW::UpgradeID::TitanReactor          : return BW::UnitID::Terran_ScienceFacility;
      case BW::UpgradeID::MoebiusReactor        :
      case BW::UpgradeID::OcularImplants        : return BW::UnitID::Terran_CovertOps;
      case BW::UpgradeID::ApolloReactor         : return BW::UnitID::Terran_ControlTower;
      case BW::UpgradeID::ColossusReactor       : return BW::UnitID::Terran_PhysicsLab;

      case BW::UpgradeID::ZergMeleeAttacks      :
      case BW::UpgradeID::ZergMissileAttacks    :
      case BW::UpgradeID::ZergCarapace          : return BW::UnitID::Zerg_EvolutionChamber;
      case BW::UpgradeID::ZergFlyerAttacks      :
      case BW::UpgradeID::ZergFlyerCaparace     : return BW::UnitID::Zerg_Spire;
      case BW::UpgradeID::PneumatizedCarapace   :
      case BW::UpgradeID::Antennae              :
      case BW::UpgradeID::VentralSacs           : return BW::UnitID::Zerg_Lair;
      case BW::UpgradeID::AdrenalGlands         :
      case BW::UpgradeID::MetabolicBoost        : return BW::UnitID::Zerg_SpawningPool;
      case BW::UpgradeID::MuscularAugments      :
      case BW::UpgradeID::GroovedSpines         : return BW::UnitID::Zerg_Hydralisk;
      case BW::UpgradeID::GameteMeiosis         : return BW::UnitID::Zerg_QueensNest;
      case BW::UpgradeID::MetasynapticNode      : return BW::UnitID::Zerg_DefilerMound;
      case BW::UpgradeID::AnabolicSynthesis     :
      case BW::UpgradeID::ChitinousPlating      : return BW::UnitID::Zerg_UltraliskCavern;

      case BW::UpgradeID::ProtossArmor          :
      case BW::UpgradeID::ProtossGroundWeapons  :
      case BW::UpgradeID::ProtossPlasmaShields  : return BW::UnitID::Protoss_Forge;
      case BW::UpgradeID::SingularityCharge     :
      case BW::UpgradeID::ProtossAirWeapons     :
      case BW::UpgradeID::ProtossPlating        : return BW::UnitID::Protoss_CyberneticsCore;
      case BW::UpgradeID::LegEnhancements       : return BW::UnitID::Protoss_CitadelOfAdun;
      case BW::UpgradeID::GraviticDrive         :
      case BW::UpgradeID::ReaverCapacity        :
      case BW::UpgradeID::ScarabDamage          : return BW::UnitID::Protoss_RoboticsSupportBay;
      case BW::UpgradeID::GraviticBoosters      :
      case BW::UpgradeID::SensorArray           : return BW::UnitID::Protoss_Observatory;
      case BW::UpgradeID::ArgusTalisman         :
      case BW::UpgradeID::ApialSensors          :
      case BW::UpgradeID::KhaydarinAmulet       : return BW::UnitID::Protoss_TemplarArchives;
      case BW::UpgradeID::ArgusJewel            :
      case BW::UpgradeID::CarrierCapacity       :
      case BW::UpgradeID::GraviticThrusters     : return BW::UnitID::Protoss_FleetBeacon;
      case BW::UpgradeID::KhaydarinCore         : return BW::UnitID::Protoss_ArbiterTribunal;

      default : return BW::UnitID::None;
    }
  }
}