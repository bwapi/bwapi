#include "CommandHoldPosition.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandHoldPosition::CommandHoldPosition(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandHoldPosition::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        switch (this->executors[i]->getBWType().getID())
        {
          case BW::UnitID::Protoss_Carrier:
          case BW::UnitID::Protoss_Hero_Gantrithor:
            this->executors[i]->getRawDataLocal->orderID = BW::OrderID::CarrierHoldPosition;
            break;
          case BW::UnitID::Zerg_Queen:
          case BW::UnitID::Zerg_Hero_Matriarch:
            this->executors[i]->getRawDataLocal->orderID = BW::OrderID::QueenHoldPosition;
            break;
          case BW::UnitID::Zerg_InfestedTerran:
          case BW::UnitID::Zerg_Scourge:
            this->executors[i]->getRawDataLocal->orderID = BW::OrderID::SuicideHoldPosition;
            break;
          case BW::UnitID::Terran_Medic:
            this->executors[i]->getRawDataLocal->orderID = BW::OrderID::MedicHoldPosition;
            break;
          case BW::UnitID::Protoss_Reaver:
            this->executors[i]->getRawDataLocal->orderID = BW::OrderID::ReaverHoldPosition;
            break;
          default:
            this->executors[i]->getRawDataLocal->orderID = BW::OrderID::HoldPosition;
        }
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandHoldPosition::getType()
  {
    return BWAPI::CommandTypes::HoldPosition;
  }
  //----------------------------------------------------------------------------------------------------------
};
