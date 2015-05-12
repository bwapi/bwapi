#include "CommandOptimizer.h"

#include <BWAPI/UnitType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/Player.h>

#include "UnitImpl.h"
#include "GameImpl.h"
#include <BW/OrderTypes.h>
#include "../DLLMain.h"

using namespace BWAPI;


bool CommandOptimizer::add(UnitCommand command)
{
  // ignore queued and invalid commands, or return if optimizer is disabled
  if (level == 0 ||
    command.isQueued() ||
    command.getType() >= UnitCommandTypes::None)
    return false;

  // Store some commonly accessed variables
  UnitCommandType uct = command.getType();
  Unit utarg = command.getTarget();
  UnitType  targType = utarg ? utarg->getType() : UnitTypes::None;
  Unit uthis = command.getUnit();
  UnitType  thisType = uthis ? uthis->getType() : UnitTypes::None;

  //  Exclude commands that cannot be optimized.
  if (uct == UnitCommandTypes::Build_Addon ||
    uct == UnitCommandTypes::Land ||
    uct == UnitCommandTypes::Build ||
    uct == UnitCommandTypes::Place_COP ||
    uct == UnitCommandTypes::Research ||
    uct == UnitCommandTypes::Upgrade ||
    (level < 4 &&
    uct == UnitCommandTypes::Use_Tech_Unit &&
    (command.getTechType() == TechTypes::Archon_Warp ||
    command.getTechType() == TechTypes::Dark_Archon_Meld)))
    return false;


  // Simplify some commands if possible to decrease their size
  if (uct == UnitCommandTypes::Attack_Unit)
  {
    // Use Right Click for Attack Unit
    if (thisType.canAttack() && utarg && Broodwar->self() && Broodwar->self()->isEnemy(utarg->getPlayer()))
      command.type = UnitCommandTypes::Right_Click_Unit;
  }
  else if (uct == UnitCommandTypes::Move)
  {
    // Use Right Click for Move
    command = UnitCommand::rightClick(uthis, command.getTargetPosition());
  }
  else if (uct == UnitCommandTypes::Gather)
  {
    // Use Right Click for gather
    if (targType.isResourceContainer())
      command = UnitCommand::rightClick(uthis, utarg);
  }
  else if (uct == UnitCommandTypes::Set_Rally_Position)
  {
    // Use Right Click for Set Rally
    if (thisType.canProduce() &&
      thisType != UnitTypes::Protoss_Carrier && thisType != UnitTypes::Hero_Gantrithor &&
      thisType != UnitTypes::Protoss_Reaver  && thisType != UnitTypes::Hero_Warbringer)
      command = UnitCommand::rightClick(uthis, command.getTargetPosition());
  }
  else if (uct == UnitCommandTypes::Set_Rally_Unit)
  {
    // Use Right Click for Set Rally
    if (thisType.canProduce() &&
      thisType != UnitTypes::Protoss_Carrier && thisType != UnitTypes::Hero_Gantrithor &&
      thisType != UnitTypes::Protoss_Reaver  && thisType != UnitTypes::Hero_Warbringer)
      command = UnitCommand::rightClick(uthis, utarg);
  }
  else if (uct == UnitCommandTypes::Use_Tech_Unit)
  {
    // Use Right Click for infestation
    if (command.getTechType() == TechTypes::Infestation &&
      (thisType == UnitTypes::Zerg_Queen || thisType == UnitTypes::Hero_Matriarch) &&
      targType == UnitTypes::Terran_Command_Center)
      command = UnitCommand::rightClick(uthis, utarg);
  }
  else if (uct == UnitCommandTypes::Train)
  {
    // Create a single placeholder since we assume it stores an interceptor or scarab when it's not important
    if (thisType == UnitTypes::Protoss_Carrier ||
      thisType == UnitTypes::Hero_Gantrithor ||
      thisType == UnitTypes::Protoss_Reaver ||
      thisType == UnitTypes::Hero_Warbringer)
      command = UnitCommand::train(uthis, UnitTypes::Protoss_Interceptor);
  }
  else if (uct == UnitCommandTypes::Use_Tech)
  {
    // Simplify siege/cloak/burrow tech to their specific commands to allow grouping them
    switch (command.getTechType())
    {
    case TechTypes::Enum::Tank_Siege_Mode:
      if (command.unit && command.unit->isSieged())
        command = UnitCommand::unsiege(uthis);
      else
        command = UnitCommand::siege(uthis);
      break;
    case TechTypes::Enum::Personnel_Cloaking:
    case TechTypes::Enum::Cloaking_Field:
      if (command.unit && command.unit->isCloaked())
        command = UnitCommand::decloak(uthis);
      else
        command = UnitCommand::cloak(uthis);
      break;
    case TechTypes::Enum::Burrowing:
      if (command.unit && command.unit->isBurrowed())
        command = UnitCommand::unburrow(uthis);
      else
        command = UnitCommand::burrow(uthis);
      break;
    }
  }

  // Exclude commands not optimized at optimizer level 1 (no multi-select buildings)
  if (level <= 1 && thisType < UnitTypes::None && thisType.isBuilding())
    return false;

  // Exclude commands not optimized at or below optimizer level 2 (no position commands)
  if (level <= 2 &&
    (uct == UnitCommandTypes::Attack_Move ||
    uct == UnitCommandTypes::Move ||
    uct == UnitCommandTypes::Patrol ||
    uct == UnitCommandTypes::Right_Click_Position ||
    uct == UnitCommandTypes::Set_Rally_Position ||
    uct == UnitCommandTypes::Unload_All_Position ||
    uct == UnitCommandTypes::Use_Tech_Position))
    return false;

  if (level >= 4)
  {
    // Convert tech unit target to tech position target commands so that they can be
    // optimized with nearby tech position commands of the same type.
    if (uct == UnitCommandTypes::Use_Tech_Unit && command.getTechType().targetsPosition() && utarg)
      command = UnitCommand::useTech(uthis, command.getTechType(), utarg->getPosition());

    // Align locations to 32 pixels
    if (uct == UnitCommandTypes::Attack_Move ||
      uct == UnitCommandTypes::Move ||
      uct == UnitCommandTypes::Patrol ||
      uct == UnitCommandTypes::Right_Click_Position ||
      uct == UnitCommandTypes::Set_Rally_Position ||
      uct == UnitCommandTypes::Unload_All_Position ||
      uct == UnitCommandTypes::Use_Tech_Position)
      command = UnitCommand(uthis, uct, utarg, command.x & (~0x1F), command.y & (~0x1F), command.extra);
    else if (uct == UnitCommandTypes::Use_Tech_Unit &&   // Group Archon & Dark Archon merging
      (command.getTechType() == TechTypes::Archon_Warp ||
      command.getTechType() == TechTypes::Dark_Archon_Meld))
      command = UnitCommand::useTech(uthis, command.getTechType(), nullptr);
  }

  // Set last immediate command again in case it was altered when inserting it into the optimizer
  static_cast<UnitImpl*>(command.unit)->setLastImmediateCommand(command);

  // Add command to the command optimizer buffer and unload it later (newest commands first)
  optimizerQueue[command.getType().getID()].push_front(command);
  return true;
}

