#pragma once
#include "Triggers.h"

#include "../Types.h"

#pragma pack(1)
namespace BW
{
  namespace Triggers
  {
    class Condition
    {
    public:
      DWORD dwLocation;
      DWORD dwGroup;
      DWORD dwCount;
      WORD  wUnitType;
      BYTE  bComparisonType;
      BYTE  bConditionType;
      BYTE  bExtraType;
      BYTE  bFlags;
      BYTE  __bAlign[2];

      bool canBeTrue() const;
    };

    class Action
    {
    public:
      DWORD dwSourceLocation;
      DWORD dwTextString;
      DWORD dwWavString;
      DWORD dwTime;
      DWORD dwSourceGroup;
      DWORD dwExtra;
      WORD  wType;
      BYTE  bActionType;
      BYTE  bModifier;
      BYTE  bFlags;
      BYTE  __bAlign[3];

      bool affectsGameplay(DWORD dwExecutionPlayer, DWORD dwCheckingPlayer) const;
    };

    class Trigger
    {
    public:
      Condition conditions[16];
      Action    actions[64];
      DWORD     dwExecutionFlags;
      BYTE      bExecuteFor[PlayerGroups::Max];
      BYTE      bCurrentActionIndex;

      bool conditionsCanBeMet() const;
      bool actionsAllowGameplay(DWORD dwExecutionPlayer, DWORD dwCheckingPlayer) const;
    };

  };
};
#pragma pack()
