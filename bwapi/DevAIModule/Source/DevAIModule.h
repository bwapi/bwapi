#pragma once
#include <BWAPI.h>

#define bw Broodwar

#define SEARCH_UNEXPLORED   0
#define SEARCH_EXPLORED     1
#define SEARCH_NOTVISIBLE   2

class DevAIModule : public BWAPI::AIModule
{
public:
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onSendText(std::string text);
  virtual void onReceiveText(BWAPI::Player* player, std::string text);
  virtual void onPlayerLeft(BWAPI::Player* player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitDiscover(BWAPI::Unit* unit);
  virtual void onUnitEvade(BWAPI::Unit* unit);
  virtual void onUnitShow(BWAPI::Unit* unit);
  virtual void onUnitHide(BWAPI::Unit* unit);
  virtual void onUnitCreate(BWAPI::Unit* unit);
  virtual void onUnitDestroy(BWAPI::Unit* unit);
  virtual void onUnitMorph(BWAPI::Unit* unit);
  virtual void onUnitRenegade(BWAPI::Unit* unit);
  virtual void onSaveGame(std::string gameName);

  bool                pointSearch(int dwType, BWAPI::TilePosition pt, BWAPI::Unit *unit = NULL, int width = 1, int height = 1);
  BWAPI::TilePosition spiralSearch(int dwType, BWAPI::TilePosition start, int radius, BWAPI::Unit *unit = NULL, int width = 1, int height = 1);

  BWAPI::Player *self;

  BWAPI::Unit *scout;
};
