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
    if (!executors[0]->_exists) return;
    if (!executors[0]->getType().canMove()) return;
    switch (executors[0]->getType().getID())
    {
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        executors[0]->self->order = BW::OrderID::CarrierHoldPosition;
        break;
      case BW::UnitID::Zerg_Queen:
      case BW::UnitID::Zerg_Hero_Matriarch:
        executors[0]->self->order = BW::OrderID::QueenHoldPosition;
        break;
      case BW::UnitID::Zerg_InfestedTerran:
      case BW::UnitID::Zerg_Scourge:
        executors[0]->self->order = BW::OrderID::SuicideHoldPosition;
        break;
      case BW::UnitID::Terran_Medic:
        executors[0]->self->order = BW::OrderID::MedicHoldPosition;
        break;
      case BW::UnitID::Protoss_Reaver:
        executors[0]->self->order = BW::OrderID::ReaverHoldPosition;
        break;
      default:
        executors[0]->self->order = BW::OrderID::HoldPosition;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandHoldPosition::getType()
  {
    return BWAPI::CommandTypes::HoldPosition;
  }
  //----------------------------------------------------------------------------------------------------------
};
