#include <gtest/gtest.h>

#include <BWAPI/Position.h>

#include <unordered_set>

using namespace BWAPI;

TEST(Positions, Origin) {
  EXPECT_EQ(Positions::Origin.x, 0);
  EXPECT_EQ(Positions::Origin.y, 0);
  EXPECT_EQ(TilePositions::Origin.x, 0);
  EXPECT_EQ(TilePositions::Origin.y, 0);
  EXPECT_EQ(WalkPositions::Origin.x, 0);
  EXPECT_EQ(WalkPositions::Origin.y, 0);
}

TEST(Position, Equals) {
  Position posA(-33, 8);
  Position posB(-33, 8);
  Position posC(-32, 8);
  Position posD(-33, 9);

  EXPECT_TRUE(posA == posB);
  EXPECT_FALSE(posA == posC);
  EXPECT_FALSE(posB == posD);

  EXPECT_EQ(posA, posB);
}

TEST(Position, NotEquals) {
  Position posA(-33, 8);
  Position posB(-33, 8);
  Position posC(-32, 8);
  Position posD(-33, 9);

  EXPECT_FALSE(posA != posB);
  EXPECT_TRUE(posA != posC);
  EXPECT_TRUE(posB != posD);
  
  EXPECT_NE(posA, posC);
  EXPECT_NE(posB, posD);
}

// Specifically written to ensure x and y get assigned appropriate values
TEST(Position, Ctor) {
  {
    Position pos(32, 32);
    EXPECT_EQ(pos.x, 32);
    EXPECT_EQ(pos.y, 32);
  }
  {
    WalkPosition pos(16, 9);
    EXPECT_EQ(pos.x, 16);
    EXPECT_EQ(pos.y, 9);
  }
  {
    TilePosition pos(-1, -33);
    EXPECT_EQ(pos.x, -1);
    EXPECT_EQ(pos.y, -33);
  }
}

TEST(Position, DefaultCtor) {
  Position pos;
  EXPECT_EQ(pos, Positions::Origin);
}

TEST(Position, ConversionCtor) {
  // From position
  Position pos(32, 32);
  TilePosition tpos(pos);
  WalkPosition wpos(pos);
  EXPECT_EQ(32, pos.x);
  EXPECT_EQ(32, pos.y);
  EXPECT_EQ(1, tpos.x);
  EXPECT_EQ(1, tpos.y);
  EXPECT_EQ(4, wpos.x);
  EXPECT_EQ(4, wpos.y);

  // From tile position
  Position pos2(tpos);
  WalkPosition wpos2(tpos);
  EXPECT_EQ(32, pos2.x);
  EXPECT_EQ(32, pos2.y);
  EXPECT_EQ(4, wpos2.x);
  EXPECT_EQ(4, wpos2.y);

  // From walk position
  Position pos3(wpos);
  TilePosition tpos3(wpos);
  EXPECT_EQ(32, pos3.x);
  EXPECT_EQ(32, pos3.y);
  EXPECT_EQ(1, tpos3.x);
  EXPECT_EQ(1, tpos3.y);
}

TEST(Position, LessThan)
{
  Position p1(0, 0), p2(1, 0), p3(0, 1), p4(1, 1);

  EXPECT_FALSE(p1 < p1);
  EXPECT_TRUE(p1 < p2);
  EXPECT_TRUE(p1 < p3);
  EXPECT_TRUE(p1 < p4);

  EXPECT_FALSE(p2 < p1);
  EXPECT_FALSE(p2 < p2);
  EXPECT_FALSE(p2 < p3);
  EXPECT_TRUE(p2 < p4);

  EXPECT_FALSE(p3 < p1);
  EXPECT_TRUE(p3 < p2);
  EXPECT_FALSE(p3 < p3);
  EXPECT_TRUE(p3 < p4);

  EXPECT_FALSE(p4 < p1);
  EXPECT_FALSE(p4 < p2);
  EXPECT_FALSE(p4 < p3);
  EXPECT_FALSE(p4 < p4);
}

TEST(Position, Add)
{
  Position p1(1, 1), p2(1, 2);

  Position p3 = p1 + p2;
  EXPECT_EQ(Position(2, 3), p3);

  p3 += p1;
  EXPECT_EQ(Position(3, 4), p3);

  p3 += Positions::Origin;
  EXPECT_EQ(Position(3, 4), p3);
}

TEST(Position, Subtract)
{
  Position p1(1, 1), p2(1, 2);

  Position p3 = p1 - p2;
  EXPECT_EQ(Position(0, -1), p3);

  p3 -= p1;
  EXPECT_EQ(Position(-1, -2), p3);

  p3 -= Positions::Origin;
  EXPECT_EQ(Position(-1, -2), p3);
}

