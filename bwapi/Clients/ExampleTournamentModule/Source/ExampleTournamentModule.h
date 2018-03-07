#pragma once
#include <BWAPI.h>

#define TOURNAMENT_NAME "AIIDE 2012"
#define SPONSORS "the BWAPI Project Team, and Blizzard Entertainment"
#define MINIMUM_COMMAND_OPTIMIZATION 1

class ExampleTournamentModule : public BWAPI::TournamentModule
{
  virtual bool onAction(BWAPI::Tournament::ActionID actionType, void *parameter = nullptr);
  virtual void onFirstAdvertisement();
};

class ExampleTournamentAI : public BWAPI::AIModule
{
public:
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onSendText(std::string text);
  virtual void onReceiveText(BWAPI::Player player, std::string text);
  virtual void onPlayerLeft(BWAPI::Player player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitDiscover(BWAPI::Unit unit);
  virtual void onUnitEvade(BWAPI::Unit unit);
  virtual void onUnitShow(BWAPI::Unit unit);
  virtual void onUnitHide(BWAPI::Unit unit);
  virtual void onUnitCreate(BWAPI::Unit unit);
  virtual void onUnitDestroy(BWAPI::Unit unit);
  virtual void onUnitMorph(BWAPI::Unit unit);
  virtual void onUnitRenegade(BWAPI::Unit unit);
  virtual void onSaveGame(std::string gameName);
};
