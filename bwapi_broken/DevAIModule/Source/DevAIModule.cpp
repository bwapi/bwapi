#include "DevAIModule.h"

using namespace BWAPI;

void DevAIModule::onStart()
{
  Broodwar->enableFlag(Flag::UserInput);

}

void DevAIModule::onEnd(bool isWinner)
{
}

void DevAIModule::onFrame()
{
  if (Broodwar->isReplay())
    return;

}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/ver" )
  {
    Broodwar->printf("Heinermann DevTest");
  }
  else
  {
    // WTF doesn't work?????
    Broodwar->sendText("%s", text.c_str());
  }
}

void DevAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
}

void DevAIModule::onPlayerLeft(BWAPI::Player* player)
{
}

void DevAIModule::onNukeDetect(BWAPI::Position target)
{
}

void DevAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitEvade(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitShow(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitHide(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitCreate(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitMorph(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
}

void DevAIModule::onSaveGame(std::string gameName)
{
}
