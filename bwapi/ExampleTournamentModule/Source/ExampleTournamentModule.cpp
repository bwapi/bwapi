#include "ExampleTournamentModule.h"
using namespace BWAPI;

bool leader = false;


void ExampleTournamentAI::onStart()
{
  // Set the command optimization level (reduces high APM, size of bloated replays, etc)
  Broodwar->setCommandOptimizationLevel(MINIMUM_COMMAND_OPTIMIZATION);
}

int maxAPM;
void ExampleTournamentAI::onEnd(bool isWinner)
{
  // save maxAPM or something
}

void ExampleTournamentAI::onFrame()
{
  int thisAPM = Broodwar->getAPM();
  if ( thisAPM > maxAPM )
    maxAPM = thisAPM;

  // If the elapsed game time has exceeded 20 minutes
  if ( Broodwar->elapsedTime() > 20 * 60 ) 
  {
    /*// Possibly save the player's score
      Broodwar->self()->getUnitScore();
      Broodwar->self()->getKillScore();
      Broodwar->self()->getBuildingScore();
      Broodwar->self()->getRazingScore();  */
    Broodwar->sendText("DRAW");
    Broodwar->leaveGame();
  }
}

void ExampleTournamentAI::onSendText(std::string text)
{
}

void ExampleTournamentAI::onReceiveText(BWAPI::Player player, std::string text)
{
}

void ExampleTournamentAI::onPlayerLeft(BWAPI::Player player)
{
}

void ExampleTournamentAI::onNukeDetect(BWAPI::Position target)
{
}

void ExampleTournamentAI::onUnitDiscover(BWAPI::Unit unit)
{
}

void ExampleTournamentAI::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleTournamentAI::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleTournamentAI::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleTournamentAI::onUnitCreate(BWAPI::Unit unit)
{
}

void ExampleTournamentAI::onUnitDestroy(BWAPI::Unit unit)
{
}

void ExampleTournamentAI::onUnitMorph(BWAPI::Unit unit)
{
}

void ExampleTournamentAI::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleTournamentAI::onSaveGame(std::string gameName)
{}

bool ExampleTournamentModule::onAction(BWAPI::Tournament::ActionID actionType, void *parameter)
{
  switch ( actionType )
  {
  case Tournament::SendText:
  case Tournament::Printf:
    // Allow the AI module to send text
    return true;
  case Tournament::EnableFlag:
    switch ( *(int*)parameter )
    {
    case Flag::CompleteMapInformation:
    case Flag::UserInput:
      // Disallow these two flags
      return false;
    }
    // Allow other flags if we add more that don't affect gameplay specifically
    return true;
  case Tournament::PauseGame:
  case Tournament::ResumeGame:
  case Tournament::SetFrameSkip:
  case Tournament::SetGUI:
  case Tournament::SetLocalSpeed:
  case Tournament::SetMap:
    return false; // Disallow these actions
  case Tournament::LeaveGame:
  case Tournament::SetLatCom:
  case Tournament::SetTextSize:
    return true; // Allow these actions
  case Tournament::SetCommandOptimizationLevel:
    return *(int*)parameter > MINIMUM_COMMAND_OPTIMIZATION; // Set a minimum command optimization level 
                                                            // to reduce APM with no action loss
  default:
    break;
  }
  return true;
}

void ExampleTournamentModule::onFirstAdvertisement()
{
  leader = true;
  Broodwar->sendText("Welcome to " TOURNAMENT_NAME "!");
  Broodwar->sendText("Brought to you by " SPONSORS ".");
}
