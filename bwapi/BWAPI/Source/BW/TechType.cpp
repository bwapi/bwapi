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
     return (*BW::BWXFN_StringTable + (*BW::BWXFN_StringTableIndex)[BW::BWXFN_TechLabelIndex[this->getID()]]);
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
      case BW::TechID::Stimpacks          :
      case BW::TechID::OpticalFlare       :
      case BW::TechID::Restoration        : return BW::UnitID::Terran_Academy;
      case BW::TechID::Lockdown           :
      case BW::TechID::PersonnelCloaking  : return BW::UnitID::Terran_CovertOps;
      case BW::TechID::EMPShockwave       :
      case BW::TechID::Irradiate          : return BW::UnitID::Terran_ScienceFacility;
      case BW::TechID::SpiderMines        :
      case BW::TechID::TankSiegeMode      : return BW::UnitID::Terran_MachineShop;
      case BW::TechID::YamatoGun          : return BW::UnitID::Terran_PhysicsLab;
      case BW::TechID::CloakingField      : return BW::UnitID::Terran_ControlTower;
      
      case BW::TechID::Burrowing          : return BW::UnitID::Zerg_Hatchery;
      case BW::TechID::LurkerAspect       : return BW::UnitID::Zerg_HydraliskDen;
      case BW::TechID::Ensnare            :
      case BW::TechID::Parasite           :
      case BW::TechID::SpawnBroodlings    : return BW::UnitID::Zerg_QueensNest;
      case BW::TechID::Plague             :
      case BW::TechID::Consume            : return BW::UnitID::Zerg_DefilerMound;
      
      case BW::TechID::PsionicStorm       :
      case BW::TechID::Hallucination      :
      case BW::TechID::MindControl        :
      case BW::TechID::Maelstorm          : return BW::UnitID::Protoss_TemplarArchives;
      case BW::TechID::Recall             :
      case BW::TechID::StasisField        : return BW::UnitID::Protoss_ArbiterTribunal;
      case BW::TechID::DisruptionWeb      : return BW::UnitID::Protoss_FleetBeacon;
      
      default : return BW::UnitID::None;
    }
  }
  //------------------------------------------------------------------------------
}