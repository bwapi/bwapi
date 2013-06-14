#include <BWAPI/Position.h>
#include <BWAPI/Game.h>

namespace BWAPI
{
  template<typename _T, int __Scale>
  bool Point<_T,__Scale>::isValid() const
  {
    // Not valid if < 0
    if ( this->x < 0 || this->y < 0 )
      return false;
    
    // If Broodwar pointer is not initialized, just assume maximum map size
    if ( !BroodwarPtr ) 
      return this->x < (256 * 32)/__Scale &&
              this->y < (256 * 32)/__Scale;

    // If BW ptr exists then compare with actual map size
    return this->x < (Broodwar->mapWidth()  * 32)/__Scale && 
           this->y < (Broodwar->mapHeight() * 32)/__Scale;
  }

  template<typename _T, int __Scale>
  Point<_T,__Scale> &Point<_T,__Scale>::makeValid()
  {
    // Set x/y to 0 if less than 0
    this->setMin(0, 0);
    
    if ( !BroodwarPtr )  // If broodwar ptr doesn't exist, set to below max size
    {
      _T max = (256*32)/__Scale - 1;
      this->setMax(max, max);
      return *this;
    }

    // If BW ptr exists then set it to map width/height
    _T wid = (_T)(Broodwar->mapWidth() * 32)/__Scale - 1;
    _T hgt = (_T)(Broodwar->mapHeight() * 32)/__Scale - 1;
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

