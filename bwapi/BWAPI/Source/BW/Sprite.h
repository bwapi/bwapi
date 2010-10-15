#pragma once
#pragma pack(1)
#include "Position.h"
#include "Image.h"

namespace BW { struct Image::CImage; };

namespace BW
{
  struct CSprite
  {
    CSprite  *prev;
    CSprite  *next;
    u16      spriteID;
    u8       playerID;
    u8       selectionIndex; //0 <= selectionIndex <= 11. Index in the selection area at bottom of screen.
    u8       visibilityFlags;
    u8       elevationLevel;
    u8       selectionFlags; //lowest bit is selection circle
    u8       selectionTimer;
    u32      index;
    Position position;
    Image::CImage   *mainGraphic;
    Image::CImage   *underlay;
    Image::CImage   *overlay;
  };
};
#pragma pack()
