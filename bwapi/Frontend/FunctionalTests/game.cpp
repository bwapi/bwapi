#include "game.h"
#include "..\Frontend\BWAPIFrontendClient\ProtoClient.h"

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
  int targetFrame = game->getFrameCount() + frames;
  while (game->getFrameCount() < targetFrame)
  {
    update();
  }
}

void FunGame::reset()
{
  game->removeUnits(game->getAllUnits());

  // TODO: Game has some state for many things, they all need to be reset here
}

BWAPI::Game* FunGame::operator ->()
{
  return game.get();
}
