#include <gtest/gtest.h>

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

TEST_F(GameFixture_SmallMap, makeValidRvalueRef)
{
  Position expected{ 8,3 };
  EXPECT_EQ(game.makeValid(Positions::Origin + Position{ 8,3 }), expected);
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
namespace GameTest::printf {
  TEST_F(Game_printf, printf_callsClient)
  {
    std::string expected{ "\x15 chocolate\ncake" };

    EXPECT_CALL(client, printText(expected));
    game.printf("%c chocolate\ncake", Text::Brown);
  }
}
namespace GameTest::ostream {
  TEST_F(Game_ostream, flush_callsPrintf)
  {
    EXPECT_CALL(client, printText("\x15 chocolate\ncake"));
    game << static_cast<char>(Text::Brown) << " chocolate";
    game << "\n";
    game << "cake";
    game.flush();
  }

  TEST_F(Game_ostream, ostream_operator_endl_flushes)
  {
    EXPECT_CALL(client, printText("test\n"));
    game << "test" << std::endl;
  }

  TEST_F(Game_ostream, ostream_operator_ends_flushes)
  {
    EXPECT_CALL(client, printText("who"));
    game << "who" << std::ends;
  }

  TEST_F(Game_ostream, ostream_operator_clearsBufferAfterFlush)
  {
    EXPECT_CALL(client, printText("test\n"));
    game << "test" << std::endl;

    EXPECT_CALL(client, printText("what"));
    game << "what" << std::ends;
  }

  TEST_F(Game_ostream, ostream_operator_doesntFlushIfNotExplicit)
  {
    EXPECT_CALL(client, printText(_)).Times(0);
    game << "ice cream" << '\n';
  }
}
namespace GameTest::Draw {
  TEST_F(Game_DrawShape, drawLine_callsClient)
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

  TEST_F(Game_DrawShape, drawBox_callsClient)
  {
    EXPECT_CALL(client, drawShape(ShapeType::Box, CoordinateType::Mouse, 2, 5, 15, 18, 0, 0, Colors::Cyan, true));
    game.drawBox(CoordinateType::Mouse, 2, 5, 15, 18, Colors::Cyan, true);

    EXPECT_CALL(client, drawShape(ShapeType::Box, CoordinateType::Map, 1, 3, 1, 25, 0, 0, Colors::Red, false)).Times(2);
    game.drawBoxMap(1, 3, 1, 25, Colors::Red);
    game.drawBoxMap(Position(1, 3), Position(1, 25), Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Box, CoordinateType::Mouse, 1, 3, 1, 25, 0, 0, Colors::Red, false)).Times(2);
    game.drawBoxMouse(1, 3, 1, 25, Colors::Red);
    game.drawBoxMouse(Position(1, 3), Position(1, 25), Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Box, CoordinateType::Screen, 1, 3, 1, 25, 0, 0, Colors::Red, false)).Times(2);
    game.drawBoxScreen(1, 3, 1, 25, Colors::Red);
    game.drawBoxScreen(Position(1, 3), Position(1, 25), Colors::Red);
  }

  TEST_F(Game_DrawShape, drawCircle_callsClient)
  {
    EXPECT_CALL(client, drawShape(ShapeType::Circle, CoordinateType::Mouse, 18, 44, 0, 0, 10, 0, Colors::Cyan, true));
    game.drawCircle(CoordinateType::Mouse, 18, 44, 10, Colors::Cyan, true);

    EXPECT_CALL(client, drawShape(ShapeType::Circle, CoordinateType::Map, 25, 20, 0, 0, 5, 0, Colors::Red, false)).Times(2);
    game.drawCircleMap(25, 20, 5, Colors::Red);
    game.drawCircleMap(Position(25, 20), 5, Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Circle, CoordinateType::Mouse, 25, 20, 0, 0, 5, 0, Colors::Red, false)).Times(2);
    game.drawCircleMouse(25, 20, 5, Colors::Red);
    game.drawCircleMouse(Position(25, 20), 5, Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Circle, CoordinateType::Screen, 25, 20, 0, 0, 5, 0, Colors::Red, false)).Times(2);
    game.drawCircleScreen(25, 20, 5, Colors::Red);
    game.drawCircleScreen(Position(25, 20), 5, Colors::Red);
  }

  TEST_F(Game_DrawShape, drawDot_callsClient)
  {
    EXPECT_CALL(client, drawShape(ShapeType::Dot, CoordinateType::Mouse, 7, 9, 0, 0, 0, 0, Colors::Cyan, false));
    game.drawDot(CoordinateType::Mouse, 7, 9, Colors::Cyan);

    EXPECT_CALL(client, drawShape(ShapeType::Dot, CoordinateType::Map, 3, 9, 0, 0, 0, 0, Colors::Red, false)).Times(2);
    game.drawDotMap(3, 9, Colors::Red);
    game.drawDotMap(Position(3, 9), Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Dot, CoordinateType::Mouse, 3, 9, 0, 0, 0, 0, Colors::Red, false)).Times(2);
    game.drawDotMouse(3, 9, Colors::Red);
    game.drawDotMouse(Position(3, 9), Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Dot, CoordinateType::Screen, 3, 9, 0, 0, 0, 0, Colors::Red, false)).Times(2);
    game.drawDotScreen(3, 9, Colors::Red);
    game.drawDotScreen(Position(3, 9), Colors::Red);
  }

  TEST_F(Game_DrawShape, drawEllipse_callsClient)
  {
    EXPECT_CALL(client, drawShape(ShapeType::Ellipse, CoordinateType::Mouse, 18, 44, 0, 0, 10, 12, Colors::Cyan, true));
    game.drawEllipse(CoordinateType::Mouse, 18, 44, 10, 12, Colors::Cyan, true);

    EXPECT_CALL(client, drawShape(ShapeType::Ellipse, CoordinateType::Map, 25, 20, 0, 0, 5, 7, Colors::Red, false)).Times(2);
    game.drawEllipseMap(25, 20, 5, 7, Colors::Red);
    game.drawEllipseMap(Position(25, 20), 5, 7, Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Ellipse, CoordinateType::Mouse, 25, 20, 0, 0, 5, 7, Colors::Red, false)).Times(2);
    game.drawEllipseMouse(25, 20, 5, 7, Colors::Red);
    game.drawEllipseMouse(Position(25, 20), 5, 7, Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Ellipse, CoordinateType::Screen, 25, 20, 0, 0, 5, 7, Colors::Red, false)).Times(2);
    game.drawEllipseScreen(25, 20, 5, 7, Colors::Red);
    game.drawEllipseScreen(Position(25, 20), 5, 7, Colors::Red);
  }

  TEST_F(Game_DrawShape, drawTriangle_callsClient)
  {
    EXPECT_CALL(client, drawShape(ShapeType::Triangle, CoordinateType::Mouse, 2, 5, 15, 18, 9, 7, Colors::Cyan, true));
    game.drawTriangle(CoordinateType::Mouse, 2, 5, 15, 18, 9, 7, Colors::Cyan, true);

    EXPECT_CALL(client, drawShape(ShapeType::Triangle, CoordinateType::Map, 1, 3, 1, 25, 10, 8, Colors::Red, false)).Times(2);
    game.drawTriangleMap(1, 3, 1, 25, 10, 8, Colors::Red);
    game.drawTriangleMap(Position(1, 3), Position(1, 25), Position(10, 8), Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Triangle, CoordinateType::Mouse, 1, 3, 1, 25, 10, 8, Colors::Red, false)).Times(2);
    game.drawTriangleMouse(1, 3, 1, 25, 10, 8, Colors::Red);
    game.drawTriangleMouse(Position(1, 3), Position(1, 25), Position(10, 8), Colors::Red);

    EXPECT_CALL(client, drawShape(ShapeType::Triangle, CoordinateType::Screen, 1, 3, 1, 25, 10, 8, Colors::Red, false)).Times(2);
    game.drawTriangleScreen(1, 3, 1, 25, 10, 8, Colors::Red);
    game.drawTriangleScreen(Position(1, 3), Position(1, 25), Position(10, 8), Colors::Red);
  }
}
namespace GameTest::DrawText {
  TEST_F(Game_DrawText, drawText_callsClient)
  {
    std::string expected{ "\x15 chocolate\ncake" };

    EXPECT_CALL(client, drawText(CoordinateType::Mouse, expected, 77, 33, Text::Size::Default));
    game.drawText(CoordinateType::Mouse, 77, 33, "%s", expected.c_str());

    EXPECT_CALL(client, drawText(CoordinateType::Map, expected, 26, 73, Text::Size::Default));
    game.drawTextMap(26, 73, "%s", expected.c_str());

    EXPECT_CALL(client, drawText(CoordinateType::Mouse, expected, 26, 73, Text::Size::Default));
    game.drawTextMouse(26, 73, "%s", expected.c_str());

    EXPECT_CALL(client, drawText(CoordinateType::Screen, expected, 26, 73, Text::Size::Default));
    game.drawTextScreen(26, 73, "%s", expected.c_str());
  }
}
namespace GameTest::SetTextSize {
  TEST_F(Game_SetTextSize, sendsTextSizeToClient)
  {
    // Default
    EXPECT_CALL(client, drawText(CoordinateType::Map, "", 0, 0, Text::Size::Default));
    game.drawTextMap(0, 0, "");

    // Small
    EXPECT_CALL(client, drawText(CoordinateType::Map, "", 0, 0, Text::Size::Small));
    game.setTextSize(Text::Size::Small);
    game.drawTextMap(0, 0, "");

    // Large
    EXPECT_CALL(client, drawText(CoordinateType::Map, "", 0, 0, Text::Size::Large));
    game.setTextSize(Text::Size::Large);
    game.drawTextMap(0, 0, "");

    // Huge
    EXPECT_CALL(client, drawText(CoordinateType::Map, "", 0, 0, Text::Size::Huge));
    game.setTextSize(Text::Size::Huge);
    game.drawTextMap(0, 0, "");
  }
}
namespace GameTest::SetText {
  TEST_F(Game_SendText, sendText_callsClient)
  {
    std::string expected{ "\nhello\tit's me again" };

    EXPECT_CALL(client, sendText(expected, false));
    game.sendText("%s", expected.c_str());
  }

