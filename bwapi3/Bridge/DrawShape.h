#pragma once
#include <BWAPITypes\Position.h>
#include <BWAPITypes\CoordinateBase.h>
#include <Util\TypeHead.h>

namespace Bridge
{
  namespace DrawShape
  {
#pragma pack(push, 1)
#define UNIQUE_ID __LINE__
    struct Line : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::CoordinateBase base;
      BWAPI::Position from;
      BWAPI::Position to;
      int color;
    };

    struct Ellipse : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::CoordinateBase base;
      BWAPI::Position pos;
      int sizex;
      int sizey;
      int color;
      bool isSolid;
    };

    struct Circle : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::CoordinateBase base;
      BWAPI::Position center;
      double radius;
      int color;
      bool isSolid;
    };

    struct Rectangle : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::CoordinateBase base;
      BWAPI::Position pos;
      BWAPI::Position size;
      int color;
      bool isSolid;
    };

    struct Dot : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::CoordinateBase base;
      BWAPI::Position pos;
      int color;
    };

    struct Text : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::CoordinateBase base;
      BWAPI::Position pos;
    };

    struct Triangle : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::CoordinateBase base;
      BWAPI::Position posa;
      BWAPI::Position posb;
      BWAPI::Position posc;
      int color;
      bool isSolid;
    };
#undef UNIQUE_ID
#pragma pack(pop)
  }
}
