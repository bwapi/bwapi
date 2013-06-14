#pragma once

#include "WMode.h"

extern bool recordingStarted;

extern void *pVidBuffer;

bool StartVideoRecording(int width, int height);
bool StopVideoRecording();
void RecordFrame(void *pBuffer);

