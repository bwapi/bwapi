#pragma once
#include <string>

void __fastcall QueueGameCommand(void *pBuffer, size_t dwLength);

#define QUEUE_COMMAND(x,...) QueueGameCommand(&x(__VA_ARGS__), sizeof(x))

void BWAPIError(const char *format, ...);
void BWAPIError(DWORD dwErrCode, const char *format, ...);
int getFileType(const std::string &sFileName);

