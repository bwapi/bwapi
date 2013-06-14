#pragma once
#include <string>
#include "BW/Offsets.h"

void __fastcall QueueGameCommand(void *pBuffer, size_t dwLength);

#define QUEUE_COMMAND(x,...) QueueGameCommand(&x(__VA_ARGS__), sizeof(x))

void BWAPIError(const char *format, ...);
void BWAPIError(DWORD dwErrCode, const char *format, ...);
int getFileType(const char *szFileName);

