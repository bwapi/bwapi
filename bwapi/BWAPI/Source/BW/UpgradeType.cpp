#include "UpgradeType.h"
#include "UnitTypeID.h"

#include "Offsets.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UpgradeType::UpgradeType()
    : id(BW::UpgradeIDs::None)
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
    if (this->getID() == BW::UpgradeIDs::None)
      return "None";
    else if (this->getID() < 60)
      return (*BW::BWDATA_StringTable + (*BW::BWDATA_StringTableIndex)[BW::BWDATA_UpgradeLabelIndex[this->getID()]]);
    else
      return "Invalid";
  }
  //----------------------------------------------------------------------------------------------------------
  bool UpgradeType::isValid() const
  {
    return this->getID()  < UpgradeIDs::UnusedUpgrade55 &&
           this->getID() != UpgradeIDs::UnusedUpgrade45 &&
           this->getID() != UpgradeIDs::UnusedUpgrade46 &&
           this->getID() != UpgradeIDs::UnusedUpgrade48 &&
           this->getID() != UpgradeIDs::UnusedUpgrade50 &&
           this->getID() != UpgradeIDs::BurstLasers;
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
      case BW::UpgradeIDs::TerranInfantryWeapons :
      case BW::UpgradeIDs::TerranInfantryArmor   : return BW::UnitTypeIDs::Terran_EngineeringBay;
      case BW::UpgradeIDs::TerranVehicleWeapons  :
      case BW::UpgradeIDs::TerranVehiclePlating  :
      case BW::UpgradeIDs::TerranShipWeapons     :
      case BW::UpgradeIDs::TerranShipPlating     : return BW::UnitTypeIDs::Terran_Armory;
      case BW::UpgradeIDs::CaduceusReactor       :
      case BW::UpgradeIDs::U_238Shells           : return BW::UnitTypeIDs::Terran_Academy;
      case BW::UpgradeIDs::CharonBooster         :
      case BW::UpgradeIDs::IonThrusters          : return BW::UnitTypeIDs::Terran_MachineShop;
      case BW::UpgradeIDs::TitanReactor          : return BW::UnitTypeIDs::Terran_ScienceFacility;
      case BW::UpgradeIDs::MoebiusReactor        :
      case BW::UpgradeIDs::OcularImplants        : return BW::UnitTypeIDs::Terran_CovertOps;
      case BW::UpgradeIDs::ApolloReactor         : return BW::UnitTypeIDs::Terran_ControlTower;
      case BW::UpgradeIDs::ColossusReactor       : return BW::UnitTypeIDs::Terran_PhysicsLab;

      case BW::UpgradeIDs::ZergMeleeAttacks      :
      case BW::UpgradeIDs::ZergMissileAttacks    :
      case BW::UpgradeIDs::ZergCarapace          : return BW::UnitTypeIDs::Zerg_EvolutionChamber;
      case BW::UpgradeIDs::ZergFlyerAttacks      :
      case BW::UpgradeIDs::ZergFlyerCaparace     : return BW::UnitTypeIDs::Zerg_Spire;
      case BW::UpgradeIDs::PneumatizedCarapace   :
      case BW::UpgradeIDs::Antennae              :
      case BW::UpgradeIDs::VentralSacs           : return BW::UnitTypeIDs::Zerg_Lair;
      case BW::UpgradeIDs::AdrenalGlands         :
      case BW::UpgradeIDs::MetabolicBoost        : return BW::UnitTypeIDs::Zerg_SpawningPool;
      case BW::UpgradeIDs::MuscularAugments      :
      case BW::UpgradeIDs::GroovedSpines         : return BW::UnitTypeIDs::Zerg_Hydralisk;
      case BW::UpgradeIDs::GameteMeiosis         : return BW::UnitTypeIDs::Zerg_QueensNest;
      case BW::UpgradeIDs::MetasynapticNode      : return BW::UnitTypeIDs::Zerg_DefilerMound;
      case BW::UpgradeIDs::AnabolicSynthesis     :
      case BW::UpgradeIDs::ChitinousPlating      : return BW::UnitTypeIDs::Zerg_UltraliskCavern;

      case BW::UpgradeIDs::ProtossArmor          :
      case BW::UpgradeIDs::ProtossGroundWeapons  :
      case BW::UpgradeIDs::ProtossPlasmaShields  : return BW::UnitTypeIDs::Protoss_Forge;
      case BW::UpgradeIDs::SingularityCharge     :
      case BW::UpgradeIDs::ProtossAirWeapons     :
      case BW::UpgradeIDs::ProtossPlating        : return BW::UnitTypeIDs::Protoss_CyberneticsCore;
      case BW::UpgradeIDs::LegEnhancements       : return BW::UnitTypeIDs::Protoss_CitadelOfAdun;
      case BW::UpgradeIDs::GraviticDrive         :
      case BW::UpgradeIDs::ReaverCapacity        :
      case BW::UpgradeIDs::ScarabDamage          : return BW::UnitTypeIDs::Protoss_RoboticsSupportBay;
      case BW::UpgradeIDs::GraviticBoosters      :
      case BW::UpgradeIDs::SensorArray           : return BW::UnitTypeIDs::Protoss_Observatory;
      case BW::UpgradeIDs::ArgusTalisman         :
      case BW::UpgradeIDs::ApialSensors          :
      case BW::UpgradeIDs::KhaydarinAmulet       : return BW::UnitTypeIDs::Protoss_TemplarArchives;
      case BW::UpgradeIDs::ArgusJewel            :
      case BW::UpgradeIDs::CarrierCapacity       :
      case BW::UpgradeIDs::GraviticThrusters     : return BW::UnitTypeIDs::Protoss_FleetBeacon;
      case BW::UpgradeIDs::KhaydarinCore         : return BW::UnitTypeIDs::Protoss_ArbiterTribunal;

      default : return BW::UnitTypeIDs::None;
    }
  }
};
