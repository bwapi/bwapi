#include "GameImpl.h"
#include "DLLMain.h"

#include <BWAPI/Command.h>

#include <BW/TechID.h>
#include <BW/UnitID.h>

#include "../../Debug.h"

namespace BWAPI
{
  //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
  void GameImpl::addToCommandBuffer(Command* command)
  {
    //executes latency compensation code and added it to the buffer
    command->execute(0);
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
  }
  //----------------------------------------- APPLY LATENCY COMPENSATION
  void GameImpl::applyLatencyCompensation()
  {
    //apply latency compensation
    while ((int)(this->commandBuffer.size()) > this->getLatency()+15)
    {
      for (unsigned int i = 0; i < this->commandBuffer.front().size(); ++i)
        delete this->commandBuffer.front()[i];
      this->commandBuffer.erase(this->commandBuffer.begin());
    }
    this->commandBuffer.push_back(std::vector<Command *>());
    for (unsigned int i = 0; i < this->commandBuffer.size(); ++i)
      for (unsigned int j = 0; j < this->commandBuffer[i].size(); ++j)
        this->commandBuffer[i][j]->execute(this->commandBuffer.size()-1-i);
  }

  //------------------------------------------- ADD TO CMD OPTIMIZER -----------------------------------------
  bool GameImpl::addToCommandOptimizer(UnitCommand command)
  {
    // ignore queued and invalid commands, or return if optimizer is disabled
    if (  commandOptimizerLevel == 0 || 
          command.isQueued() || 
          command.getType() >= UnitCommandTypes::None )
      return false;

    // Store some commonly accessed variables
    UnitCommandType uct = command.getType();

    Unit      *utarg   = command.getTarget();
    UnitType  targType = utarg ? utarg->getType() : UnitTypes::None;
    Unit      *uthis   = command.getUnit();
    UnitType  thisType = uthis ? uthis->getType() : UnitTypes::None;

    //  Exclude commands that cannot be optimized.
    if (  uct == UnitCommandTypes::Build_Addon ||
          uct == UnitCommandTypes::Land        || 
          uct == UnitCommandTypes::Build       ||
          uct == UnitCommandTypes::Place_COP   ||
          uct == UnitCommandTypes::Research    ||
          uct == UnitCommandTypes::Upgrade     || 
          (commandOptimizerLevel < 4 &&
           uct == UnitCommandTypes::Use_Tech_Unit &&
          (command.getTechType() == TechTypes::Archon_Warp ||
           command.getTechType() == TechTypes::Dark_Archon_Meld)) )
      return false;


    // Simplify some commands if possible to decrease their size
    if ( uct == UnitCommandTypes::Attack_Unit )
    {
      // Use Right Click for Attack Unit
      if ( thisType.canAttack() && utarg && self() && self()->isEnemy(utarg->getPlayer()) )
        command.type = UnitCommandTypes::Right_Click_Unit;
    }
    else if ( uct == UnitCommandTypes::Move )
    {
      // Use Right Click for Move
      command = UnitCommand::rightClick(uthis, command.getTargetPosition());
    }
    else if ( uct == UnitCommandTypes::Gather )
    {
      // Use Right Click for gather
      if ( targType.isResourceContainer() )
        command = UnitCommand::rightClick(uthis, utarg);
    }
    else if ( uct == UnitCommandTypes::Set_Rally_Position )
    {
      // Use Right Click for Set Rally
      if ( thisType.canProduce() && 
           thisType != UnitTypes::Protoss_Carrier && thisType != UnitTypes::Hero_Gantrithor &&
           thisType != UnitTypes::Protoss_Reaver  && thisType != UnitTypes::Hero_Warbringer )
        command = UnitCommand::rightClick(uthis, command.getTargetPosition());
    }
    else if ( uct == UnitCommandTypes::Set_Rally_Unit )
    {
      // Use Right Click for Set Rally
      if ( thisType.canProduce() && 
           thisType != UnitTypes::Protoss_Carrier && thisType != UnitTypes::Hero_Gantrithor &&
           thisType != UnitTypes::Protoss_Reaver  && thisType != UnitTypes::Hero_Warbringer )
        command = UnitCommand::rightClick(uthis, utarg);
    }
    else if ( uct == UnitCommandTypes::Use_Tech_Unit )
    {
      // Use Right Click for infestation
      if ( command.getTechType() == TechTypes::Infestation &&
           (thisType == UnitTypes::Zerg_Queen || thisType == UnitTypes::Hero_Matriarch) &&
           targType == UnitTypes::Terran_Command_Center )
        command = UnitCommand::rightClick(uthis, utarg);
    }
    else if ( uct == UnitCommandTypes::Train )
    {
      // Create a single placeholder since we assume it stores an interceptor or scarab when it's not important
      if ( thisType == UnitTypes::Protoss_Carrier ||
           thisType == UnitTypes::Hero_Gantrithor ||
           thisType == UnitTypes::Protoss_Reaver  ||
           thisType == UnitTypes::Hero_Warbringer)
        command = UnitCommand::train(uthis, UnitTypes::Protoss_Interceptor);
    }
    else if ( uct == UnitCommandTypes::Unload_All_Position )
    {
      // Bunkers should only use Unload_All
      if ( thisType == UnitTypes::Terran_Bunker )
        command = UnitCommand::unloadAll(uthis);
    }
    else if ( uct == UnitCommandTypes::Use_Tech )
    {
      // Simplify siege/cloak/burrow tech to their specific commands to allow grouping them
      switch ( command.getTechType() )
      {
      case BW::TechID::TankSiegeMode:
        if ( command.unit && command.unit->isSieged() )
          command = UnitCommand::unsiege(uthis);
        else
          command = UnitCommand::siege(uthis);
        break;
      case BW::TechID::PersonnelCloaking:
      case BW::TechID::CloakingField:
        if ( command.unit && command.unit->isCloaked() )
          command = UnitCommand::decloak(uthis);
        else
          command = UnitCommand::cloak(uthis);
        break;
      case BW::TechID::Burrowing:
        if ( command.unit && command.unit->isBurrowed() )
          command = UnitCommand::unburrow(uthis);
        else
          command = UnitCommand::burrow(uthis);
        break;
      }
    }

    // Exclude commands not optimized at optimizer level 1 (no multi-select buildings)
    if ( commandOptimizerLevel <= 1 && thisType < UnitTypes::None && thisType.isBuilding() )
      return false;

    // Exclude commands not optimized at or below optimizer level 2 (no position commands)
    if ( commandOptimizerLevel <= 2 && 
        (uct == UnitCommandTypes::Attack_Move           ||
         uct == UnitCommandTypes::Move                  ||
         uct == UnitCommandTypes::Patrol                ||
         uct == UnitCommandTypes::Right_Click_Position  ||
         uct == UnitCommandTypes::Set_Rally_Position    ||
         uct == UnitCommandTypes::Unload_All_Position   ||
         uct == UnitCommandTypes::Use_Tech_Position) )
      return false;

    if ( commandOptimizerLevel >= 4 )
    {
      // Convert tech unit target to tech position target commands so that they can be
      // optimized with nearby tech position commands of the same type.
      if ( uct == UnitCommandTypes::Use_Tech_Unit && command.getTechType().targetsPosition() && utarg )
        command = UnitCommand::useTech(uthis, command.getTechType(), utarg->getPosition());

      // Align locations to 32 pixels
      if ( uct == UnitCommandTypes::Attack_Move          ||
           uct == UnitCommandTypes::Move                 ||
           uct == UnitCommandTypes::Patrol               ||
           uct == UnitCommandTypes::Right_Click_Position ||
           uct == UnitCommandTypes::Set_Rally_Position   ||
           uct == UnitCommandTypes::Unload_All_Position  ||
           uct == UnitCommandTypes::Use_Tech_Position )
        command = UnitCommand(uthis, uct, utarg, command.x & (~0x1F), command.y & (~0x1F), command.extra);
      else if ( uct == UnitCommandTypes::Use_Tech_Unit &&   // Group Archon & Dark Archon merging
                (command.getTechType() == TechTypes::Archon_Warp ||
                command.getTechType() == TechTypes::Dark_Archon_Meld) )
        command = UnitCommand::useTech(uthis, command.getTechType(), NULL);
    }
    // Add command to the command optimizer buffer and unload it later
    commandOptimizer[command.getType().getID()].push_back(command);
    return true;
  }
  //--------------------------------------------- EXECUTE COMMAND --------------------------------------------
  void GameImpl::executeCommand(UnitCommand command, bool addCommandToLatComBuffer)
  {
    botAPMCounter_noselects++;
    UnitCommandType ct = command.type;
    bool queued = command.isQueued();
    if (ct == UnitCommandTypes::Attack_Move)
    {
      if ( command.unit && command.unit->getType() == UnitTypes::Zerg_Infested_Terran )
        QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, BW::OrderID::Attack1, queued);
      else
        QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, BW::OrderID::AttackMove, queued);
    }
    else if (ct == UnitCommandTypes::Attack_Unit)
    {
      UnitImpl *target = (UnitImpl*)command.target;
      UnitType ut      = command.unit ? command.unit->getType() : UnitTypes::None;
      if ( ut == UnitTypes::Protoss_Carrier || ut == UnitTypes::Hero_Gantrithor )
        QUEUE_COMMAND(BW::Orders::Attack, target, BW::OrderID::CarrierAttack1, queued);
      else if ( ut == UnitTypes::Protoss_Reaver || ut == UnitTypes::Hero_Warbringer )
        QUEUE_COMMAND(BW::Orders::Attack, target, BW::OrderID::ReaverAttack1, queued);
      else if ( ut.isBuilding() )
        QUEUE_COMMAND(BW::Orders::Attack, target, BW::OrderID::TowerAttack, queued);
      else
        QUEUE_COMMAND(BW::Orders::Attack, target, BW::OrderID::Attack1, queued);
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
      TilePosition target(command.unit->getTilePosition().x() + 4, command.unit->getTilePosition().y() + 1);
      QUEUE_COMMAND(BW::Orders::MakeAddon, target.makeValid(), command.getUnitType());
    }
    else if ( ct == UnitCommandTypes::Train )
    {
      UnitType type1(command.extra);
      switch ( command.unit ? command.unit->getType() : UnitTypes::None )
      {
      case BW::UnitID::Zerg_Larva:
      case BW::UnitID::Zerg_Mutalisk:
      case BW::UnitID::Zerg_Hydralisk:
        QUEUE_COMMAND(BW::Orders::UnitMorph, type1);
        break;
      case BW::UnitID::Zerg_Hatchery:
      case BW::UnitID::Zerg_Lair:
      case BW::UnitID::Zerg_Spire:
      case BW::UnitID::Zerg_CreepColony:
        QUEUE_COMMAND(BW::Orders::BuildingMorph, type1);
        break;
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
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
      QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, BW::OrderID::RallyPointTile);
    else if (ct == UnitCommandTypes::Set_Rally_Unit)
      QUEUE_COMMAND(BW::Orders::Attack, command.target, BW::OrderID::RallyPointUnit);
    else if (ct == UnitCommandTypes::Move)
      QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, BW::OrderID::Move, queued);
    else if (ct == UnitCommandTypes::Patrol)
      QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, BW::OrderID::Patrol, queued);
    else if (ct == UnitCommandTypes::Hold_Position)
      QUEUE_COMMAND(BW::Orders::HoldPosition, queued);
    else if (ct == UnitCommandTypes::Stop)
    {
      switch ( command.unit ? command.unit->getType() : UnitTypes::None )
      {
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
        QUEUE_COMMAND(BW::Orders::ReaverStop);
        break;
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        QUEUE_COMMAND(BW::Orders::CarrierStop);
        break;
      default:
        QUEUE_COMMAND(BW::Orders::Stop, queued);
        break;
      }
    }
    else if (ct == UnitCommandTypes::Follow)
      QUEUE_COMMAND(BW::Orders::Attack, command.target, BW::OrderID::Follow, queued);
    else if (ct == UnitCommandTypes::Gather)
      QUEUE_COMMAND(BW::Orders::Attack, command.target, BW::OrderID::Harvest1, queued);
    else if (ct == UnitCommandTypes::Return_Cargo)
      QUEUE_COMMAND(BW::Orders::ReturnCargo, queued);
    else if (ct == UnitCommandTypes::Repair)
      QUEUE_COMMAND(BW::Orders::Attack, command.target, BW::OrderID::Repair1, queued);
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
      QUEUE_COMMAND(BW::Orders::Land, command.x, command.y, command.unit->getType());
    else if (ct == UnitCommandTypes::Load)
    {
      BWAPI::UnitType thisType = command.unit ? command.unit->getType() : UnitTypes::None;
      if ( thisType == UnitTypes::Terran_Bunker )
        QUEUE_COMMAND(BW::Orders::Attack, command.target, BW::OrderID::PickupBunker, queued);
      else if ( thisType == UnitTypes::Terran_Dropship || 
                thisType == UnitTypes::Protoss_Shuttle || 
                thisType == UnitTypes::Zerg_Overlord   ||
                thisType == UnitTypes::Hero_Yggdrasill )
        QUEUE_COMMAND(BW::Orders::Attack, command.target, BW::OrderID::PickupTransport, queued);
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
        QUEUE_COMMAND(BW::Orders::Attack, command.unit->getPosition(), BW::OrderID::MoveUnload, queued);
    }
    else if (ct == UnitCommandTypes::Unload_All_Position)
    {
      if ( command.unit && command.unit->getType() == UnitTypes::Terran_Bunker)
        QUEUE_COMMAND(BW::Orders::UnloadAll);
      else
        QUEUE_COMMAND(BW::Orders::Attack, command.x, command.y, BW::OrderID::MoveUnload, queued);
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
        case BW::TechID::Stimpacks:
          QUEUE_COMMAND(BW::Orders::UseStimPack);
          break;
        case BW::TechID::TankSiegeMode:
          if ( command.unit && command.unit->isSieged() )
            QUEUE_COMMAND(BW::Orders::Unsiege);
          else
            QUEUE_COMMAND(BW::Orders::Siege);
          break;
        case BW::TechID::PersonnelCloaking:
        case BW::TechID::CloakingField:
          if ( command.unit && command.unit->isCloaked() )
            QUEUE_COMMAND(BW::Orders::Decloak);
          else
            QUEUE_COMMAND(BW::Orders::Cloak);
          break;
        case BW::TechID::Burrowing:
          if ( command.unit && command.unit->isBurrowed() )
            QUEUE_COMMAND(BW::Orders::Unburrow);
          else
            QUEUE_COMMAND(BW::Orders::Burrow);
          break;
      }
    }
    else if (ct == UnitCommandTypes::Use_Tech_Position)
    {
      int order = (command.getTechType() == TechTypes::Healing ? Orders::HealMove : command.getTechType().getOrder());
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

    if (addCommandToLatComBuffer)
      BroodwarImpl.addToCommandBuffer(new Command(command));
  }


}
