#include "GameImpl.h"
#include "../DLLMain.h"

#include "Command.h"
#include "UnitImpl.h"

#include <BWAPI/TechType.h>
#include <BWAPI/UnitType.h>

#include "../BW/OrderTypes.h"

namespace BWAPI4
{
  //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
  void GameImpl::addToCommandBuffer(Command command)
  {
    std::move(command).insertIntoCommandBuffer(this->commandBuffer);
  }
  //----------------------------------------- APPLY LATENCY COMPENSATION
  void GameImpl::applyLatencyCompensation()
  {
    // Remove the current frame from the buffer and execute the current frame
    // (only some actions execute on the current frame, like resource reserving)
    if (!this->commandBuffer.empty())
    {
      for (auto &command : this->commandBuffer.front())
      {
        command.execute(true);
      }
      this->commandBuffer.erase(std::begin(this->commandBuffer));
    }

    // Apply latency compensation
    for (auto buf = 0; buf < getRemainingLatencyFrames()
                    && buf < static_cast<int>(this->commandBuffer.size()); ++buf)
    {
      for (auto &command : this->commandBuffer[buf])
      {
        command.execute();
      }
    }

    // Prepare buffer space for new commands
    if (this->commandBuffer.size() < static_cast<size_t>(getRemainingLatencyFrames() + 15))
    {
      this->commandBuffer.resize(getRemainingLatencyFrames() + 15);
    }
  }

