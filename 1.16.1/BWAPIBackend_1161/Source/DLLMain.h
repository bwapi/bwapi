#pragma once
#include <string>

void __fastcall QueueGameCommand(void *pBuffer, size_t dwLength);

template <class T, class ... ArgTypes>
void QueueCommand(ArgTypes ... args) { T cmd = T(args...); QueueGameCommand(&cmd, sizeof(T)); }

#define QUEUE_COMMAND(x,...) QueueCommand<x>(__VA_ARGS__)

void BWAPIError(const char *format, ...);
void BWAPIError(DWORD dwErrCode, const char *format, ...);
int getFileType(const std::string &sFileName);

