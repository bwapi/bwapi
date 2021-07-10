#include <BWAPI.h>
#include "../Network/BWAPIFrontendClient/ProtoClient.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

using namespace BWAPI;

void drawStats(Game &Broodwar);
void drawBullets(Game &Broodwar);
void drawVisibilityData(Game &Broodwar);
void showPlayers(Game &Broodwar);
void showForces(Game &Broodwar);
bool show_bullets;
bool show_visibility_data;


void reconnect(Client &BWAPIClient)
{
  while (!BWAPIClient.connect())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
  }
}

int main()
{
  ProtoClient BWAPIClient;
  Game Broodwar(BWAPIClient);
  std::cout << "Connecting..." << std::endl;
  reconnect(BWAPIClient);
  while (true)
  {
    std::cout << "waiting to enter match" << std::endl;
    while (!Broodwar->isInGame())
    {
      while (!BWAPIClient.isConnected())
      {
        std::cout << "Reconnecting..." << std::endl;
        reconnect(BWAPIClient);
      }
      BWAPIClient.update(Broodwar);
    }
    std::cout << "starting match!" << std::endl;
    Broodwar->sendText("Hello world!");
    Broodwar << "The map is " << Broodwar->mapName() << ", a " << Broodwar->getStartLocations().size() << " player map" << std::endl;

    show_bullets = true;
    show_visibility_data = false;

    if (Broodwar->isReplay())
    {
      Broodwar << "The following players are in this replay:" << std::endl;
      Playerset players = Broodwar->getPlayers();
      for (Player p : players)
      {
        if (!p->getUnits().empty() && !p->isNeutral())
          Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
      }
    }
    else
    {
      if (Broodwar->enemy())
        Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;

      //send each worker to the mineral field that is closest to it
      Unitset units = Broodwar->self()->getUnits();
      Unitset minerals = Broodwar->getMinerals();
      for (Unit u : units)
      {
        if (u->getType().isWorker())
        {
          Unit closestMineral = nullptr;

          for (Unit m : minerals)
          {
            if (!closestMineral || u->getDistance(m) < u->getDistance(closestMineral))
              closestMineral = m;
          }
          if (closestMineral)
            u->rightClick(closestMineral);

        }
        else if (u->getType().isResourceDepot())
        {
          //if this is a center, tell it to build the appropriate type of worker
          u->train(Broodwar->self()->getRace().getWorker());
        }
      }
    }
    while (Broodwar->isInGame())
    {
      for (auto &e : Broodwar->getEvents())
      {
        switch (e.getType())
        {
        case EventType::MatchEnd:
          if (e.isWinner())
            Broodwar << "I won the game" << std::endl;
          else
            Broodwar << "I lost the game" << std::endl;
          break;
        case EventType::SendText:
          if (e.getText() == "/show bullets")
            show_bullets = !show_bullets;
          else if (e.getText() == "/show players")
            showPlayers(Broodwar);
          else if (e.getText() == "/show forces")
            showForces(Broodwar);
          else if (e.getText() == "/show visibility")
            show_visibility_data = !show_visibility_data;
          else
            Broodwar << "You typed \"" << e.getText() << "\"!" << std::endl;
          break;
        case EventType::ReceiveText:
          Broodwar << e.getPlayer()->getName() << " said \"" << e.getText() << "\"" << std::endl;
          break;
        case EventType::PlayerLeft:
          Broodwar << e.getPlayer()->getName() << " left the game." << std::endl;
          break;
        case EventType::NukeDetect:
          if (e.getPosition() != Positions::Unknown)
          {
            Broodwar->drawCircleMap(e.getPosition(), 40, Colors::Red, true);
            Broodwar << "Nuclear Launch Detected at " << e.getPosition() << std::endl;
          }
          else
            Broodwar << "Nuclear Launch Detected" << std::endl;
          break;
        case EventType::UnitCreate:
          if (!Broodwar->isReplay())
            Broodwar << "A " << e.getUnit()->getType().c_str() << " [" << e.getUnit().getID().id << "] has been created at " << e.getUnit()->getPosition() << std::endl;
          else
          {
            // if we are in a replay, then we will print out the build order
            // (just of the buildings, not the units).
            if (e.getUnit()->getType().isBuilding() && e.getUnit()->getPlayer()->isNeutral() == false)
            {
              int seconds = Broodwar->getFrameCount() / 24;
              int minutes = seconds / 60;
              seconds %= 60;
              std::string playerName;
              playerName = e.getUnit()->getPlayer().getName();
              Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, playerName.c_str(), e.getUnit()->getType().c_str());
            }
          }
          break;
          case EventType::UnitDestroy:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%p] has been destroyed at (%d,%d)", e.getUnit()->getType().c_str(), e.getUnit(), e.getUnit()->getPosition().x, e.getUnit()->getPosition().y);
            break;
          case EventType::UnitMorph:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%p] has been morphed at (%d,%d)", e.getUnit()->getType().c_str(), e.getUnit(), e.getUnit()->getPosition().x, e.getUnit()->getPosition().y);
            else
            {
              // if we are in a replay, then we will print out the build order
              // (just of the buildings, not the units).
              if (e.getUnit()->getType().isBuilding() && e.getUnit()->getPlayer()->isNeutral() == false)
              {
                int seconds = Broodwar->getFrameCount() / 24;
                int minutes = seconds / 60;
                seconds %= 60;
                std::string playerName;
                playerName = e.getUnit()->getPlayer()->getName();
                Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, playerName.c_str(), e.getUnit()->getType().c_str());
              }
            }
            break;
          case EventType::UnitShow:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%p] has been spotted at (%d,%d)", e.getUnit()->getType().c_str(), e.getUnit(), e.getUnit()->getPosition().x, e.getUnit()->getPosition().y);
            break;
          case EventType::UnitHide:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%p] was last seen at (%d,%d)", e.getUnit()->getType().c_str(), e.getUnit(), e.getUnit()->getPosition().x, e.getUnit()->getPosition().y);
            break;
          case EventType::UnitRenegade:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%p] is now owned by %s", e.getUnit()->getType().c_str(), e.getUnit(), e.getUnit()->getPlayer()->getName());
            break;
          case EventType::SaveGame:
            Broodwar->sendText("The game was saved to \"%s\".", e.getText().c_str());
            break;
        }
      }

      if (show_bullets)
        drawBullets(Broodwar);

      if (show_visibility_data)
        drawVisibilityData(Broodwar);

      drawStats(Broodwar);
      Broodwar->drawTextScreen(300, 0, "FPS: %f", Broodwar->getAverageFPS());

      BWAPIClient.update(Broodwar);
      if (!BWAPIClient.isConnected())
      {
        std::cout << "Reconnecting..." << std::endl;
        reconnect(BWAPIClient);
      }
    }
    std::cout << "Game ended" << std::endl;
  }
  std::cout << "Press ENTER to continue..." << std::endl;
  std::cin.ignore();
  return 0;
}

