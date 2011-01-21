#include "DevAIModule.h"

using namespace BWAPI;

bool enabled;
int mapH, mapW;
DWORD startTicks;

DWORD dwCount = 0;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);
  enabled = true;

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();
  startTicks = GetTickCount();

  FILE *dIn = fopen("bwapi-data\\logs\\DevCount.txt", "r");
  if ( dIn )
  {
    fscanf(dIn, "%u", &dwCount);
    fclose(dIn);
  }

  ++dwCount;
  FILE *dOut = fopen("bwapi-data\\logs\\DevCount.txt", "w");
  if ( dOut )
  {
    fprintf(dOut, "%u", dwCount);
    fclose(dOut);
  }

  bw->sendText("Beginning run %u.", dwCount);

  // Set speed info
  bw->setLocalSpeed(0);
  bw->setFrameSkip(20);
}

void DevAIModule::onEnd(bool isWinner)
{
  bw->restartGame();
}

DWORD dwLastTickCount;
void DevAIModule::onFrame()
{
  bw->drawTextMap(20, 20, "Runs: %u", dwCount);

  if ( bw->isReplay() )
    return;

  if ( !enabled )
    return;

}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/t" )
  {
    enabled = !enabled;
    Broodwar->printf("DevAITest %s", enabled ? "ENABLED" : "DISABLED");
  }
  else if ( text == "/wikiTypes" )
  {
    writeUnitWiki();
    Broodwar->printf("Printed wiki unit type information");
  }
  else
  {
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