  TEST_F(Game_SendText, sendTextEx_callsClient)
  {
    std::string expected{ "Friends! Yay" };

    EXPECT_CALL(client, sendText(expected, false));
    game.sendTextEx(false, "%s", expected.c_str());

    EXPECT_CALL(client, sendText(expected, true));
    game.sendTextEx(true, "%s", expected.c_str());
  }
}
namespace GameTest::SetLocalSpeed {
  TEST_F(Game_SetLocalSpeed, callsClient)
  {
    EXPECT_CALL(client, setLocalSpeed(42));
    game.setLocalSpeed(42);
  }
}
namespace GameTest::SetAlliance {
  TEST_F(Game_SetAlliance, hasNoError_ifSuccess)
  {
    EXPECT_CALL(client, setAlliance(1, 2));
    game.setAlliance(game.enemy(), true);
    EXPECT_EQ(game.getLastError(), Errors::None);
  }

  TEST_F(Game_SetAlliance, fails_ifNoSelf)
  {
    withNoSelf();
    EXPECT_CALL(client, setAlliance(_, _)).Times(0);
    game.setAlliance(game.enemy(), false);
    EXPECT_EQ(game.getLastError(), Errors::Invalid_Parameter);
  }

  TEST_F(Game_SetAlliance, fails_ifReplay)
  {
    withReplay();
    EXPECT_CALL(client, setAlliance(_, _)).Times(0);
    game.setAlliance(game.enemy(), false);
    EXPECT_EQ(game.getLastError(), Errors::Invalid_Parameter);
  }

