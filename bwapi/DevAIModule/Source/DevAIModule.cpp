#include "DevAIModule.h"

using namespace BWAPI;

void DevAIModule::onStart()
{
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->printf("%s", Broodwar->mapHash().c_str());
  overflow = false;
}

void DevAIModule::onEnd(bool isWinner)
{
}

void DevAIModule::onFrame()
{
  if (Broodwar->isReplay())
    return;

  if ( overflow )
  {
    Broodwar->printf("<-- Begin overflow test");

    Broodwar->canBuildHere(NULL, TilePosition(), NULL);
    Broodwar->canMake(NULL, NULL);
    Broodwar->canResearch(NULL, NULL);
    Broodwar->canUpgrade(NULL, NULL);
    Broodwar->changeRace(NULL);
    Broodwar->hasCreep(333, 666);
    Broodwar->hasPower(333, 666, 333, 666);
    Broodwar->pingMinimap(22, 44);

    Broodwar->self()->allUnitCount(NULL);
    Broodwar->self()->allUnitCount(9999999);
    Broodwar->self()->completedUnitCount(NULL);
    Broodwar->self()->completedUnitCount(9999999);
    Broodwar->self()->deadUnitCount(NULL);
    Broodwar->self()->deadUnitCount(9999999);
    Broodwar->self()->getUpgradeLevel(NULL);
    Broodwar->self()->getUpgradeLevel(9999999);
    Broodwar->self()->hasResearched(NULL);
    Broodwar->self()->hasResearched(9999999);
    Broodwar->self()->incompleteUnitCount(NULL);
    Broodwar->self()->incompleteUnitCount(9999999);
    Broodwar->self()->isAlly(NULL);
    Broodwar->self()->isEnemy(NULL);
    Broodwar->self()->isResearching(NULL);
    Broodwar->self()->isResearching(9999999);
    Broodwar->self()->isUpgrading(NULL);
    Broodwar->self()->isUpgrading(9999999);
    Broodwar->self()->killedUnitCount(NULL);
    Broodwar->self()->killedUnitCount(9999999);
    Broodwar->self()->maxEnergy(NULL);
    Broodwar->self()->maxEnergy(9999999);
    Broodwar->self()->supplyTotal(NULL);
    Broodwar->self()->supplyTotal(9999999);
    Broodwar->self()->supplyUsed(NULL);
    Broodwar->self()->supplyUsed(9999999);
    Broodwar->printf("End overflow test -->");
    overflow = false;
  }

  for each ( Unit *u in Broodwar->self()->getUnits() )
  {
    BWAPI::UnitType uType = u->getType();
    if ( uType.isResourceDepot() && u->isIdle() )
    {
      BWAPI::UnitType workerType = uType.getRace().getWorker();
      if ( Broodwar->canMake(NULL, workerType) )
      {
        u->train(workerType);
      }
    }

    if ( Broodwar->self()->supplyTotal(uType.getRace()) <= Broodwar->self()->supplyUsed(uType.getRace()) )
    {
      UnitType supplyType = uType.getRace().getSupplyProvider();
      if ( Broodwar->canMake(NULL, supplyType) && uType == supplyType.whatBuilds().first )
      {
        if ( uType == UnitTypes::Zerg_Larva )
        {
          u->morph(supplyType);
        }
        else
        {
          //u->build(NULL, supplyType);
        }
      }
    }

    if ( uType.isWorker() && u->isIdle() )
    {
      Unit *best = NULL;
      for each ( Unit *r in Broodwar->getNeutralUnits() )
      {
        if ( r->getType() == UnitTypes::Resource_Mineral_Field )
        {
          if ( u->getDistance(r) < u->getDistance(best) && !r->isBeingGathered() )
            best = r;
        }
      }
      if ( best )
        u->gather(best);
    }
  }

}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/ver" )
  {
    Broodwar->printf("Heinermann DevTest");
  }
  else if ( text == "/overflow" )
  {
    overflow = true;
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
