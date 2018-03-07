#pragma once

void DropMessage(int errorlevel, const char *format, ...); // 0-info 1-warning 2-error
void DropLastError(const char *format = "", ...);
char* sprintfBytes(void *bytes, int bytecount);
void OutputStatus(const char *format, ...);
