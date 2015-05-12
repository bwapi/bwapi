#pragma once
#include "BW/Bitmap.h"

namespace BW
{
  struct bounds;
}

void GameUpdate(BW::Bitmap *pSurface, BW::bounds *pBounds);

extern BW::Bitmap bmpTerrainCache;
