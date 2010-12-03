#ifndef __GAMEINFO_H__
#define __GAMEINFO_H__
#pragma once

struct GameInfo
{
	int szSize;
	int flType;
	DWORD dwGameState;
	char chGameName[128];
	char chGameStats[128];
};

#endif // __GAMEINFO_H__