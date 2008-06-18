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
  //----------------------------------------------------------------------------------------------------------
}