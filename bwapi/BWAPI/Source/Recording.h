#pragma once

#include <windows.h>
#include "WMode.h"

extern bool recordingStarted;

bool StartVideoRecording(int width, int height);
bool StopVideoRecording();
void RecordFrame(void *pBuffer);

