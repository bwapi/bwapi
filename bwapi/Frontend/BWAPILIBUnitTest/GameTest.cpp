#include "gtest/gtest.h"

#include "GameFixture.h"

using namespace BWAPI;

TEST_F(GameFixture, pauseGame_callsClient)
{
  EXPECT_CALL(client, pauseGame());
  game.pauseGame();
}

TEST_F(GameFixture, resumeGame_callsClient)
{
  EXPECT_CALL(client, resumeGame());
  game.resumeGame();
}

TEST_F(GameFixture, leaveGame_callsClient)
{
  EXPECT_CALL(client, leaveGame());
  game.leaveGame();
}

TEST_F(GameFixture, restartGame_callsClient)
{
  EXPECT_CALL(client, restartGame());
  game.restartGame();
}
