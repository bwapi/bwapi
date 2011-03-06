#pragma once
#include "Position.h"
#include "Image.h"

namespace BW { class Image; };

#pragma pack(1)
namespace BW
{
  class Sprite
  {
  public:
    /*0x00*/ Sprite    *prev;
    /*0x04*/ Sprite    *next;
    /*0x08*/ u16       spriteID;
    /*0x0A*/ u8        playerID;
    /*0x0B*/ u8        selectionIndex; //0 <= selectionIndex <= 11. Index in the selection area at bottom of screen.
    /*0x0C*/ u8        visibilityFlags;
    /*0x0D*/ u8        elevationLevel;
    /*0x0E*/ u8        selectionFlags; //lowest bit is selection circle
    /*0x0F*/ u8        selectionTimer;
    /*0x10*/ u32       index;
    /*0x14*/ Position  position;
    /*0x18*/ Image     *mainGraphic;
    /*0x1C*/ Image     *underlay;
    /*0x20*/ Image     *overlay;
  };
};
#pragma pack()
