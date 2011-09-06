#pragma once

#include <windows.h>

#define RFLAG_EXTRACT 1
#define RFLAG_REPAIR  2

bool ParseReplay(const char *pszFilename, DWORD dwFlags = 0);
