#pragma once
#include "Position.h"
#include "Image.h"
namespace BW
{
  struct CSprite
  {
    CSprite* prev;
    CSprite* next;
    u16      spriteID;
    u8       color;
    u8       unknown1;
    u8       visibilityFlags;
    u8       elevationLevel;
    u8       unknown2;
    u8       selectionTimer;
    u32      index;
    Position position;
    BW::Image::CImage   *underlay;
    BW::Image::CImage   *mainGraphic;
    BW::Image::CImage   *overlay;
  };
};

