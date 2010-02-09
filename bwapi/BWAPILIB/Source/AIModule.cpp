#include <BWAPI/AIModule.h>
namespace BWAPI
{
  AIModule::AIModule()
  {
  }
  AIModule::~AIModule()
  {
  }
  void AIModule::onStart()
  {
  }
  void AIModule::onEnd(bool isWinner)
  {
  }
  void AIModule::onFrame()
  {
  }
  bool AIModule::onSendText(std::string text)
  {
    return true;
  }
  void AIModule::onPlayerLeft(Player *player)
  {
  }
  void AIModule::onNukeDetect(Position target)
  {
  }
  void AIModule::onUnitCreate(BWAPI::Unit* unit)
  {
  }
  void AIModule::onUnitDestroy(BWAPI::Unit* unit)
  {
  }
  void AIModule::onUnitMorph(BWAPI::Unit* unit)
  {
  }
  void AIModule::onUnitShow(BWAPI::Unit* unit)
  {
  }
  void AIModule::onUnitHide(BWAPI::Unit* unit)
  {
  }
  void AIModule::onUnitRenegade(BWAPI::Unit* unit)
  {
  }
  void AIModule::onSaveGame(std::string gameName)
  {
  }
}
