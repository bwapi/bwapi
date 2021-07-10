#include <BWAPI.h>
#include "../Network/BWAPIFrontendClient/ProtoClient.h"

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
  if (!BWAPIClient.isRemaster())
    BWAPIClient.update(broodwar);
  std::cout << "waiting to enter match" << std::endl;
  if (BWAPIClient.isRemaster())
    BWAPIClient.createGameRequest(true, "D:/bwapifacebook/Data/Maps/(2)Bottleneck.scm", 2, true, Races::Terran);

    //BWAPIClient.createGameRequest(false, "maps/sscai/(4)Fighting Spirit.scx", 2, true, Races::Terran);

  while (BWAPIClient.isConnected())
  {
    while (!broodwar.isInGame())
    {
      BWAPIClient.gameInfoRequest();
      BWAPIClient.update(broodwar);
    }
    std::cout << "Starting match!" << std::endl;

    self = broodwar.self();
    mapH = broodwar->mapHeight();
    mapW = broodwar->mapWidth();
    //broodwar->setCommandOptimizationLevel(0);
    //broodwar << "Random Seed: " << broodwar.getRandomSeed() << std::endl;
    while (broodwar.isInGame())
    {
      if (!BWAPIClient.isRemaster())
      {
        int i = 1;
        for (auto player : broodwar.getPlayers()) {
          broodwar.drawTextScreen(10, i * 16, "%cPlayer %d text @@@@ TEXT", player.getTextColor(), player.getID());
          i++;
        }

        /*
        BWAPI::Color colors[13] = {
          Colors::Red, Colors::Blue, Colors::Teal, Colors::Purple, Colors::Orange, Colors::Brown, Colors::WhitePlayer, Colors::Yellow, Colors::Green, Colors::Cyan, Colors::White, Colors::Black, Colors::Grey
        };
        for (int i = 0; i < 13; ++i) {
          broodwar.drawBoxScreen(i * 20, 0, 19 + i * 20, 100, colors[i], true);
        }*/
        /*
        for (int y = 0; y < 16; ++y) {
          for (int x = 0; x < 16; ++x) {
            broodwar.drawBoxScreen(x * 32, y * 16, x * 32 + 31, y * 16 + 15, Color{y*16 + x}, true);
            broodwar.drawTextScreen(x * 32, y * 16, "%d", y * 16 + x);
          }
        }
        */
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
      }
      BWAPIClient.update(broodwar);
    }
  }
  return 1;
}