void drawStats(Game &Broodwar)
{
  int line = 0;
  Broodwar->drawTextScreen(5, 0, "I have %d units:", Broodwar->self()->allUnitCount());
  for (UnitType unitType : UnitTypes::allUnitTypes())
  {
    int count = Broodwar->self()->allUnitCount(unitType);
    if (count)
    {
      Broodwar->drawTextScreen(5, 16 * line, "- %d %s%c", count, unitType.c_str(), count == 1 ? ' ' : 's');
      line++;
    }
  }
}

void drawBullets(Game &Broodwar)
{
  for (Bullet b : Broodwar->getBullets())
  {
    Position p = b->getPosition();
    double velocityX = b->getVelocityX();
    double velocityY = b->getVelocityY();
    Broodwar.drawLineMap(p, p + Position{ static_cast<int>(velocityX), static_cast<int>(velocityY) }, b->getPlayer() == Broodwar->self() ? Colors::Green : Colors::Red);
    Broodwar.drawTextMap(p, "%c%s", b->getPlayer() == Broodwar->self() ? Text::Green : Text::Red, b->getType().c_str());
  }
}

void drawVisibilityData(Game &Broodwar)
{
  int wid = Broodwar.mapHeight(), hgt = Broodwar.mapWidth();
  for (int x = 0; x < wid; ++x)
    for (int y = 0; y < hgt; ++y)
    {
      if (Broodwar.isExplored(x, y))
        Broodwar.drawDotMap(x * 32 + 16, y * 32 + 16, Broodwar.isVisible(x, y) ? Colors::Green : Colors::Blue);
      else
        Broodwar.drawDotMap(x * 32 + 16, y * 32 + 16, Colors::Red);
    }
}

void showPlayers(Game &Broodwar)
{
  Playerset players = Broodwar.getPlayers();
  for (Player p : players)
    Broodwar << "Player [" << p->getID().id << "]: " << p->getName() << " is in force: " << p->getForce()->getName() << std::endl;
}

void showForces(Game &Broodwar)
{
  Forceset forces = Broodwar.getForces();
  for (Force f : forces)
  {
    Playerset players = f->getPlayers();
    Broodwar << "Force " << f->getName() << " has the following players:" << std::endl;
    for (Player p : players)
      Broodwar << "  - Player [" << p->getID().id << "]: " << p->getName() << std::endl;
  }
}