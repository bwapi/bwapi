#include <windows.h>

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
      WORD  __wAlign;

      bool canBeTrue();
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
      WORD  __wAlign;
      BYTE  __bAlign;

      bool affectsGameplay(DWORD dwExecutionPlayer, DWORD dwCheckingPlayer);
    };

    class Trigger
    {
    public:
      Condition conditions[16];
      Action    actions[64];
      DWORD     dwExecutionFlags;
      BYTE      bExecuteFor[27];
      BYTE      bCurrentActionIndex;

      bool conditionsCanBeMet();
      bool actionsAllowGameplay(DWORD dwExecutionPlayer, DWORD dwCheckingPlayer);
    };

  };
};
