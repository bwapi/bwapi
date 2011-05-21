#pragma once
#include <BWAPI.h>

class ExampleTournamentModule : public BWAPI::TournamentModule
{
  virtual bool onPrintf(char *pszString);
  virtual bool onSendText(char *pszString);
  virtual bool onAction(int actionType, int parameter);
  virtual bool onDraw(int shape, int ctype, int x1, int y1, int x2, int y2, int x3, int y3, BWAPI::Color color, bool isSolid);
};

class ExampleTournamentAI : public BWAPI::AIModule
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
};
