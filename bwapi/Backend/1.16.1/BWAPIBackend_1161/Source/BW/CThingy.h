#pragma once

#pragma pack(1)
namespace BW
{
  class CSprite;

  class CThingy
  {
  public:
    CThingy *prev;
    CThingy *next;
    int param;
    BW::CSprite *sprite;
  };



}
#pragma pack()
