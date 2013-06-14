#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

struct SelectUnitData
{
  UnitID unitID;
  BYTE data;
};

#define EXTRACT_SELECT_DATA_UNITID(Data)  (Data & 0x7FF)
#define EXTRACT_SELECT_DATA_DATA(Data)    (Data >> 12)
#define FORM_SELECT_DATA(ID, Data)        ((WORD)((WORD)ID | ((WORD)Data << 12)))

class SelectAction : public GameAction
{
public:
  enum SelectType
  {
    SELECT_DeltaAdd,
    SELECT_DeltaDel,
    SELECT_Units
  };

  SelectAction(PlayerID player, SelectType type);
  SelectAction(PlayerID player, SelectType type, const SelectUnitData selectedUnits[], unsigned unitsCount);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const SelectUnitData* getSelectedUnits() const { return selectedUnits; }
  const unsigned getUnitsCount() const { return unitsCount; }

  IMPLEMENT_FROM_WITH_1_PARAM(SelectAction, type);

  static const unsigned MAX_UNITS = 12;

private:
  SelectType type;
  SelectUnitData selectedUnits[MAX_UNITS];
  unsigned unitsCount;
};

END_REPLAY_TOOL