#pragma once
#include "Sprite.h"
#include <windows.h>

namespace BW { struct Sprite; };

#pragma pack(1)
namespace BW
{
  struct grpFrame
  {
    u8  left;
    u8  top;
    u8  right;
    u8  bottom;
    u32 dataOffset;
  };
  struct grpHead
  {
    u16       frameCount;
    u16       width;
    u16       height;
    grpFrame  frames[1];
  };
  struct Image
  {
    /* 0x00 */ Image        *prev;
    /* 0x04 */ Image        *next;
    /* 0x08 */ u16          imageID;
    /* 0x0A */ u8           paletteType;
    /* 0x0B */ u8           direction;
    /* 0x0C */ u16          flags;
    /* 0x0E */ s8           horizontalOffset;
    /* 0x0F */ s8           verticalOffset;
    /* 0x10 */ u16          iscriptHeader;
    /* 0x12 */ u16          iscriptOffset;
    /* 0x14 */ u16          unknown_14;
    /* 0x16 */ u8           anim;
    /* 0x17 */ u8           wait;
    /* 0x18 */ u16          frameSet;
    /* 0x1A */ u16          frameIndex;
    /* 0x1C */ BW::Position mapPosition;
    /* 0x20 */ BW::Position screenPosition;
    /* 0x24 */ s16          graphicLeft;    // For GRP file. Only altered when screen 
    /* 0x26 */ s16          graphicTop;     // puts part of graphic out of bounds. Also
    /* 0x28 */ s16          graphicRight;   // changes to match the graphic size when the
    /* 0x2A */ s16          graphicBottom;  // GRP frame index has changed.
    /* 0x2C */ void         *GRPFile;
    /* 0x30 */ void         *coloringData;
    // void (__fastcall *renderFunction)(int screenX, int screenY, grpFrame *pFrame, RECT *grpRect, int colorData);
    /* 0x34 */ void (__fastcall *renderFunction1)(int,int,grpFrame*,RECT*,int);
    /* 0x38 */ void (__fastcall *renderFunction2)(int,int,grpFrame*,RECT*,int);
    /* 0x3C */ Sprite       *spriteOwner;
  };
  namespace Anims
  {
    enum Enum
    {
      Init          = 0x00,
      Death         = 0x01,
      GndAttkInit   = 0x02,
      AirAttkInit   = 0x03,
      Unused1       = 0x04,
      GndAttkRpt    = 0x05,
      AirAttkRpt    = 0x06,
      CastSpell     = 0x07,
      GndAttkToIdle = 0x08,
      AirAttkToIdle = 0x09,
      Unused2       = 0x0A,
      Walking       = 0x0B,
      WalkingToIdle = 0x0C,
      SpecialState1 = 0x0D,
      SpecialState2 = 0x0E,
      AlmostBuilt   = 0x0F,
      Built         = 0x10,
      Landing       = 0x11,
      LiftOff       = 0x12,
      IsWorking     = 0x13,
      WorkingToIdle = 0x14,
      WarpIn        = 0x15,
      Unused3       = 0x16,
      StarEditInit  = 0x17,
      Disable       = 0x18,
      Burrow        = 0x19,
      UnBurrow      = 0x1A,
      Enable        = 0x1B
    };
  };

};
#pragma pack()
