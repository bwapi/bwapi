#include <BWAPI/AIModule.h>

namespace BWAPI
{
  AIModule::AIModule()
  { }
  AIModule::~AIModule()
  { }
  void AIModule::onStart()
  { }
  void AIModule::onEnd(bool)
  { }
  void AIModule::onFrame()
  { }
  void AIModule::onSendText(std::string)
  { }
  void AIModule::onReceiveText(Player, std::string)
  { }
  void AIModule::onPlayerLeft(Player)
  { }
  void AIModule::onNukeDetect(Position)
  { }
  void AIModule::onUnitDiscover(BWAPI::Unit )
  { }
  void AIModule::onUnitEvade(BWAPI::Unit )
  { }
  void AIModule::onUnitShow(BWAPI::Unit )
  { }
  void AIModule::onUnitHide(BWAPI::Unit )
  { }
  void AIModule::onUnitCreate(BWAPI::Unit )
  { }
  void AIModule::onUnitDestroy(BWAPI::Unit )
  { }
  void AIModule::onUnitMorph(BWAPI::Unit )
  { }
  void AIModule::onUnitRenegade(BWAPI::Unit )
  { }
  void AIModule::onSaveGame(std::string)
  { }
  void AIModule::onUnitComplete(BWAPI::Unit )
  { }

  TournamentModule::TournamentModule()
  { }
  TournamentModule::~TournamentModule()
  { }
  bool TournamentModule::onAction(BWAPI::Tournament::ActionID, void*)
  {
    return true;
  }
  void TournamentModule::onFirstAdvertisement()
  { }
}
