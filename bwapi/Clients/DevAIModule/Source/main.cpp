#include <BWAPI.h>
#include "..\Frontend\BWAPIFrontendClient\ProtoClient.h"

#include <iostream>

using namespace BWAPI;

bool enabled;
int mapH, mapW;

unsigned dwCount = 0;

Player self = nullptr;

int main()
{
  ProtoClient BWAPIClient;
  Game broodwar(BWAPIClient);
  std::cout << "Connecting..." << std::endl;
  while (!BWAPIClient.connect())
  {
  }
  BWAPIClient.update(broodwar);
  std::cout << "waiting to enter match" << std::endl;
  while (BWAPIClient.isConnected())
  {
    while (!broodwar.isInGame())
    {
      BWAPIClient.update(broodwar);
    }
    std::cout << "Starting match!" << std::endl;
    self = broodwar.self();
    mapH = broodwar->mapHeight();
    mapW = broodwar->mapWidth();
    broodwar->setCommandOptimizationLevel(0);
    broodwar << "Random Seed: " << broodwar.getRandomSeed() << std::endl;
    while (broodwar.isInGame())
    {
      //std::cout << "frame " << broodwar.getFrameCount() << std::endl;
      for (auto& e : broodwar.getEvents())
      {
        switch (e.getType())
        {
        case EventType::MatchEnd:
          std::cout << "MatchEnd" << std::endl;
          break;
        case EventType::MatchStart:
          std::cout << "MatchStart" << std::endl;
          break;
        case EventType::NukeDetect:
          std::cout << "NukeDetect" << std::endl;
          break;
        case EventType::PlayerLeft:
          std::cout << "PlayerLeft" << std::endl;
          break;
        case EventType::ReceiveText:
          std::cout << "ReceiveText: " << e.getText() << std::endl;
          break;
        case EventType::SaveGame:
          std::cout << "SaveGame: " << e.getText() << std::endl;
          break;
        case EventType::SendText:
        {
          std::cout << "SendText: " << e.getText().c_str() << std::endl;
          std::istringstream ss(e.getText());
          std::string cmd;
          ss >> cmd;
          if (cmd == "/morph")
          {
            Unitset larvae = self->getUnits();
            larvae.erase_if(Filter::GetType != UnitTypes::Zerg_Larva);
            if (!larvae.empty())
            {
              if (!(*larvae.begin())->morph(UnitTypes::Zerg_Mutalisk))
              {
                broodwar << broodwar.getLastError() << ":" << self->incompleteUnitCount(UnitTypes::Zerg_Greater_Spire) << ":" << self->incompleteUnitCount(UnitTypes::Zerg_Spire) << std::endl;
              }
            }
          }
          else if (cmd == "/kill")
          {
            for (Unit u : self.getUnits())
            {
              broodwar.printf("Killing %s", u->getType().c_str());
              broodwar.killUnit(u);
            }
          }
          else if (cmd == "/remove")
          {
            for (Unit u : self.getUnits())
            {
              broodwar.printf("Removing %s", u->getType().c_str());
              broodwar.removeUnit(u);
            }
          }
          else if (cmd == "/create")
          {
            int count = 1;
            std::string unitTypeName;
            ss >> count >> unitTypeName;

            Position pos = broodwar.getScreenPosition();// +broodwar.getMousePosition();
            UnitType unitType = UnitType::getType(unitTypeName);

            broodwar.printf("Attempted to create %s at (%d, %d)", unitType.c_str(), pos.x, pos.y);

            broodwar.createUnit(broodwar.self(), unitType, pos.x, pos.y, count);
          }
          else if (e.getText() == "/pause")
          {
            broodwar->pauseGame();
          }
          else if (e.getText() == "/unpause")
          {
            broodwar->resumeGame();
          }
          else
          {
            broodwar->sendText("%s", e.getText().c_str());
          }
          break;
        }
        case EventType::UnitDiscover:
          if (e.getUnit()->getType() == UnitTypes::Spell_Scanner_Sweep)
          {
            broodwar << "discover @ " << e.getUnit()->getPosition() << std::endl;
          }
          std::cout << "UnitDiscover: " << e.getUnit()->getType() << std::endl;
          break;
        case EventType::UnitEvade:
          if (e.getUnit()->getType() == UnitTypes::Spell_Scanner_Sweep)
          {
            broodwar << "evade @ " << e.getUnit()->getPosition() << std::endl;
          }
          std::cout << "UnitEvade: " << e.getUnit()->getType() << std::endl;
          break;
        case EventType::UnitCreate:
          if (e.getUnit()->getType() == UnitTypes::Spell_Scanner_Sweep)
          {
            broodwar << "create @ " << e.getUnit()->getPosition() << std::endl;
          }
          std::cout << "UnitCreate: " << e.getUnit()->getType() << std::endl;
          break;
        case EventType::UnitDestroy:
          if (e.getUnit()->getType() == UnitTypes::Spell_Scanner_Sweep)
          {
            broodwar << "show @ " << e.getUnit()->getPosition() << std::endl;
          }
          std::cout << "UnitDestroy: " << e.getUnit()->getType() << std::endl;
          break;
        case EventType::UnitShow:
          if (e.getUnit()->getType() == UnitTypes::Spell_Scanner_Sweep)
          {
            broodwar << "show @ " << e.getUnit()->getPosition() << std::endl;
          }
          std::cout << "UnitShow: " << e.getUnit()->getType() << std::endl;
          break;
        case EventType::UnitHide:
          if (e.getUnit()->getType() == UnitTypes::Spell_Scanner_Sweep)
          {
            broodwar << "hide @ " << e.getUnit()->getPosition() << std::endl;
          }
          std::cout << "UnitHide: " << e.getUnit()->getType() << std::endl;
          break;
        default:
          break;
        }
      }
      BWAPIClient.update(broodwar);
      if (!broodwar.isReplay())
      {
        for (Unit u : broodwar.getAllUnits())
        {
          if (u->getType() == UnitTypes::Spell_Scanner_Sweep) {
            broodwar << "SCANNER @ " << u->getPosition() << std::endl;
          }
        }

        Unitset workers = self.getUnits();
        workers.erase_if(!Filter::IsWorker || !Filter::IsIdle);
        for (Unit u : workers)
        {
          if (!u->gather(u->getClosestUnit(Filter::IsMineralField)))
          {
            //std::cout << broodwar.getLastError() << std::endl;
          }
        }
        if (broodwar.self()->completedUnitCount(UnitTypes::Terran_Factory) < 1)
          broodwar.createUnit(broodwar.self(), BWAPI::UnitTypes::Terran_Factory, static_cast<Position>(broodwar.getBuildLocation(BWAPI::UnitTypes::Terran_Factory, broodwar.self().getStartLocation())));
        if (broodwar.self()->hasResearched(TechTypes::Tank_Siege_Mode))
          std::cout << "I know siege mode!" << std::endl;
      }
    }
  }
  return 1;
}