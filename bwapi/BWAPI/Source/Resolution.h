#pragma once
#include "BW/Offsets.h"

void SetResolution(int width, int height);

void DDrawDestroy();
void DDrawInitialize(int width, int height);
void GameUpdate(BW::bitmap *pSurface, BW::bounds *pBounds);
