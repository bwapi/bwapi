#pragma once

namespace BWAPI
{
  namespace Math
  {
    struct Point
    {
        int x;
        int y;

        static Point Invalid;

        Point();
        Point(int x, int y);
        bool operator == (const Point& b) const;
        bool operator != (const Point& b) const;
        bool operator  < (const Point& b) const;
        Point operator + (const Point& b) const;
        Point operator - (const Point& b) const;
        Point& operator += (const Point& b);
        Point& operator -= (const Point& b);
        void exchange(Point& b);
        double getDistance(const Point& b) const;
        double getLength() const;
        bool isValid();
    };
  }
};

