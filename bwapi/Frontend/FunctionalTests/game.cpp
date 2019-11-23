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
  int framesPassed = 0;
  while (framesPassed < frames)
  {
    update();
    framesPassed++;
  }
}

void FunGame::reset()
{
  //game->removeUnits(game->getAllUnits());
  game->restartGame();
  advance(5);

  // TODO: Game has some state for many things, they all need to be reset here
  // I'm thinking we reinit the GameData, but store the old GameData for use
  // in restoring everything that's only sent on game start.
  // Below code does not preserve player data, which is private so I cannot
  // access it to store and restore it.
  //auto oldGameData = std::move(game->gameData);
  //game->initGameData(); 
  //game->gameData->player = oldGameData->player;
}

BWAPI::Game* FunGame::operator ->()
{
  return game.get();
}
