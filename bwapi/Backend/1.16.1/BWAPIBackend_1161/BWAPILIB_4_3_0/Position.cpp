#include <BWAPI4/Position.h>
#include "../BWAPI/GameImpl.h"

#include "GameImpl.h"

namespace BWAPI4
{
  template<typename _T, int __Scale>
  bool Point<_T,__Scale>::isValid() const
  {
    // Not valid if < 0
    if ( this->x < 0 || this->y < 0 )
      return false;

    // If BW ptr exists then compare with actual map size
    return this->x < (BroodwarImpl.mapWidth()  * 32)/__Scale && 
           this->y < (BroodwarImpl.mapHeight() * 32)/__Scale;
  }

  template<typename _T, int __Scale>
  Point<_T,__Scale> &Point<_T,__Scale>::makeValid()
  {
    // Set x/y to 0 if less than 0
    this->setMin(0, 0);

    // If BW ptr exists then set it to map width/height
    _T wid = (_T)(BroodwarImpl.mapWidth() * 32)/__Scale - 1;
    _T hgt = (_T)(BroodwarImpl.mapHeight() * 32)/__Scale - 1;
    this->setMax(wid,hgt);
    return *this;
  };

  template Point<int,32> &Point<int,32>::makeValid();
  template Point<int,8> &Point<int,8>::makeValid();
  template Point<int,1> &Point<int,1>::makeValid();
  template Point<short,32> &Point<short,32>::makeValid();
  template Point<short,1> &Point<short,1>::makeValid();
  
  template bool Point<int,32>::isValid() const;
  template bool Point<int,8>::isValid() const;
  template bool Point<int,1>::isValid() const;
  template bool Point<short,32>::isValid() const;
  template bool Point<short,1>::isValid() const;
}

