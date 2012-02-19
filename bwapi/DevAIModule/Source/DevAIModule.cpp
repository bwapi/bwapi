#include "DevAIModule.h"
#include <set>

using namespace BWAPI;

bool enabled;
int mapH, mapW;

DWORD dwCount = 0;
int bestFPS;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();

  bw->printf("%u", bw->getBullets().size());
}

void DevAIModule::onEnd(bool isWinner)
{

}

DWORD dwLastTickCount;
bool testunload;
void DevAIModule::onFrame()
{
	if ( bw->isReplay() )
		return;

	int tFPS = bw->getFPS();
	if ( tFPS > bestFPS )
		bestFPS = tFPS;

	for ( std::set<Unit*>::const_iterator i = self->getUnits().begin(); i != self->getUnits().end(); ++i )
	{
		UnitType utype = (*i)->getType();
		if ( utype.isBuilding() )
		{
			// draw radius
			Position pos = (*i)->getPosition();
			bw->drawEllipseMap(pos.x(), pos.y(), 320 + (utype.dimensionLeft() + 1 + utype.dimensionRight()) / 2, 320 + (utype.dimensionUp() + 1 + utype.dimensionDown()) / 2, Colors::Green);
			
			// draw line to each in radius
			std::set<Unit*> radiusSet = (*i)->getUnitsInRadius(320);
			for ( std::set<Unit*>::iterator u = radiusSet.begin(); u != radiusSet.end(); ++u )
			{
				Position targPos = (*u)->getPosition();
				bw->drawLineMap(pos.x(), pos.y(), targPos.x(), targPos.y(), Colors::Yellow);
			}
		}
	}
}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/wiki" )
  {
    writeUnitWiki();
    writeWeaponWiki();
    Broodwar->printf("Generated wiki pages!");
  }
  else if ( text == "/best" )
  {
    bw->printf("Best: %d FPS", bestFPS);
  }
  else if ( text == "/races" )
  {
    for ( std::set<Player*>::const_iterator p = bw->getPlayers().begin(),
          pend = bw->getPlayers().end();
          p != pend;
          ++p )
    {
      bw->printf("%s is %s", (*p)->getName().c_str(), (*p)->getRace().c_str());
    }
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
