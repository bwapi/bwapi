#include "TriggerEngine.h"
#include "Triggers.h"

#include "Offsets.h"

namespace BW
{
  namespace Triggers
  {
    bool Trigger::conditionsCanBeMet() const
    {
      // iterate maximum number of conditions
      for ( int i = 0; i < 16; ++i )
      {
        // return false if the condition can never be met
        if ( !this->conditions[i].canBeTrue() )
          return false;

        // "None" also indicates the end of the list
        if ( this->conditions[i].bConditionType == Conditions::None )
          break;
      }
      return true;
    }
    bool Trigger::actionsAllowGameplay(DWORD dwExecutionPlayer, DWORD dwCheckingPlayer) const
    {
      // iterate maximum number of actions
      for ( int i = 0; i < 64; ++i )
      {
        // return true if this action is capable of creating or giving the player units
        if ( this->actions[i].affectsGameplay(dwExecutionPlayer, dwCheckingPlayer) )
          return true;

        // "None" also indicates the end of the list
        if ( this->actions[i].bActionType == Actions::None )
          break;
      }
      return false;
    }

    bool playerCanMatch(DWORD dwActionGroup, DWORD dwExecutionPlayer, DWORD dwCheckingPlayer)
    {
      // Return true if it's possible for the affected group to give units to the player we're checking
      switch ( dwActionGroup )
      {
      case PlayerGroups::Player1:
      case PlayerGroups::Player2:
      case PlayerGroups::Player3:
      case PlayerGroups::Player4:
      case PlayerGroups::Player5:
      case PlayerGroups::Player6:
      case PlayerGroups::Player7:
      case PlayerGroups::Player8:
        return dwActionGroup == dwCheckingPlayer;
      case PlayerGroups::AllPlayers:
      case PlayerGroups::NonAlliedVictoryPlayers:
        return true;
      case PlayerGroups::Allies:
      case PlayerGroups::Foes:
        return dwExecutionPlayer != dwCheckingPlayer;
      case PlayerGroups::CurrentPlayer:
        return dwExecutionPlayer == dwCheckingPlayer;
      case PlayerGroups::Force1:
      case PlayerGroups::Force2:
      case PlayerGroups::Force3:
      case PlayerGroups::Force4:
        return BW::BWDATA::Players[dwCheckingPlayer].nTeam == (dwActionGroup - PlayerGroups::Force1 + 1);
      default:
        return false;
      };
    }
    bool Action::affectsGameplay(DWORD dwExecutionPlayer, DWORD dwCheckingPlayer) const
    {
      // Ignore disabled actions
      if ( this->bFlags & TriggerEntryFlags::IgnoreExecution )
        return false;

      // Compare the action Type and weed out what we're looking for
      switch ( this->bActionType )
      {
      case Actions::CreateUnit:
      case Actions::CreateUnitwithProperties:
        return playerCanMatch(this->dwSourceGroup, dwExecutionPlayer, dwCheckingPlayer);
      case Actions::GiveUnitstoPlayer:
        return playerCanMatch(this->dwExtra, dwExecutionPlayer, dwCheckingPlayer);
      default:
        break;
      }
      return false;
    }
    bool Condition::canBeTrue() const
    {
      // Return false only if the condition will always return false, preventing trigger execution
      return this->bConditionType != Conditions::Never && !(this->bFlags & TriggerEntryFlags::IgnoreExecution);
    }
  };
};