  TEST_F(Game_SetAlliance, fails_ifNoPlayer)
  {
    EXPECT_CALL(client, setAlliance(_, _)).Times(0);
    game.setAlliance(nullptr, false);
    EXPECT_EQ(game.getLastError(), Errors::Invalid_Parameter);
  }

  TEST_F(Game_SetAlliance, fails_ifSelf)
  {
    EXPECT_CALL(client, setAlliance(_, _)).Times(0);
    game.setAlliance(game.self(), false);
    EXPECT_EQ(game.getLastError(), Errors::Invalid_Parameter);
  }

  TEST_F(Game_SetAlliance, callsClient)
  {
    Player player = createFakePlayer(3, PlayerTypes::RescuePassive, Races::Protoss);

    EXPECT_CALL(client, setAlliance(3, 0));
    game.setAlliance(player, false);

    EXPECT_CALL(client, setAlliance(3, 1));
    game.setAlliance(player, true, false);

    EXPECT_CALL(client, setAlliance(3, 2));
    game.setAlliance(player, true, true);
  }
}
namespace GameTest::SetVision {
  TEST_F(Game_SetVision, callsClient)
  {
    Player player = createFakePlayer(3, PlayerTypes::RescuePassive, Races::Protoss);

    EXPECT_CALL(client, setVision(3, 0));
    game.setVision(player, false);

    EXPECT_CALL(client, setVision(3, 1));
    game.setVision(player, true);
  }
}
