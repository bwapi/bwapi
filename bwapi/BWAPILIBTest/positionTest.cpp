#include "stdafx.h"
#include "CppUnitTest.h"
#include "specializations.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BWAPI;

namespace BWAPILIBTest
{
  // TODO test these
  // - getDistance
  // - getLength
  // - getApproxDistance
	TEST_CLASS(positionTest)
	{
	public:
    TEST_METHOD(PositionCtorDefault)
    {
      Position pos;
      TilePosition tpos;
      WalkPosition wpos;

      Assert::AreEqual(0, pos.x);
      Assert::AreEqual(0, pos.y);
      Assert::AreEqual(0, tpos.x);
      Assert::AreEqual(0, tpos.y);
      Assert::AreEqual(0, wpos.x);
      Assert::AreEqual(0, wpos.y);
    }
    TEST_METHOD(PositionCtorValue)
    {
      Position pos(32, 32);
      TilePosition tpos(32, 32);
      WalkPosition wpos(32, 32);

      Assert::AreEqual(32, pos.x);
      Assert::AreEqual(32, pos.y);
      Assert::AreEqual(32, tpos.x);
      Assert::AreEqual(32, tpos.y);
      Assert::AreEqual(32, wpos.x);
      Assert::AreEqual(32, wpos.y);
    }
    TEST_METHOD(PositionCtorConversion)
    {
      // From position
      Position pos(32, 32);
      TilePosition tpos(pos);
      WalkPosition wpos(pos);
      Assert::AreEqual(32, pos.x);
      Assert::AreEqual(32, pos.y);
      Assert::AreEqual(1, tpos.x);
      Assert::AreEqual(1, tpos.y);
      Assert::AreEqual(4, wpos.x);
      Assert::AreEqual(4, wpos.y);

      // From tile position
      Position pos2(tpos);
      WalkPosition wpos2(tpos);
      Assert::AreEqual(32, pos2.x);
      Assert::AreEqual(32, pos2.y);
      Assert::AreEqual(4, wpos2.x);
      Assert::AreEqual(4, wpos2.y);

      // From walk position
      Position pos3(wpos);
      TilePosition tpos3(wpos);
      Assert::AreEqual(32, pos3.x);
      Assert::AreEqual(32, pos3.y);
      Assert::AreEqual(1, tpos3.x);
      Assert::AreEqual(1, tpos3.y);
    }
    TEST_METHOD(PositionEquality)
    {
      TilePosition p1(0, 0), p2(2, 2);

      Assert::IsFalse(p1 == p2);
      Assert::IsTrue(p1 != p2);

      Assert::IsFalse(p1 != TilePositions::Origin);
      Assert::IsTrue(p1 == TilePositions::Origin);

    }
    TEST_METHOD(PositionValidity)
    {
      TilePosition p1(0, 0), p2(-1, 0), p3(0, -1), p4(-1, -1), p5(256, 0), p6(0, 256), p7(256, 256);

      Assert::IsTrue(p1.isValid());
      Assert::IsFalse(p2.isValid());
      Assert::IsFalse(p3.isValid());
      Assert::IsFalse(p4.isValid());
      Assert::IsFalse(p5.isValid());
      Assert::IsFalse(p6.isValid());
      Assert::IsFalse(p7.isValid());

      p1.makeValid();
      p2.makeValid();
      p3.makeValid();
      p4.makeValid();
      p5.makeValid();
      p6.makeValid();
      p7.makeValid();
      Assert::IsTrue(p1.isValid());
      Assert::IsTrue(p2.isValid());
      Assert::IsTrue(p3.isValid());
      Assert::IsTrue(p4.isValid());
      Assert::IsTrue(p5.isValid());
      Assert::IsTrue(p6.isValid());
      Assert::IsTrue(p7.isValid());
      
      Assert::AreEqual(TilePositions::Origin, p1);
      Assert::AreEqual(TilePositions::Origin, p2);
      Assert::AreEqual(TilePositions::Origin, p3);
      Assert::AreEqual(TilePositions::Origin, p4);
      Assert::AreEqual(TilePosition(255, 0), p5);
      Assert::AreEqual(TilePosition(0, 255), p6);
      Assert::AreEqual(TilePosition(255, 255), p7);
    }
    TEST_METHOD(PositionLessThan)
    {
      Position p1(0, 0), p2(1, 0), p3(0, 1), p4(1, 1);
      
      Assert::IsFalse(p1 < p1);
      Assert::IsTrue(p1 < p2);
      Assert::IsTrue(p1 < p3);
      Assert::IsTrue(p1 < p4);

      Assert::IsFalse(p2 < p1);
      Assert::IsFalse(p2 < p2);
      Assert::IsFalse(p2 < p3);
      Assert::IsTrue(p2 < p4);

      Assert::IsFalse(p3 < p1);
      Assert::IsTrue(p3 < p2);
      Assert::IsFalse(p3 < p3);
      Assert::IsTrue(p3 < p4);

      Assert::IsFalse(p4 < p1);
      Assert::IsFalse(p4 < p2);
      Assert::IsFalse(p4 < p3);
      Assert::IsFalse(p4 < p4);

    }
    TEST_METHOD(PositionAdd)
    {
      Position p1(1, 1), p2(1, 2);

      Position p3 = p1 + p2;
      Assert::AreEqual(Position(2, 3), p3);

      p3 += p1;
      Assert::AreEqual(Position(3, 4), p3);

      p3 += Positions::Origin;
      Assert::AreEqual(Position(3, 4), p3);
    }
    TEST_METHOD(PositionSubtract)
    {
      Position p1(1, 1), p2(1, 2);

      Position p3 = p1 - p2;
      Assert::AreEqual(Position(0, -1), p3);

      p3 -= p1;
      Assert::AreEqual(Position(-1, -2), p3);

      p3 -= Positions::Origin;
      Assert::AreEqual(Position(-1, -2), p3);
    }
    TEST_METHOD(PositionMultiply)
    {
      Position p1(1, 2);

      Position p2 = p1 * 1;
      Position p3 = p1 * 2;
      Assert::AreEqual(Position(1, 2), p2);
      Assert::AreEqual(Position(2, 4), p3);

      p2 *= 2;
      p3 *= 1;
      Assert::AreEqual(Position(2, 4), p2);
      Assert::AreEqual(Position(2, 4), p3);
    }
    TEST_METHOD(PositionOr)
    {
      Position p1(1, 2);

      Position p2 = p1 | 1;
      Position p3 = p1 | 2;
      Assert::AreEqual(Position(1, 3), p2);
      Assert::AreEqual(Position(3, 2), p3);

      p2 |= 2;
      p3 |= 1;
      Assert::AreEqual(Position(3, 3), p2);
      Assert::AreEqual(Position(3, 3), p3);
    }
    TEST_METHOD(PositionAnd)
    {
      Position p1(1, 2);

      Position p2 = p1 & 1;
      Position p3 = p1 & 2;
      Assert::AreEqual(Position(1, 0), p2);
      Assert::AreEqual(Position(0, 2), p3);

      p2 &= 2;
      p3 &= 1;
      Assert::AreEqual(Positions::Origin, p2);
      Assert::AreEqual(Positions::Origin, p3);
    }
    TEST_METHOD(PositionXor)
    {
      Position p1(1, 2);

      Position p2 = p1 ^ 1;
      Position p3 = p1 ^ 2;
      Assert::AreEqual(Position(0, 3), p2);
      Assert::AreEqual(Position(3, 0), p3);

      p2 ^= 2;
      p3 ^= 1;
      Assert::AreEqual(Position(2, 1), p2);
      Assert::AreEqual(Position(2, 1), p3);
    }
    TEST_METHOD(PositionDivide)
    {
      Position p1(1, 2);

      Position p2 = p1 / 1;
      Position p3 = p1 / 2;
      Assert::AreEqual(Position(1, 2), p2);
      Assert::AreEqual(Position(0, 1), p3);

      p2 /= 2;
      p3 /= 1;
      Assert::AreEqual(Position(0, 1), p2);
      Assert::AreEqual(Position(0, 1), p3);

      // cppcheck-suppress zerodiv
      p2 = p1 / 0;
      // cppcheck-suppress zerodiv
      p3 /= 0;
      Assert::AreEqual(Positions::Invalid, p2);
      Assert::AreEqual(Positions::Invalid, p3);
    }
    TEST_METHOD(PositionModulus)
    {
      Position p1(1, 2);

      Position p2 = p1 % 1;
      Position p3 = p1 % 2;
      Assert::AreEqual(Position(0, 0), p2);
      Assert::AreEqual(Position(1, 0), p3);

      p2 = p3 = Position(3, 4);
      p2 %= 2;
      p3 %= 1;
      Assert::AreEqual(Position(1, 0), p2);
      Assert::AreEqual(Position(0, 0), p3);

      // cppcheck-suppress zerodiv
      p2 = p1 % 0;
      // cppcheck-suppress zerodiv
      p3 %= 0;
      Assert::AreEqual(Positions::Invalid, p2);
      Assert::AreEqual(Positions::Invalid, p3);
    }
    TEST_METHOD(PositionSetMin)
    {
      Position p1(0, 0), p2(3, 3);

      p1.setMin(1, 0);
      p2.setMin(Position(0, 4));
      Assert::AreEqual(Position(1, 0), p1);
      Assert::AreEqual(Position(3, 4), p2);
    }
    TEST_METHOD(PositionSetMax)
    {
      Position p1(0, 0), p2(3, 3);

      p1.setMax(1, -1);
      p2.setMax(Position(2, 2));
      Assert::AreEqual(Position(0, -1), p1);
      Assert::AreEqual(Position(2, 2), p2);
    }
    TEST_METHOD(PositionOstream)
    {
      Position p1(2, -3);
      std::stringstream ss;
      ss << p1;
      Assert::AreEqual("(2,-3)", ss.str().c_str());
    }
    TEST_METHOD(PositionIstream)
    {
      Position p1;
      std::stringstream ss("2 -3");
      ss >> p1;
      Assert::AreEqual(Position(2, -3), p1);
    }
    TEST_METHOD(PositionWOstream)
    {
      Position p1(2, -3);
      std::wstringstream ss;
      ss << p1;
      Assert::AreEqual(L"(2,-3)", ss.str().c_str());
    }
    TEST_METHOD(PositionWIstream)
    {
      Position p1;
      std::wstringstream ss(L"2 -3");
      ss >> p1;
      Assert::AreEqual(Position(2, -3), p1);
    }

	};
}