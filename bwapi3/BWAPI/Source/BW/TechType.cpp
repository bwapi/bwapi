#include "TechType.h"
#include "Offsets.h"
#include "UnitTypeID.h"

namespace BW
{
//----------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TechType::TechType()
      : id(BW::TechIDs::None)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TechType::TechType(const u8& id)
      : id(id)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool TechType::operator ==(const u8& id)
  {
    return id == this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool TechType::operator !=(const u8& id)
  {
    return id != this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool TechType::operator ==(const TechType& type)
  {
    return this->id == type.id;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  const char* TechType::getName() const
  {
    if (this->getID() == BW::TechIDs::None)
      return "None";
    else if (this->getID() < 44)
      return (*BW::BWDATA_StringTable + (*BW::BWDATA_StringTableIndex)[BW::BWDATA_TechLabelIndex[this->getID()]]);
    else
      return "Invalid";
  }
  //------------------------------------------------- GET ID -------------------------------------------------
  u8 TechType::getID() const
  {
    return this->id;
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool TechType::isValid() const
  {
    if (id >= BW::TechIDs::UnusedTech35 ||
        id == BW::TechIDs::UnusedTech33 ||
        id == BW::TechIDs::UnusedTech26)
      return false;
    return true;
  }
  //------------------------------------------- GET MINERAL PRICE --------------------------------------------
  u16 TechType::getMineralPrice()
  {
    return BW::BWDATA_TechMineralCost->tech[this->getID()];
  }
  //--------------------------------------------- GET GAS PRICE ----------------------------------------------
  u16 TechType::getGasPrice()
  {
    return BW::BWDATA_TechGasCost->tech[this->getID()];
  }
  //-------------------------------------------- GET ENERGY PRICE --------------------------------------------
  u16 TechType::getEnergyPrice()
  {
    return BW::BWDATA_TechEnergyCost->tech[this->getID()];
  }
  //-------------------------------------------- WHERE TO INVENT ---------------------------------------------
  UnitType TechType::whereToInvent()
  {
    switch (this->getID())
    {
      case BW::TechIDs::Stimpacks          :
      case BW::TechIDs::OpticalFlare       :
      case BW::TechIDs::Restoration        : return BW::UnitTypeIDs::Terran_Academy;
      case BW::TechIDs::Lockdown           :
      case BW::TechIDs::PersonnelCloaking  : return BW::UnitTypeIDs::Terran_CovertOps;
      case BW::TechIDs::EMPShockwave       :
      case BW::TechIDs::Irradiate          : return BW::UnitTypeIDs::Terran_ScienceFacility;
      case BW::TechIDs::SpiderMines        :
      case BW::TechIDs::TankSiegeMode      : return BW::UnitTypeIDs::Terran_MachineShop;
      case BW::TechIDs::YamatoGun          : return BW::UnitTypeIDs::Terran_PhysicsLab;
      case BW::TechIDs::CloakingField      : return BW::UnitTypeIDs::Terran_ControlTower;

      case BW::TechIDs::Burrowing          : return BW::UnitTypeIDs::Zerg_Hatchery;
      case BW::TechIDs::LurkerAspect       : return BW::UnitTypeIDs::Zerg_HydraliskDen;
      case BW::TechIDs::Ensnare            :
      case BW::TechIDs::Parasite           :
      case BW::TechIDs::SpawnBroodlings    : return BW::UnitTypeIDs::Zerg_QueensNest;
      case BW::TechIDs::Plague             :
      case BW::TechIDs::Consume            : return BW::UnitTypeIDs::Zerg_DefilerMound;

      case BW::TechIDs::PsionicStorm       :
      case BW::TechIDs::Hallucination      :
      case BW::TechIDs::MindControl        :
      case BW::TechIDs::Maelstorm          : return BW::UnitTypeIDs::Protoss_TemplarArchives;
      case BW::TechIDs::Recall             :
      case BW::TechIDs::StasisField        : return BW::UnitTypeIDs::Protoss_ArbiterTribunal;
      case BW::TechIDs::DisruptionWeb      : return BW::UnitTypeIDs::Protoss_FleetBeacon;

      default : return BW::UnitTypeIDs::None;
    }
  }
  //----------------------------------------------------------------------------------------------------------
};
