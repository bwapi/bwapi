#include "ActionParser.h"
#include <cstdio>
#include <fstream>
#include <list>
#include "DefaultActions.h"
#include "FileReader.h"
#include "BWAPI.h"
#include "Logger.h"
#include "ReplayReader.h"
#include "StrUtil.h"

using namespace BWAPI;
using namespace std;
using namespace ReplayTool;

extern "C" void parseActions(AbstractReplayReader &rr, list<GameAction*> &actions)
{
  while (rr.isGood())
  {
    // Begin reading a new replay frame
    rr.newFrame();
    while (rr.isGood() && rr.isValidFrame())
    {
      // Get the player ID and opcode for the current command being executed
      BYTE bPlayerID = rr.readBYTE();
      BYTE bCommand  = rr.readBYTE();

      GameAction* action = GameAction::Factory::instance().create(bPlayerID, bCommand);

      // Action not parameterized, which means that it is not registered in the action factory
      // In this general case, just represent it with a generic GameAction
      if (NULL == action)
      {
        action = new GameAction(bPlayerID, bCommand);
      }
      else
      {
        action->read(rr);
      }

      actions.push_back(action);
    } // per-frame loop

  } // per-replay loop
}

extern "C" void logActions(const list<GameAction*> &actions, const char *logFilename)
{
  Logger actionLog(logFilename);
  string dbgStr;
  unsigned actionNum = 0;

  for(list<GameAction*>::const_iterator itr = actions.begin(); itr != actions.end(); ++itr)
  {
    dbgStr = StrUtil::format("%d: %s\n", actionNum, (*itr)->toString().c_str());
    ++actionNum;

    OutputDebugStringA(dbgStr.c_str());

    actionLog.writeLine((*itr)->toString().c_str());
  }
}