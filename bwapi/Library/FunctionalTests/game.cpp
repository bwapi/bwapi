#include "game.h"
#include "../Network/BWAPIFrontendClient/ProtoClient.h"

#include <chrono>
#include <thread>

using namespace Funtest;

namespace Funtest
{
  FunGame funGame;
}

FunGame::FunGame()
  : game(std::make_unique<BWAPI::Game>(client))
{
}

void FunGame::connect()
{
  while (!client.connect())
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  update();

  std::cout << "waiting to enter match" << std::endl;
  while (!game->isInGame())
  {
    update();
  }
}

void FunGame::update()
{
  client.update(*game);
}

void FunGame::advance(int frames)
{
  int framesPassed = 0;
  while (framesPassed < frames)
  {
    update();
    framesPassed++;
  }
}

void FunGame::reset()
{
  game->restartGame();
  advance(5); // BUG: Takes 4-5 frames for restartGame to happen
  game->setLocalSpeed(0);
}

BWAPI::Game* FunGame::operator ->()
{
  return game.get();
}
