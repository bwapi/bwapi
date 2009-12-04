#pragma once
#include <BW\Position.h>
#include <Util\TypeHead.h>

namespace Bridge
{
  namespace Doodle
  {
#pragma pack(push, 1)
#define UNIQUE_ID static const int _typeId = __LINE__;
    /* uncomment when BW::Position fixed
    struct Line
    {
      BW::Position from;
      BW::Position to;
      UNIQUE_ID;
    };

    struct Circle
    {
      BW::Position center;
      double radius;
      UNIQUE_ID;
    };

    struct Rectangle
    {
      BW::Position from;
      BW::Position to;
      UNIQUE_ID;
    };

    struct Text
    {
      BW::Position pos;
      Util::SharedStack::Index text;
      UNIQUE_ID;
    };

    struct DoodleEntry
    {
      int doodleType;
      union
      {
        Line lineData;
        Circle circleData;
        Rectangle rectangleData;
        Text textData;
      };
    };
    */
#undef UNIQUE_ID
#pragma pack(pop)
  }
}