  //--------------------------------------------- EXECUTE COMMAND --------------------------------------------
  void GameImpl::executeCommand(UnitCommand command)
  {
    apmCounter.addNoSelect();

    UnitCommandType ct = command.type;
    bool queued = command.isQueued();
    if (ct == UnitCommandTypes::Attack_Move)
    {
      if ( command.unit && command.unit->getType() == UnitTypes::Zerg_Infested_Terran )
        QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, Order(Orders::Enum::Attack1), queued);
      else
        QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, Order(Orders::Enum::AttackMove), queued);
    }
    else if (ct == UnitCommandTypes::Attack_Unit)
    {
      UnitType ut      = command.unit ? command.unit->getType() : UnitTypes::None;
      if ( ut == UnitTypes::Protoss_Carrier || ut == UnitTypes::Hero_Gantrithor )
        QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::CarrierAttack), queued);
      else if ( ut == UnitTypes::Protoss_Reaver || ut == UnitTypes::Hero_Warbringer )
        QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::ReaverAttack), queued);
      else if ( ut.isBuilding() )
        QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::TowerAttack), queued);
      else
        QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::Attack1), queued);
    }
    else if (ct == UnitCommandTypes::Build)
    {
      UnitType extraType(command.extra);
      if ( command.unit && command.unit->getType() == BWAPI::UnitTypes::Zerg_Nydus_Canal &&
           extraType == UnitTypes::Zerg_Nydus_Canal )
        QUEUE_COMMAND(BW::Orders::MakeNydusExit, command.x, command.y);
      else if ( extraType.isAddon() )
        QUEUE_COMMAND(BW::Orders::MakeAddon, command.x, command.y, extraType);
      else
        QUEUE_COMMAND(BW::Orders::MakeBuilding, command.x, command.y, extraType);
    }
    else if ( ct == UnitCommandTypes::Build_Addon && command.unit )
    {
      TilePosition target = command.unit->getTilePosition() + TilePosition(4, 1);
      QUEUE_COMMAND(BW::Orders::MakeAddon, BW::TilePosition(makeValid(target)), command.getUnitType());
    }
    else if ( ct == UnitCommandTypes::Train )
    {
      UnitType type1(command.extra);
      switch ( command.unit ? command.unit->getType() : UnitTypes::None )
      {
      case UnitTypes::Enum::Zerg_Larva:
      case UnitTypes::Enum::Zerg_Mutalisk:
      case UnitTypes::Enum::Zerg_Hydralisk:
        QUEUE_COMMAND(BW::Orders::UnitMorph, type1);
        break;
      case UnitTypes::Enum::Zerg_Hatchery:
      case UnitTypes::Enum::Zerg_Lair:
      case UnitTypes::Enum::Zerg_Spire:
      case UnitTypes::Enum::Zerg_Creep_Colony:
        QUEUE_COMMAND(BW::Orders::BuildingMorph, type1);
        break;
      case UnitTypes::Enum::Protoss_Carrier:
      case UnitTypes::Enum::Hero_Gantrithor:
      case UnitTypes::Enum::Protoss_Reaver:
      case UnitTypes::Enum::Hero_Warbringer:
        QUEUE_COMMAND(BW::Orders::TrainFighter);
        break;
      default:
        QUEUE_COMMAND(BW::Orders::TrainUnit, type1);
        break;
      }
    }
    else if (ct == UnitCommandTypes::Morph)
    {
      UnitType type(command.extra);
      if ( type.isBuilding() )
        QUEUE_COMMAND(BW::Orders::BuildingMorph, type);
      else
        QUEUE_COMMAND(BW::Orders::UnitMorph, type);
    }
    else if (ct == UnitCommandTypes::Research)
      QUEUE_COMMAND(BW::Orders::Invent, command.getTechType());
    else if (ct == UnitCommandTypes::Upgrade)
      QUEUE_COMMAND(BW::Orders::Upgrade, command.getUpgradeType());
    else if (ct == UnitCommandTypes::Set_Rally_Position)
      QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, Order(Orders::Enum::RallyPointTile));
    else if (ct == UnitCommandTypes::Set_Rally_Unit)
      QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::RallyPointUnit));
    else if (ct == UnitCommandTypes::Move)
      QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, Order(Orders::Enum::Move), queued);
    else if (ct == UnitCommandTypes::Patrol)
      QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, Order(Orders::Enum::Patrol), queued);
    else if (ct == UnitCommandTypes::Hold_Position)
      QUEUE_COMMAND(BW::Orders::HoldPosition, queued);
    else if (ct == UnitCommandTypes::Stop)
    {
      switch ( command.unit ? command.unit->getType() : UnitTypes::None )
      {
      case UnitTypes::Enum::Protoss_Reaver:
      case UnitTypes::Enum::Hero_Warbringer:
        QUEUE_COMMAND(BW::Orders::ReaverStop);
        break;
      case UnitTypes::Enum::Protoss_Carrier:
      case UnitTypes::Enum::Hero_Gantrithor:
        QUEUE_COMMAND(BW::Orders::CarrierStop);
        break;
      default:
        QUEUE_COMMAND(BW::Orders::Stop, queued);
        break;
      }
    }
    else if (ct == UnitCommandTypes::Follow)
      QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::Follow), queued);
    else if (ct == UnitCommandTypes::Gather)
      QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::Harvest1), queued);
    else if (ct == UnitCommandTypes::Return_Cargo)
      QUEUE_COMMAND(BW::Orders::ReturnCargo, queued);
    else if (ct == UnitCommandTypes::Repair)
      QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::Repair), queued);
    else if (ct == UnitCommandTypes::Burrow)
      QUEUE_COMMAND(BW::Orders::Burrow);
    else if (ct == UnitCommandTypes::Unburrow)
      QUEUE_COMMAND(BW::Orders::Unburrow);
    else if (ct == UnitCommandTypes::Cloak)
      QUEUE_COMMAND(BW::Orders::Cloak);
    else if (ct == UnitCommandTypes::Decloak)
      QUEUE_COMMAND(BW::Orders::Decloak);
    else if (ct == UnitCommandTypes::Siege)
      QUEUE_COMMAND(BW::Orders::Siege);
    else if (ct == UnitCommandTypes::Unsiege)
      QUEUE_COMMAND(BW::Orders::Unsiege);
    else if (ct == UnitCommandTypes::Lift)
      QUEUE_COMMAND(BW::Orders::Lift);
    else if (ct == UnitCommandTypes::Land)
    {
      if (command.unit)
        QUEUE_COMMAND(BW::Orders::Land, command.x, command.y, command.unit->getType());
    }
    else if (ct == UnitCommandTypes::Load)
    {
      BWAPI::UnitType thisType = command.unit ? command.unit->getType() : UnitTypes::None;
      if ( thisType == UnitTypes::Terran_Bunker )
        QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::PickupBunker), queued);
      else if ( thisType == UnitTypes::Terran_Dropship || 
                thisType == UnitTypes::Protoss_Shuttle || 
                thisType == UnitTypes::Zerg_Overlord   ||
                thisType == UnitTypes::Hero_Yggdrasill )
        QUEUE_COMMAND(BW::Orders::Attack, command.target, Order(Orders::Enum::PickupTransport), queued);
      else if ( command.target->getType() == UnitTypes::Terran_Bunker   ||
                command.target->getType() == UnitTypes::Terran_Dropship ||
                command.target->getType() == UnitTypes::Protoss_Shuttle ||
                command.target->getType() == UnitTypes::Zerg_Overlord   ||
                command.target->getType() == UnitTypes::Hero_Yggdrasill )
        QUEUE_COMMAND(BW::Orders::RightClick, command.target, queued);
    }
    else if (ct == UnitCommandTypes::Unload)
    {
      QUEUE_COMMAND(BW::Orders::UnloadUnit, command.target);
    }
    else if (ct == UnitCommandTypes::Unload_All && command.unit)
    {
      if ( command.unit->getType() == UnitTypes::Terran_Bunker )
        QUEUE_COMMAND(BW::Orders::UnloadAll);
      else
        QUEUE_COMMAND(BW::Orders::Attack, command.unit->getPosition(), Order(Orders::Enum::MoveUnload), queued);
    }
    else if (ct == UnitCommandTypes::Unload_All_Position)
    {
      QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, Order(Orders::Enum::MoveUnload), queued);
    }
    else if (ct == UnitCommandTypes::Right_Click_Position)
      QUEUE_COMMAND(BW::Orders::RightClick, command.x, command.y, queued);
    else if (ct == UnitCommandTypes::Right_Click_Unit)
      QUEUE_COMMAND(BW::Orders::RightClick, command.target, queued);
    else if (ct == UnitCommandTypes::Halt_Construction)
      QUEUE_COMMAND(BW::Orders::Stop);
    else if (ct == UnitCommandTypes::Cancel_Construction)
      QUEUE_COMMAND(BW::Orders::CancelConstruction);
    else if (ct == UnitCommandTypes::Cancel_Addon)
      QUEUE_COMMAND(BW::Orders::CancelAddon);
    else if (ct == UnitCommandTypes::Cancel_Train || ct == UnitCommandTypes::Cancel_Train_Slot)
      QUEUE_COMMAND(BW::Orders::CancelTrain, command.extra);
    else if (ct == UnitCommandTypes::Cancel_Morph)
    {
      if ( command.unit && command.unit->getType().isBuilding() )
        QUEUE_COMMAND(BW::Orders::CancelConstruction);
      else
        QUEUE_COMMAND(BW::Orders::CancelUnitMorph);
    }
    else if (ct == UnitCommandTypes::Cancel_Research)
      QUEUE_COMMAND(BW::Orders::CancelResearch);
    else if (ct == UnitCommandTypes::Cancel_Upgrade)
      QUEUE_COMMAND(BW::Orders::CancelUpgrade);
    else if (ct == UnitCommandTypes::Use_Tech)
    {
      TechType tech(command.extra);
      switch (tech)
      {
        case TechTypes::Enum::Stim_Packs:
          QUEUE_COMMAND(BW::Orders::UseStimPack);
          break;
        case TechTypes::Enum::Tank_Siege_Mode:
          if ( command.unit && command.unit->isSieged() )
            QUEUE_COMMAND(BW::Orders::Unsiege);
          else
            QUEUE_COMMAND(BW::Orders::Siege);
          break;
        case TechTypes::Enum::Personnel_Cloaking:
        case TechTypes::Enum::Cloaking_Field:
          if ( command.unit && command.unit->isCloaked() )
            QUEUE_COMMAND(BW::Orders::Decloak);
          else
            QUEUE_COMMAND(BW::Orders::Cloak);
          break;
        case TechTypes::Enum::Burrowing:
          if ( command.unit && command.unit->isBurrowed() )
            QUEUE_COMMAND(BW::Orders::Unburrow);
          else
            QUEUE_COMMAND(BW::Orders::Burrow);
          break;
      }
    }
    else if (ct == UnitCommandTypes::Use_Tech_Position)
    {
      Order order = (command.getTechType() == TechTypes::Healing ? Orders::HealMove : command.getTechType().getOrder());
      QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, order);
    }
    else if (ct == UnitCommandTypes::Use_Tech_Unit)
    {
      TechType tech(command.extra);
      if (tech == TechTypes::Archon_Warp)
        QUEUE_COMMAND(BW::Orders::MergeArchon);
      else if (tech == TechTypes::Dark_Archon_Meld)
        QUEUE_COMMAND(BW::Orders::MergeDarkArchon);
      else
        QUEUE_COMMAND(BW::Orders::Attack, command.target, tech.getOrder());
    }
    else if ( ct == UnitCommandTypes::Place_COP && command.unit )
      QUEUE_COMMAND(BW::Orders::PlaceCOP, command.x, command.y, command.unit->getType());
  }

}
