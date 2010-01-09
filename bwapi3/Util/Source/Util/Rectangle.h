#pragma once

#include "Point.h"

namespace Util
{
  template<typename T> struct Rectangle
  {
    Point<T> pos;
    Point<T> size;

    //------------------------ CONSTRUCTORS -----------------------------
    Rectangle()
    {
    }

    Rectangle(Point<T> pos, Point<T> size)
      : pos(pos)
      , size(size)
    {
    }

    static Rectangle<T> from2Points(Point<T> pos, Point<T> pos2)
    {
      return Rectangle<T>(pos, pos2 - pos);
    }

    //------------------------ GETTERS ----------------------------------
    Point<T> getTopLeft() const
    {
      return pos;
    }
    Point<T> getTopRight() const
    {
      return Point<T>(pos.x + size.x, pos.y);
    }
    Point<T> getBottomLeft() const
    {
      return Point<T>(pos.x, pos.y + size.y);
    }
    Point<T> getBottomRight() const
    {
      return pos + size;
    }

    bool isEmpty() const
    {
      return size.isEmpty();
    }

    //------------------------ FUNCTIONALITY ----------------------------
    Rectangle<T> getBounding(const Rectangle<T>& b)
    {
      return Rectangle<T>(Point<T>(min(this->pos.x, b.pos.x), min(this->pos.y, b.pos.y)),
        Point<T>(min(this->size.x, b.size.x), min(this->size.y, b.size.y)));
    }
  };
}
