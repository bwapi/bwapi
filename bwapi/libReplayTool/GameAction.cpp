#include "GameAction.h"
#include <cstdio>
#include "DefaultActions.h"
#include <sstream>

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

void GameAction::Factory::add(GameAction* prototype)
{
  if (prototype != nullptr)
  {
    if (actions.find(prototype->getAction()) == actions.end())
      actions.insert(make_pair(prototype->getAction(), prototype));
  }
}

GameAction* GameAction::Factory::create(PlayerID player, ActionID action)
{
  ActionMap::iterator itr = actions.find(action);

  if (itr != actions.end())
    return (*itr).second->from(player);
  else
    return nullptr;
}

GameAction::Factory& GameAction::Factory::instance()
{
  static Factory instance;
  return instance;
}

GameAction::GameAction(PlayerID _player, ActionID _action)
  :player(_player)
  ,action(_action)
{
}

string GameAction::toString() const
{
  const char* actionName = action < ReplayTool::Max ? ReplayTool::pszActionNames[action] : "INVALID";
  
  ostringstream ss("(P");
  ss << player << ") " << actionName << ": ";
  
  return ss.str();
}