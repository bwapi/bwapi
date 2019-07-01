#include <BWAPI/CommandOptimizer.h>

#include <BWAPI/UnitType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/Player.h>
#include <BWAPI/Game.h>
#include <BWAPI/Client/Client.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Unitset.h>

using namespace BWAPI;

CommandOptimizer::CommandOptimizer(Game& game) : game(game) {}

bool CommandOptimizer::add(UnitCommand command)
{
  // ignore queued and invalid commands, or return if optimizer is disabled
  if (level == 0 ||
    command.isQueued() ||
    command.getType() >= UnitCommandTypes::None)
    return false;

  // Store some commonly accessed variables
  UnitCommandType uct = command.getType();
  UnitID targID = command.getTarget();
  Unit utarg = game.getUnit(targID);
  UnitType  targType = utarg ? utarg->getType() : UnitTypes::None;

  UnitID thisID = command.getUnit();
  Unit uthis = game.getUnit(thisID);
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
    if (thisType.canAttack() && utarg && game.self() && game.self()->isEnemy(utarg->getPlayer()))
      command.type = UnitCommandTypes::Right_Click_Unit;
  }
  else if (uct == UnitCommandTypes::Move)
  {
    // Use Right Click for Move
    command = UnitCommand::rightClick(thisID, command.getTargetPosition());
  }
  else if (uct == UnitCommandTypes::Gather)
  {
    // Use Right Click for gather
    if (targType.isResourceContainer())
      command = UnitCommand::rightClick(thisID, targID);
  }
  else if (uct == UnitCommandTypes::Set_Rally_Position)
  {
    // Use Right Click for Set Rally
    if (thisType.canProduce() &&
      thisType != UnitTypes::Protoss_Carrier && thisType != UnitTypes::Hero_Gantrithor &&
      thisType != UnitTypes::Protoss_Reaver  && thisType != UnitTypes::Hero_Warbringer)
      command = UnitCommand::rightClick(thisID, command.getTargetPosition());
  }
  else if (uct == UnitCommandTypes::Set_Rally_Unit)
  {
    // Use Right Click for Set Rally
    if (thisType.canProduce() &&
      thisType != UnitTypes::Protoss_Carrier && thisType != UnitTypes::Hero_Gantrithor &&
      thisType != UnitTypes::Protoss_Reaver  && thisType != UnitTypes::Hero_Warbringer)
      command = UnitCommand::rightClick(thisID, targID);
  }
  else if (uct == UnitCommandTypes::Use_Tech_Unit)
  {
    // Use Right Click for infestation
    if (command.getTechType() == TechTypes::Infestation &&
      (thisType == UnitTypes::Zerg_Queen || thisType == UnitTypes::Hero_Matriarch) &&
      targType == UnitTypes::Terran_Command_Center)
      command = UnitCommand::rightClick(thisID, targID);
  }
  else if (uct == UnitCommandTypes::Train)
  {
    // Create a single placeholder since we assume it stores an interceptor or scarab when it's not important
    if (thisType == UnitTypes::Protoss_Carrier ||
      thisType == UnitTypes::Hero_Gantrithor ||
      thisType == UnitTypes::Protoss_Reaver ||
      thisType == UnitTypes::Hero_Warbringer)
      command = UnitCommand::train(thisID, UnitTypes::Protoss_Interceptor);
  }
  else if (uct == UnitCommandTypes::Use_Tech)
  {
    // Simplify siege/cloak/burrow tech to their specific commands to allow grouping them
    switch (command.getTechType())
    {
    case TechTypes::Enum::Tank_Siege_Mode:
      if (command.unit && uthis.isSieged())
        command = UnitCommand::unsiege(thisID);
      else
        command = UnitCommand::siege(thisID);
      break;
    case TechTypes::Enum::Personnel_Cloaking:
    case TechTypes::Enum::Cloaking_Field:
      if (command.unit && uthis.isCloaked())
        command = UnitCommand::decloak(thisID);
      else
        command = UnitCommand::cloak(thisID);
      break;
    case TechTypes::Enum::Burrowing:
      if (command.unit && uthis.isBurrowed())
        command = UnitCommand::unburrow(thisID);
      else
        command = UnitCommand::burrow(thisID);
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
      command = UnitCommand::useTech(thisID, command.getTechType(), utarg->getPosition());

    // Align locations to 32 pixels
    if (uct == UnitCommandTypes::Attack_Move ||
      uct == UnitCommandTypes::Move ||
      uct == UnitCommandTypes::Patrol ||
      uct == UnitCommandTypes::Right_Click_Position ||
      uct == UnitCommandTypes::Set_Rally_Position ||
      uct == UnitCommandTypes::Unload_All_Position ||
      uct == UnitCommandTypes::Use_Tech_Position)
      command = UnitCommand(thisID, uct, targID, command.x & (~0x1F), command.y & (~0x1F), command.extra);
    else if (uct == UnitCommandTypes::Use_Tech_Unit &&   // Group Archon & Dark Archon merging
      (command.getTechType() == TechTypes::Archon_Warp ||
      command.getTechType() == TechTypes::Dark_Archon_Meld))
      command = UnitCommand::useTech(thisID, command.getTechType(), UnitID::None);
  }

  // TODO
  // Set last immediate command again in case it was altered when inserting it into the optimizer
  //static_cast<UnitImpl*>(command.unit)->setLastImmediateCommand(command);

  // Add command to the command optimizer buffer and unload it later (newest commands first)
  optimizerQueue[command.getType().getID()].push_front(command);
  return true;
}

