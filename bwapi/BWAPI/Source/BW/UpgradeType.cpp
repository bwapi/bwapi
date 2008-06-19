#include "UpgradeType.h"

#include "Offsets.h"

namespace BW
{
  //---------------------------------------- CONSTRUCTOR -----------------------------------------------------
  UpgradeType::UpgradeType()
  :id(BW::UpgradeID::None)
  {}
  //---------------------------------------- CONSTRUCTOR -----------------------------------------------------
  UpgradeType::UpgradeType(const UpgradeID::Enum& id)
  :id(id)
  {}
  //---------------------------------------- OPERATOR == -----------------------------------------------------
  bool UpgradeType::operator == (const UpgradeID::Enum& id) const
  {
    return this->id == id;
  }
  //------------------------------------------ GET ID  ------------------------------------------------------
  UpgradeID::Enum UpgradeType::getID() const
  {
    return this->id;
  }
  //----------------------------------------- GET NAME -------------------------------------------------------
  const char* UpgradeType::getName() const
  {
    return (*BW::BWXFN_StringTable + (*BW::BWXFN_StringTableIndex)[BW::BWXFN_UpgradeLabelIndex[this->getID()]]);
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
  //---------------------------------------- UPGRDES MAX -----------------------------------------------------
  u8 UpgradeType::upgradesMax() const
  {
    return BW::BWXFN_UpgradeMax[this->getID()];
  }
  //--------------------------------------- MINERAL COST BASE ------------------------------------------------
  u16 UpgradeType::mineralCostBase() const
  {
    return BW::BWXFN_UpgradeMineralCostBase[this->getID()];
  }
  //--------------------------------------- MINERAL COST FACTOR ----------------------------------------------
  u16 UpgradeType::mineralCostFactor() const
  {
    return BW::BWXFN_UpgradeMineralCostFactor[this->getID()];
  }
  //---------------------------------------- GAS COST BASE ---------------------------------------------------        
  u16 UpgradeType::gasCostBase() const
  {
    return BW::BWXFN_UpgradeGasCostBase[this->getID()];
  }
  //--------------------------------------- GAS COST FACTOR --------------------------------------------------
  u16 UpgradeType::gasCostFactor() const
  {
    return BW::BWXFN_UpgradeGasCostFactor[this->getID()];
  }
  //---------------------------------------- TIME COST BASE --------------------------------------------------
  u16 UpgradeType::timeCostBase() const
  {
    return BW::BWXFN_UpgradeTimeCostBase[this->getID()];
  }
  //---------------------------------------- TIME COST FACTOR ------------------------------------------------
  u16 UpgradeType::timeCostFactor() const
  {
    return BW::BWXFN_UpgradeTimeCostFactor[this->getID()];
  }
  //--------------------------------------------- RACE -------------------------------------------------------
  Race::Enum UpgradeType::race() const
  {
    return BW::BWXFN_UpgradeRace[this->getID()];
  }
}