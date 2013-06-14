#pragma once

#include "GenericAction1P.h"

START_REPLAY_TOOL

class AbstractReplayReader;

struct SaveLoadActionParamFormatter
{
  static std::string toString(DWORD saveInfo, std::string savePath) 
  {
    return StrUtil::format("0x%08X, %s", saveInfo, savePath.c_str());
  }
};

class SaveGameAction : public GenericAction2P<DWORD, std::string, ReplayTool::Save_Game, SaveLoadActionParamFormatter>
{
public:
  IMPLEMENT_CTOR2(SaveGameAction, DWORD, std::string);
  IMPLEMENT_FROM(SaveGameAction);
  DWORD getSaveInfo() const { return getParam1(); }
  std::string getSavePath() const { return getParam2(); }
};

class LoadGameAction : public GenericAction2P<DWORD, std::string, ReplayTool::Load_Game, SaveLoadActionParamFormatter>
{
public:
  IMPLEMENT_CTOR2(LoadGameAction, DWORD, std::string);
  IMPLEMENT_FROM(LoadGameAction);
  DWORD getLoadInfo() const { return getParam1(); }
  std::string getLoadPath() const { return getParam2(); }
};

END_REPLAY_TOOL