TEST(Position, Multiply)
{
  Position p1(1, 2);

  Position p2 = p1 * 1;
  Position p3 = p1 * 2;
  EXPECT_EQ(Position(1, 2), p2);
  EXPECT_EQ(Position(2, 4), p3);

  p2 *= 2;
  p3 *= 1;
  EXPECT_EQ(Position(2, 4), p2);
  EXPECT_EQ(Position(2, 4), p3);
}

TEST(Position, Or)
{
  Position p1(1, 2);

  Position p2 = p1 | 1;
  Position p3 = p1 | 2;
  EXPECT_EQ(Position(1, 3), p2);
  EXPECT_EQ(Position(3, 2), p3);

  p2 |= 2;
  p3 |= 1;
  EXPECT_EQ(Position(3, 3), p2);
  EXPECT_EQ(Position(3, 3), p3);
}

TEST(Position, And)
{
  Position p1(1, 2);

  Position p2 = p1 & 1;
  Position p3 = p1 & 2;
  EXPECT_EQ(Position(1, 0), p2);
  EXPECT_EQ(Position(0, 2), p3);

  p2 &= 2;
  p3 &= 1;
  EXPECT_EQ(Positions::Origin, p2);
  EXPECT_EQ(Positions::Origin, p3);
}

TEST(Position, Xor)
{
  Position p1(1, 2);

  Position p2 = p1 ^ 1;
  Position p3 = p1 ^ 2;
  EXPECT_EQ(Position(0, 3), p2);
  EXPECT_EQ(Position(3, 0), p3);

  p2 ^= 2;
  p3 ^= 1;
  EXPECT_EQ(Position(2, 1), p2);
  EXPECT_EQ(Position(2, 1), p3);
}

TEST(Position, Divide)
{
  Position p1(1, 2);

  Position p2 = p1 / 1;
  Position p3 = p1 / 2;
  EXPECT_EQ(Position(1, 2), p2);
  EXPECT_EQ(Position(0, 1), p3);

  p2 /= 2;
  p3 /= 1;
  EXPECT_EQ(Position(0, 1), p2);
  EXPECT_EQ(Position(0, 1), p3);

  p2 = p1 / 0;
  p3 /= 0;
  EXPECT_EQ(Positions::Invalid, p2);
  EXPECT_EQ(Positions::Invalid, p3);
}

TEST(Position, Modulus)
{
  Position p1(1, 2);

  Position p2 = p1 % 1;
  Position p3 = p1 % 2;
  EXPECT_EQ(Position(0, 0), p2);
  EXPECT_EQ(Position(1, 0), p3);

  p2 = p3 = Position(3, 4);
  p2 %= 2;
  p3 %= 1;
  EXPECT_EQ(Position(1, 0), p2);
  EXPECT_EQ(Position(0, 0), p3);

  p2 = p1 % 0;
  p3 %= 0;
  EXPECT_EQ(Positions::Invalid, p2);
  EXPECT_EQ(Positions::Invalid, p3);
}

TEST(Position, SetMin)
{
  Position p1(0, 0), p2(3, 3);

  p1.setMin(1, 0);
  p2.setMin(Position(0, 4));
  EXPECT_EQ(Position(1, 0), p1);
  EXPECT_EQ(Position(3, 4), p2);
}

TEST(Position, SetMax)
{
  Position p1(0, 0), p2(3, 3);

  p1.setMax(1, -1);
  p2.setMax(Position(2, 2));
  EXPECT_EQ(Position(0, -1), p1);
  EXPECT_EQ(Position(2, 2), p2);
}

TEST(Position, Ostream)
{
  Position p1(2, -3);
  std::stringstream ss;
  ss << p1;
  EXPECT_STREQ("(2,-3)", ss.str().c_str());
}

TEST(Position, Istream)
{
  Position p1;
  std::stringstream ss("2 -3");
  ss >> p1;
  EXPECT_EQ(Position(2, -3), p1);
}

TEST(Position, WOstream)
{
  Position p1(2, -3);
  std::wstringstream ss;
  ss << p1;
  EXPECT_STREQ(L"(2,-3)", ss.str().c_str());
}

TEST(Position, WIstream)
{
  Position p1;
  std::wstringstream ss(L"2 -3");
  ss >> p1;
  EXPECT_EQ(Position(2, -3), p1);
}

TEST(Position, IsHashable)
{
  Position p = { 5, 100 };
  std::unordered_set<Position> positions{ p };

  EXPECT_EQ(*positions.begin(), p);
}
