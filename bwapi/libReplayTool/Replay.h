#pragma once

#include "ReplayToolDefs.h"

START_REPLAY_TOOL

#define RFLAG_EXTRACT 1
#define RFLAG_REPAIR  2

class ParseReplayParams;

extern "C" bool parseReplay(const ParseReplayParams& params, DWORD dwFlags = 0);

END_REPLAY_TOOL