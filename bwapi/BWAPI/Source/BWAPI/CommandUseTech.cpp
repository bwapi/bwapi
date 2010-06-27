#include "CommandUseTech.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUseTech::CommandUseTech(UnitImpl* executor, BWAPI::TechType tech)
    : Command(executor), tech(tech), targetPosition(Positions::None), targetUnit(NULL)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUseTech::CommandUseTech(UnitImpl* executor, BWAPI::TechType tech, BWAPI::Position targetPosition)
    : Command(executor), tech(tech), targetPosition(targetPosition), targetUnit(NULL)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUseTech::CommandUseTech(UnitImpl* executor, BWAPI::TechType tech, UnitImpl* targetUnit)
    : Command(executor), tech(tech), targetPosition(Positions::None), targetUnit(targetUnit)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUseTech::execute()
  {
    if (!executors[0]->_exists) return;
    switch(tech.getID())
    {
      case BW::TechID::Stimpacks:
        executors[0]->getRawDataLocal->hitPoints-=10*256;
        executors[0]->getRawDataLocal->stimTimer=38;
        executors[0]->getRawDataLocal->energy-=(u16)(tech.energyUsed()*256);
        break;
      case BW::TechID::DarkSwarm:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::DarkSwarm;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::DisruptionWeb:
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::EMPShockwave:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::EmpShockwave;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::Ensnare:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::Ensnare;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::Maelstorm:
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::NuclearStrike:
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::Plague:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::Plague;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::PsionicStorm:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::PsiStorm;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::Recall:
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::ScannerSweep:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::Scanner;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::SpiderMines:
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetPosition.x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetPosition.y();
        break;
      case BW::TechID::StasisField:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::StasisField;
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Consume:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::Consume;
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::DefensiveMatrix:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::DefensiveMatrix;
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Feedback:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Hallucination:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::Hallucination1;
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Healing:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::HealMove;
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Infestation:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::InfestMine1;
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Irradiate:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::Irradiate;
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Lockdown:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::MindControl:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::OpticalFlare:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Parasite:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::Restoration:
        executors[0]->getRawDataLocal->orderID=BW::OrderID::Restoration;
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::SpawnBroodlings:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::YamatoGun:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::ArchonWarp:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      case BW::TechID::DarkArchonMeld:
        executors[0]->getRawDataLocal->orderTargetUnit=targetUnit->getOriginalRawData;
        executors[0]->getRawDataLocal->orderTargetPos.x=(u16)targetUnit->getPosition().x();
        executors[0]->getRawDataLocal->orderTargetPos.y=(u16)targetUnit->getPosition().y();
        break;
      default:
        break;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUseTech::getType()
  {
    return BWAPI::CommandTypes::UseTech;
  }
  //----------------------------------------------------------------------------------------------------------
};
