#pragma once

#include "ParamTraits.h"
#include "GameAction.h"

START_REPLAY_TOOL

template<class TParam, ActionID ID,  class TParamFormatter = ParamTraits<TParam>>
class GenericAction1P : public GameAction
{
public:
  GenericAction1P(PlayerID player)
    : GameAction(player, ID)
    , param(TParam())
  {
  }

  GenericAction1P(PlayerID player, const TParam& myParam)
    : GameAction(player, ID)
    , param(myParam)
  {
  }

  void read(AbstractReplayReader &reader) { param = ParamTraits<TParam>::read(reader); }

  std::string toString() const
  {
    std::string str = GameAction::toString();

    str += TParamFormatter::toString(param);

    return str;
  }

protected:
  const TParam& getParam() const { return param; }

private:
  TParam param;
};

template<class TParam1, class TParam2, ActionID ID, class ParamsFormatter>
class GenericAction2P : public GameAction
{
public:
  GenericAction2P(PlayerID player)
    : GameAction(player, ID)
    , param1(TParam1())
    , param2(TParam2())
  {
  }

  GenericAction2P(PlayerID player, const TParam1& myParam1, const TParam2& myParam2)
    : GameAction(player, ID)
    , param1(myParam1)
    , param2(myParam2)
  {
  }

  void read(AbstractReplayReader &reader) 
  {
    param1 = ParamTraits<TParam1>::read(reader);
    param2 = ParamTraits<TParam2>::read(reader);
  }

  std::string toString() const
  {
    std::string str = GameAction::toString();

    str += ParamsFormatter::toString(param1, param2);

    return str;
  }

protected:
  const TParam1& getParam1() const { return param1; }
  const TParam2& getParam2() const { return param2; }

private:
  TParam1 param1;
  TParam2 param2;
};

#define IMPLEMENT_CTOR1(ClassName, TParam) \
  ClassName(PlayerID player) : GenericAction1P(player) {} \
  \
  ClassName(PlayerID player, const TParam& param) : GenericAction1P(player, param) {}

#define IMPLEMENT_CTOR2(ClassName, TParam1, TParam2) \
  ClassName(PlayerID player) : GenericAction2P(player) {} \
  \
  ClassName(PlayerID player, const TParam1& param1, const TParam2& param2) : GenericAction2P(player, param1, param2) {}

END_REPLAY_TOOL