#pragma once

#include <map>
#include "BWAPI.h"
#include "ReplayToolDefs.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class GameAction
{
public:
  class Factory
  {
  public:
    void add(GameAction* prototype);
    GameAction* create(PlayerID player, ActionID AbstractAction);
    static Factory& instance();

  private:
    typedef std::map<ActionID, GameAction*> ActionMap;
    ActionMap actions;
  };

  GameAction(PlayerID player, ActionID AbstractAction);
  virtual ~GameAction() {}

  virtual void read(AbstractReplayReader &reader) {}
  virtual GameAction* from(PlayerID player) { return new GameAction(player, action); }
  virtual std::string toString() const;

  PlayerID getPlayer() const { return player; }
  ActionID getAction() const { return action; }

  template<class TAction>
  static GameAction* create(PlayerID player) { return new TAction(player); }

  template<class TAction, class TParam>
  static GameAction* create1(PlayerID player, TParam param) { return new TAction(player, param); }

private:
  PlayerID player;
  ActionID action;
};

#define g_actionFactory GameAction::Factory::instance()

#define REGISTER_ACTION(Name, ID) ReplayTool::GameAction::Factory::instance().add(ReplayTool::GameAction::create<Name>(0))
#define REGISTER_ACTION_WITH_1_PARAM(Name, ID, ParamValue) ReplayTool::GameAction::Factory::instance().add(ReplayTool::GameAction::create1<Name>(0, ParamValue))

#define IMPLEMENT_FROM(Name) GameAction* from(PlayerID player) { return create<Name>(player); }
#define IMPLEMENT_FROM_WITH_1_PARAM(Name, ParamValue) GameAction* from(PlayerID player) { return create1<Name>(player, ParamValue); }

END_REPLAY_TOOL