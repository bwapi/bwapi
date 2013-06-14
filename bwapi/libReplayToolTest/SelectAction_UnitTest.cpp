#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "SelectAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER  (1)

const SelectUnitData selected1Unit[] = { {1, 0} };
const SelectUnitData selected4Units[] = { {1, 0}, {2, 0}, {4, 0}, {8, 0} };

class SelectActionTest : public testing::Test
{
protected:
  void testReadNUnits(const SelectUnitData selectedUnits[], BYTE count)
  {
    MockReplayReader reader;
    SelectAction action(DEFAULT_PLAYER, SelectAction::SELECT_Units);

    {
      InSequence seq;

      EXPECT_CALL(reader, readBYTE()).
        WillOnce(testing::Return(count));

      for (unsigned i = 0; i < count; ++i)
      {
        const SelectUnitData& selectData = selectedUnits[i];

        EXPECT_CALL(reader, readWORD()).
          WillOnce(testing::Return(FORM_SELECT_DATA(selectData.unitID, selectData.data))).
          RetiresOnSaturation();
      }
    }

    action.read(reader);

    EXPECT_EQ(count, action.getUnitsCount());

    for (unsigned i = 0; i < count; ++i)
    {
      const SelectUnitData& expectedSelectData = selectedUnits[i];
      const SelectUnitData& actualSelectData = action.getSelectedUnits()[i];

      EXPECT_EQ(expectedSelectData.unitID, actualSelectData.unitID);
      EXPECT_EQ(expectedSelectData.data, actualSelectData.data);
    }
  }
};

TEST_F(SelectActionTest, Ctor)
{
  SelectAction action(DEFAULT_PLAYER, SelectAction::SELECT_DeltaAdd, selected1Unit, _countof(selected1Unit));

  EXPECT_EQ(DEFAULT_PLAYER, action.getPlayer());
  EXPECT_EQ(ReplayTool::Select_Delta_Add, action.getAction());

  EXPECT_EQ(_countof(selected1Unit), action.getUnitsCount());

  for (int i = 0; i < _countof(selected1Unit); ++i)
  {
    const SelectUnitData& expectedSelectData = selected1Unit[i];
    const SelectUnitData& actualSelectData = action.getSelectedUnits()[i];

    EXPECT_EQ(expectedSelectData.unitID, actualSelectData.unitID);
    EXPECT_EQ(expectedSelectData.data, actualSelectData.data);
  }
}

TEST_F(SelectActionTest, Read1Unit)
{
  testReadNUnits(selected1Unit, _countof(selected1Unit));
}

TEST_F(SelectActionTest, Read4Units)
{
  testReadNUnits(selected4Units, _countof(selected4Units));
}

TEST_F(SelectActionTest, toString1Unit)
{
  SelectAction action(DEFAULT_PLAYER, SelectAction::SELECT_Units, selected1Unit, 1);

  string expected = "(P1) Select: 1, 1:00";
  string actual = action.toString();

  EXPECT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(SelectActionTest, toString4Units)
{
  SelectAction action(DEFAULT_PLAYER, SelectAction::SELECT_Units, selected4Units, 4);

  string expected = "(P1) Select: 4, 1:00, 2:00, 4:00, 8:00";
  string actual = action.toString();

  EXPECT_STREQ(expected.c_str(), actual.c_str());
}