void CommandOptimizer::clear()
{
  for (int i = 0; i < UnitCommandTypes::None; ++i)
    optimizerQueue[i].clear();
}

namespace
{
  // Gets a state to determine whether the particular units are compatible with other units
  // under the same command type.
  bool getCommandStateFlag(BWAPI::UnitCommandTypes::Enum::Enum commandType, BWAPI::Unit unit)
  {
    if (commandType == UnitCommandTypes::Attack_Unit ||
      commandType == UnitCommandTypes::Unload_All ||
      commandType == UnitCommandTypes::Load ||
      commandType == UnitCommandTypes::Cancel_Morph)
    {
      return unit.getType().isBuilding();
    }
    else if (commandType == UnitCommandTypes::Use_Tech)
    {
      return unit.isSieged() || unit.isCloaked() || unit.isBurrowed();
    }
    else
    {
      return false;
    }
  }
}

void CommandOptimizer::flush()
{
  // Iterate the command types
  for (int i = 0; i < UnitCommandTypes::Enum::None; ++i)
  {
    BWAPI::UnitCommandTypes::Enum::Enum cmdType = static_cast<BWAPI::UnitCommandTypes::Enum::Enum>(i);
    // Declare our temporary variables
    Unitset unitsToCommand = {};


    Position  cmpPos = Positions::None;
    int       cmpExtra = 0;
    UnitID    cmpTarget = UnitID::None;
    bool      cmpFlag = false;

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
        Unit cmdUnit = game.getUnit(cmd->getUnit());

        // If we are starting a new command grouping
        if (unitsToCommand.empty())
        {
          // Assign our comparison variables to determine which commands should be grouped
          // Note: Using individual variables instead of comparing UnitCommand operator== because
          //       it will also compare the type which is not necessary, and we may create a new
          //       optimization type that does a fuzzy position comparison
          cmpExtra = cmd->extra;
          cmpTarget = cmd->target;
          cmpPos = cmd->getTargetPosition();
          cmpFlag = getCommandStateFlag(cmdType, cmdUnit);

          unitsToCommand.insert(cmdUnit);

          cmd = optimizerQueue[i].erase(cmd);
        } // otherwise if this command is the same as the first, the units can be grouped
        else if (cmpExtra == cmd->extra && cmpTarget == cmd->target && cmpPos == cmd->getTargetPosition())
        {
          bool oTmp = getCommandStateFlag(cmdType, cmdUnit);
          if (cmpFlag == oTmp)
          {
            unitsToCommand.insert(cmdUnit);
            cmd = optimizerQueue[i].erase(cmd);
          }
          else
          {
            ++cmd;
          }
        } // otherwise skip this command for now
        else
        {
          ++cmd;
        }
      } // second while

      // Forward the command to the client to send it to the server
      if (!unitsToCommand.empty())
      {
        // Workaround for doing certain actions
        UnitID unitID = (i == UnitCommandTypes::Load ||
          i == UnitCommandTypes::Attack_Unit ||
          i == UnitCommandTypes::Train ||
          i == UnitCommandTypes::Unload_All ||
          i == UnitCommandTypes::Cancel_Morph ||
          i == UnitCommandTypes::Use_Tech) ? unitsToCommand.begin()->getID() : UnitID::None;

        // execute command
        game.client.issueCommand(unitsToCommand, UnitCommand(unitID, UnitCommandType(i), cmpTarget, cmpPos.x, cmpPos.y, cmpExtra));
        unitsToCommand.clear();
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
