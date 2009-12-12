#pragma once
#include <BW\Position.h>
#include <Util\TypeHead.h>

namespace Bridge
{
  namespace DrawShape
  {
#pragma pack(push, 1)
#define UNIQUE_ID __LINE__
    struct Line : Util::TypeHead<UNIQUE_ID>
    {
      BW::Position from;
      BW::Position to;
      int color;
    };

    struct Circle : Util::TypeHead<UNIQUE_ID>
    {
      BW::Position center;
      double radius;
      int color;
      bool isSolid;
    };

    struct Rectangle : Util::TypeHead<UNIQUE_ID>
    {
      BW::Position pos;
      BW::Position size;
      int color;
      bool isSolid;
    };

    struct Dot : Util::TypeHead<UNIQUE_ID>
    {
      BW::Position pos;
      int color;
    };

    struct Text : Util::TypeHead<UNIQUE_ID>
    {
      BW::Position pos;
    };
#undef UNIQUE_ID
#pragma pack(pop)
  }
}
