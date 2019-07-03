#include "gtest/gtest.h"

#include "GameFixture.h"

using namespace BWAPI;

using ::testing::_;

TEST_F(GameFixture, isDebug)
{
#ifdef _DEBUG
  EXPECT_EQ(game.isDebug(), true);
#else
  EXPECT_EQ(game.isDebug(), false);
#endif
}

TEST_F(GameFixture_SmallMap, mapSize)
{
  EXPECT_EQ(game.mapWidth(), 64);
  EXPECT_EQ(game.mapHeight(), 96);
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

TEST_F(GameFixture, setScreenPosition_callsClient)
{
  EXPECT_CALL(client, setScreenPosition(5, 9));
  game.setScreenPosition(5, 9);

  EXPECT_CALL(client, setScreenPosition(10, 20));
  game.setScreenPosition(Position(10, 20));
}

TEST_F(GameFixture, pingMinimap_callsClient)
{
  EXPECT_CALL(client, pingMinimap(5, 9));
  game.pingMinimap(5, 9);

  EXPECT_CALL(client, pingMinimap(10, 20));
  game.pingMinimap(Position(10, 20));
}

TEST_F(GameFixture, printf_callsClient)
{
  std::string expected{ "\x15 chocolate\ncake" };

  EXPECT_CALL(client, printText(expected));
  game.printf("%c chocolate\ncake", Text::Brown);
}

TEST_F(GameFixture, flush_callsPrintf)
{
  EXPECT_CALL(client, printText("\x15 chocolate\ncake"));
  game << static_cast<char>(Text::Brown) << " chocolate";
  game << "\n";
  game << "cake";
  game.flush();
}

TEST_F(GameFixture, ostream_operator_endl_flushes)
{
  EXPECT_CALL(client, printText("test\n"));
  game << "test" << std::endl;
}

TEST_F(GameFixture, ostream_operator_ends_flushes)
{
  EXPECT_CALL(client, printText("who"));
  game << "who" << std::ends;
}

TEST_F(GameFixture, ostream_operator_clearsBufferAfterFlush)
{
  EXPECT_CALL(client, printText("test\n"));
  game << "test" << std::endl;

  EXPECT_CALL(client, printText("what"));
  game << "what" << std::ends;
}

TEST_F(GameFixture, ostream_operator_doesntFlushIfNotExplicit)
{
  EXPECT_CALL(client, printText(_)).Times(0);
  game << "ice cream" << '\n';
}

TEST_F(GameFixture, drawLine_callsClient)
{
  EXPECT_CALL(client, drawShape(ShapeType::Line, CoordinateType::Mouse, 2, 5, 15, 18, 0, 0, Colors::Cyan, false));
  game.drawLine(CoordinateType::Mouse, 2, 5, 15, 18, Colors::Cyan);

  EXPECT_CALL(client, drawShape(ShapeType::Line, CoordinateType::Map, 1, 0, 1, 25, 0, 0, Colors::Red, false)).Times(2);
  game.drawLineMap(1, 0, 1, 25, Colors::Red);
  game.drawLineMap(Position(1, 0), Position(1, 25), Colors::Red);

  EXPECT_CALL(client, drawShape(ShapeType::Line, CoordinateType::Mouse, 1, 0, 1, 25, 0, 0, Colors::Red, false)).Times(2);
  game.drawLineMouse(1, 0, 1, 25, Colors::Red);
  game.drawLineMouse(Position(1, 0), Position(1, 25), Colors::Red);

  EXPECT_CALL(client, drawShape(ShapeType::Line, CoordinateType::Screen, 1, 0, 1, 25, 0, 0, Colors::Red, false)).Times(2);
  game.drawLineScreen(1, 0, 1, 25, Colors::Red);
  game.drawLineScreen(Position(1, 0), Position(1, 25), Colors::Red);
}

