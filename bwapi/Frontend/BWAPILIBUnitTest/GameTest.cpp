#include "gtest/gtest.h"

#include "GameFixture.h"

using namespace BWAPI;

TEST_F(GameFixture, isDebug)
{
#ifdef _DEBUG
  EXPECT_EQ(game.isDebug(), true);
#else
  EXPECT_EQ(game.isDebug(), false);
#endif
}

TEST_F(GameFixture_SmallMap, isValid)
{
  // Origin
  EXPECT_TRUE(game.isValid(Positions::Origin));
  
  // Negative
  EXPECT_FALSE(game.isValid(WalkPosition(-1, 0)));
  EXPECT_FALSE(game.isValid(WalkPosition(0, -1)));

  // Positive
  EXPECT_TRUE(game.isValid(TilePosition(63, 95)));
  EXPECT_FALSE(game.isValid(TilePosition(64, 95)));
  EXPECT_FALSE(game.isValid(TilePosition(63, 96)));

  int pixelWidth = 64 * 32;
  int pixelHeight = 96 * 32;

  EXPECT_TRUE(game.isValid(Position(pixelWidth - 1, pixelHeight - 1)));
  EXPECT_FALSE(game.isValid(Position(pixelWidth, pixelHeight - 1)));
  EXPECT_FALSE(game.isValid(Position(pixelWidth - 1, pixelHeight)));
}

TEST_F(GameFixture_SmallMap, makeValid)
{
  // Double tests here are to ensure that both a value is returned and the original is modified

  // Origin
  {
    Position origin = Positions::Origin;
    EXPECT_EQ(game.makeValid(origin), Positions::Origin);
    EXPECT_EQ(origin, Positions::Origin);
  }
  // Negative
  {
    WalkPosition pos{ -1, -37 };
    EXPECT_EQ(game.makeValid(pos), WalkPositions::Origin);
    EXPECT_EQ(pos, WalkPositions::Origin);
  }
  // Positive
  {
    TilePosition expected{ 63,95 };
    TilePosition pos{ 64, 96 };
    
    EXPECT_EQ(game.makeValid(pos), expected);
    EXPECT_EQ(pos, expected);
  }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// Game Actions
///////////////////////////////////////////////////////////////////////
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
