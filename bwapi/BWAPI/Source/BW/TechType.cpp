#include "TechType.h"

#include "Offsets.h"

namespace BW
{
 //------------------------------ CONSTRUCTOR ----------------------------------
  TechType::TechType()
  :id(BW::TechID::None)
  {
  }
  //------------------------------ CONSTRUCTOR ----------------------------------
  TechType::TechType(const BW::TechID::Enum& id)
  :id(id)
  {
  }
  //------------------------------ OPERATOR == ----------------------------------
  bool TechType::operator ==(const BW::TechID::Enum& id)
  {
    return id == this->id;
  }
  //------------------------------ OPERATOR == ----------------------------------
  bool TechType::operator !=(const BW::TechID::Enum& id)
  {
    return id != this->id;
  }
  //------------------------------ OPERATOR == ----------------------------------
  bool TechType::operator ==(const TechType& type)
  {
    return this->id == type.id;
  }  
  //------------------------------- GET NAME ------------------------------------
  const char* TechType::getName() const
  {
     return (*BW::BWXFN_StingTable + (*BW::BWXFN_StringTableIndex)[BW::BWXFN_TechLabelIndex[this->getID()]]);
  }
  //------------------------------- GET ID --------------------------------------
  TechID::Enum TechType::getID() const
  {
    return this->id;
  }
  //--------------------------------- IS VALID ----------------------------------
  bool TechType::isValid() const
  {
    if (id >= BW::TechID::UnusedTech35 ||
        id == BW::TechID::UnusedTech33 ||
        id == BW::TechID::UnusedTech26)
      return false;
    else
      return true;
  }
  //-------------------------------- GET MINERAL PRICE --------------------------
  u16 TechType::getMineralPrice()
  {
    return BW::BWXFN_TechMineralCost->tech[this->getID()];
  }
  //---------------------------------- GET GAS PRICE ----------------------------
  u16 TechType::getGasPrice()
  {
    return BW::BWXFN_TechGasCost->tech[this->getID()];
  }
  //--------------------------------- GET ENERGY PRICE --------------------------
  u16 TechType::getEnergyPrice()
  {
    return BW::BWXFN_TechEnergyCost->tech[this->getID()];
  }
  //--------------------------------- WHERE TO INVENT ---------------------------
  UnitType TechType::whereToInvent()
  {
    switch (this->getID())
    {
      case BW::TechID::Stimpacks          : return BW::UnitID::Terran_Academy;
      case BW::TechID::Lockdown           : return BW::UnitID::Terran_CovertOps;
      case BW::TechID::PersonnelCloaking  : return BW::UnitID::Terran_CovertOps;
      case BW::TechID::EMPShockwave       : return BW::UnitID::Terran_ScienceFacility;
      default : return BW::UnitID::None;
    }
  }
  //------------------------------------------------------------------------------
}