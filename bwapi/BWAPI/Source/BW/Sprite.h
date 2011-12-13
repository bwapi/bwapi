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
    /*0x0E*/ u8        flags; 
                       /*   0x01  Draw selection circle.
                            0x02  
                            0x04  
                            0x08  Selected.
                            0x10  
                            0x20  Hidden
                            0x40  Burrowed
                            0x80  Iscript unbreakable code section.
                       */
    /*0x0F*/ u8        selectionTimer;
    /*0x10*/ u32       index;
    /*0x14*/ Position  position;
    /*0x18*/ Image     *mainGraphic;
    /*0x1C*/ Image     *underlay;
    /*0x20*/ Image     *overlay;
  };

  CTASSERT( sizeof(Sprite) == 36 );
};
#pragma pack()
