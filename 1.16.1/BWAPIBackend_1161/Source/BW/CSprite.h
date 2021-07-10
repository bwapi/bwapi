#pragma once
#include "Position.h"

#pragma pack(1)
namespace BW
{
  class CImage;

  // Must be on 4-byte alignment
  class CSprite
  {
  public:
    /*0x00*/ CSprite*   prev;
    /*0x04*/ CSprite*   next;
    /*0x08*/ u16        spriteID;
    /*0x0A*/ u8         playerID;      // officially "creator"
    /*0x0B*/ u8         selectionIndex; //0 <= selectionIndex <= 11. Index in the selection area at bottom of screen.
    /*0x0C*/ u8         visibilityFlags;  // Player bits indicating the visibility for a player
    /*0x0D*/ u8         elevationLevel;
    /*0x0E*/ u8         flags; 
                        /*   0x01  Draw selection circle.
                             0x02  
                             0x04  
                             0x08  Selected.
                             0x10  
                             0x20  Hidden
                             0x40  Burrowed
                             0x80  Iscript unbreakable code section.
                        */
    /*0x0F*/ u8         selectionTimer;
    /*0x10*/ u16        index;
    /*0x12*/ u8         unkflags_12;
    /*0x13*/ u8         unkflags_13;
    /*0x14*/ Position   position;
    /*0x18*/ CImage*    pImagePrimary;    // official
    /*0x1C*/ CImage*    pImageHead;       // official
    /*0x20*/ CImage*    pImageTail;       // official

    ////////////////////////////////////////////////////////////////////
    // Official Broodwar methods (from beta), ignore these
    /*
    bool  Dead() const;
    bool  HasDamageFrames() const;
    */
  };

  static_assert( sizeof(CSprite) == 36, "BW::CSprite is incorrect." );
};
#pragma pack()
