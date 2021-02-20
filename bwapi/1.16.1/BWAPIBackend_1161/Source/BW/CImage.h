#pragma once

#include "Position.h"

#pragma pack(1)
namespace BW
{
  class CSprite;

  struct grpFrame
  {
    s8  x;
    s8  y;
    s8  wid;
    s8  hgt;
    u32 dataOffset;
  };
  struct grpHead
  {
    u16      wFrames;  // official
    s16       width;
    s16       height;
    grpFrame  frames[1];
  };

  namespace Anims
  {
    enum Enum : u8
    {
      Init = 0x00,
      Death = 0x01,
      GndAttkInit = 0x02,
      AirAttkInit = 0x03,
      Unused1 = 0x04,
      GndAttkRpt = 0x05,
      AirAttkRpt = 0x06,
      CastSpell = 0x07,
      GndAttkToIdle = 0x08,
      AirAttkToIdle = 0x09,
      Unused2 = 0x0A,
      Walking = 0x0B,
      WalkingToIdle = 0x0C,
      SpecialState1 = 0x0D,
      SpecialState2 = 0x0E,
      AlmostBuilt = 0x0F,
      Built = 0x10,
      Landing = 0x11,
      LiftOff = 0x12,
      IsWorking = 0x13,
      WorkingToIdle = 0x14,
      WarpIn = 0x15,
      Unused3 = 0x16,
      StarEditInit = 0x17,
      Disable = 0x18,
      Burrow = 0x19,
      UnBurrow = 0x1A,
      Enable = 0x1B
    };
  };

  // Must be on a 4-byte alignment
  class CImage
  {
  public:
    /*0x00*/ CImage*      prev;
    /*0x04*/ CImage*      next;
    /*0x08*/ u16          imageID;        // officially "uwNo"
    /*0x0A*/ u8           paletteType;    /* officially "ubRLE"
                                              RLE_NORMAL = 0
                                              RLE_CLOAK = 5
                                              RLE_CLOAKED = 6
                                              RLE_DECLOAK = 7
                                              RLE_EFFECT = 9
                                              RLE_SHADOW = 10 (official)
                                              RLE_HPFLOATDRAW = 11 (official)
                                              RLE_WARP_IN = 12
                                              RLE_OUTLINE = 13
                                              RLE_PLAYER_SIDE = 14 (official)
                                              RLE_SHIFT = 16
                                              RLE_FIRE = 17
                                          */
    /*0x0B*/ u8           direction;
    /*0x0C*/ u16          flags;
                          /*  0x0001  - Redraw
                              0x0002  - Flipped/Mirrored
                              0x0004  - 
                              0x0008  - Has rotation frames
                              0x0010  - 
                              0x0020  - Clickable
                              0x0040  - Hidden/Invisible (don't draw)
                              0x0080  - 
                              0x0100  - 
                              0x0200  - 
                              0x0400  - 
                              0x0800  - 
                              0x1000  - 
                              0x2000  - 
                              0x4000  - 
                              0x8000  - 
                          */
    /*0x0E*/ s8           horizontalOffset;
    /*0x0F*/ s8           verticalOffset;
    /*0x10*/ u16          iscriptHeader;
    /*0x12*/ u16          iscriptOffset;
    /*0x14*/ u16          unknown_14;
    /*0x16*/ Anims::Enum  anim;
    /*0x17*/ u8           sleep;        // iscript sleep time
    /*0x18*/ u16          frameSet;
    /*0x1A*/ u16          frameIndex;
    /*0x1C*/ BW::Position mapPosition;
    /*0x20*/ BW::Position screenPosition;
    /*0x24*/ rect         grpBounds;      // Bounds for GRP frame, only different from normal when part of graphic is out of bounds.
    /*0x2C*/ grpHead*     GRPFile;
    /*0x30*/ void*        coloringData;
    //void__fastcall renderFunction(int screenX, int screenY, grpFrame *pFrame, RECT *grpRect, int colorData);
    /*0x34*/ void (__fastcall* renderFunction)(int,int,grpFrame*,rect*,int);    // officially "DrawFunction"
    /*0x38*/ void (__fastcall* updateFunction)(CImage*);                        // officially "UpdateFunction"
    /*0x3C*/ CSprite*     spriteOwner;

    ////////////////////////////////////////////////////////////////////
    // Official Broodwar methods (from beta), ignore these
    /*
    RunScript
    PathRunScript
    */
  };

  static_assert( sizeof(CImage) == 64, "BW::CImage is incorrect.");

};
#pragma pack()