void CommandOptimizer::clear()
{
  for (int i = 0; i < UnitCommandTypes::None; ++i)
    optimizerQueue[i].clear();
}

void CommandOptimizer::flush()
{
  // TODO Only process on the frame before commands are sent

  // Iterate the command types
  for (int i = 0; i < UnitCommandTypes::Enum::None; ++i)
  {
    // Declare our temporary variables
    std::vector<Unit> groupOf12;

    Position pos = Positions::None;
    int       e = 0;
    Unit t = nullptr;
    UnitType  ut;
    bool      o = false;

    // Iterate the list
    auto cmd = optimizerQueue[i].begin();

    // Re-Iterate all remaining commands
    while (cmd != optimizerQueue[i].end())
    {
      // Iterate all commands, and only process those that are equal
      while (cmd != optimizerQueue[i].end())
      {
        // Ignore anything but the command that the unit last processed
        /*if ( //static_cast<UnitImpl*>(cmd->unit)->lastImmediateCommandFrame == this->getFrameCount() &&
        static_cast<UnitImpl*>(cmd->unit)->lastImmediateCommand != *cmd )
        {
        cmd = commandOptimizer[i].erase(cmd);
        continue;
        }*/

        // If we are starting a new command grouping
        if (groupOf12.empty())
        {
          // Assign our comparison variables to determine which commands should be grouped
          // Note: Using individual variables instead of comparing UnitCommand operator== because
          //       it will also compare the type which is not necessary, and we may create a new
          //       optimization type that does a fuzzy position comparison
          e = cmd->extra;
          t = cmd->target;
          pos = cmd->getTargetPosition();
          if (i == UnitCommandTypes::Attack_Unit ||
            i == UnitCommandTypes::Unload_All ||
            i == UnitCommandTypes::Load ||
            i == UnitCommandTypes::Cancel_Morph)
            o = cmd->unit->getType().isBuilding();
          else if (i == UnitCommandTypes::Use_Tech)
            o = cmd->unit->isSieged() || cmd->unit->isCloaked() || cmd->unit->isBurrowed();
          else
            o = false;
          groupOf12.push_back(cmd->unit);
          cmd = optimizerQueue[i].erase(cmd);
        } // otherwise if this command is the same as the first, the units can be grouped
        else if (e == cmd->extra && t == cmd->target && pos == cmd->getTargetPosition())
        {
          bool oTmp;
          if (i == UnitCommandTypes::Attack_Unit ||
            i == UnitCommandTypes::Unload_All ||
            i == UnitCommandTypes::Load ||
            i == UnitCommandTypes::Cancel_Morph)
            oTmp = cmd->unit->getType().isBuilding();
          else if (i == UnitCommandTypes::Use_Tech)
            oTmp = cmd->unit->isSieged() || cmd->unit->isCloaked() || cmd->unit->isBurrowed();
          else
            oTmp = false;

          if (o == oTmp)
          {
            groupOf12.push_back(cmd->unit);
            cmd = optimizerQueue[i].erase(cmd);
          }
          else
            ++cmd;
        } // otherwise skip this command for now
        else
        {
          ++cmd;
        }

        // If our group of 12 is full
        if (groupOf12.size() == 12)
        {
          // Select the group
          BW::Orders::Select sel(groupOf12);
          QueueGameCommand(&sel, sel.size());
          BroodwarImpl.apmCounter.addSelect();

          // Workaround for doing certain actions
          Unit unit = (i == UnitCommandTypes::Load ||
            i == UnitCommandTypes::Attack_Unit ||
            i == UnitCommandTypes::Train ||
            i == UnitCommandTypes::Unload_All ||
            i == UnitCommandTypes::Cancel_Morph ||
            i == UnitCommandTypes::Use_Tech) ? groupOf12.front() : nullptr;

          // execute command
          BroodwarImpl.executeCommand(UnitCommand(unit, i, t, pos.x, pos.y, e));
          groupOf12.clear();
        } // groupOf12 max execute
      } // second while

      // If we iterated the entire command list and don't have an empty group, then give a command
      // to the remaining units in the group
      if (!groupOf12.empty())
      {
        // Select the group
        BW::Orders::Select sel(groupOf12);
        QueueGameCommand(&sel, sel.size());
        BroodwarImpl.apmCounter.addSelect();

        // Workaround for doing certain actions
        Unit unit = (i == UnitCommandTypes::Load ||
          i == UnitCommandTypes::Attack_Unit ||
          i == UnitCommandTypes::Train ||
          i == UnitCommandTypes::Unload_All ||
          i == UnitCommandTypes::Cancel_Morph ||
          i == UnitCommandTypes::Use_Tech) ? groupOf12.front() : nullptr;

        // execute command
        BroodwarImpl.executeCommand(UnitCommand(unit, i, t, pos.x, pos.y, e));
        groupOf12.clear();
      }
      // Reset iterator
      cmd = optimizerQueue[i].begin();
    } // first while
  } // iterate command types
}

void CommandOptimizer::init()
{
  clear();
  level = 0;
}
