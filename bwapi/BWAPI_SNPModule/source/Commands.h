#pragma once
#include <windows.h>

#include "LocalPC.h"
#include "Threads.h"

void BroadcastAdvertisement(SOCKADDR *to = NULL);
void BroadcastGameListRequest();
void CleanGameList(DWORD dwTimeout);
void UpdateGameList(SOCKADDR_IN *from, char *data, bool remove);
