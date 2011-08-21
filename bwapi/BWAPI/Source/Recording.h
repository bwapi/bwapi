#pragma once

#include <windows.h>
#include "WMode.h"

extern bool recordingStarted;

bool StartVideoRecording(const char *pszFilename, int width, int height);
bool StopVideoRecording();
void RecordFrame(void *pBuffer, int width, int height);

