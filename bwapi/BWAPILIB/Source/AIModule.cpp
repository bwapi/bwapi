#include <BWAPI/AIModule.h>

#include "../../Debug.h"

namespace BWAPI
{
  AIModule::AIModule()
  { }
  AIModule::~AIModule()
  { }
  void AIModule::onStart()
  { }
  void AIModule::onEnd(bool isWinner)
  { }
  void AIModule::onFrame()
  { }
  void AIModule::onSendText(std::string text)
  { }
  void AIModule::onReceiveText(Player* player, std::string text)
  { }
  void AIModule::onPlayerLeft(Player *player)
  { }
  void AIModule::onNukeDetect(Position target)
  { }
  void AIModule::onUnitDiscover(BWAPI::Unit* unit)
  { }
  void AIModule::onUnitEvade(BWAPI::Unit* unit)
  { }
  void AIModule::onUnitShow(BWAPI::Unit* unit)
  { }
  void AIModule::onUnitHide(BWAPI::Unit* unit)
  { }
  void AIModule::onUnitCreate(BWAPI::Unit* unit)
  { }
  void AIModule::onUnitDestroy(BWAPI::Unit* unit)
  { }
  void AIModule::onUnitMorph(BWAPI::Unit* unit)
  { }
  void AIModule::onUnitRenegade(BWAPI::Unit* unit)
  { }
  void AIModule::onSaveGame(std::string gameName)
  { }

  TournamentModule::TournamentModule()
  { }
  TournamentModule::~TournamentModule()
  { }
  bool TournamentModule::onPrintf(char *pszString)
  {
    return true;
  }
  bool TournamentModule::onSendText(char *pszString)
  {
    return true;
  }
  bool TournamentModule::onAction(int actionType, int parameter)
  {
    return true;
  }
  bool TournamentModule::onDraw(int shape, int ctype, int x1, int y1, int x2, int y2, int x3, int y3, Color color, bool isSolid)
  {
    return true;
  }
}
