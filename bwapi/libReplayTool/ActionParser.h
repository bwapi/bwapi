#pragma once

#include "GameAction.h"
#include <list>

START_REPLAY_TOOL

class AbstractReplayReader;

extern "C" void parseActions(AbstractReplayReader &rr, std::list<GameAction*> &actions);
extern "C" void logActions(const std::list<GameAction*> &actions, const char *logFilename);

END_REPLAY_TOOL