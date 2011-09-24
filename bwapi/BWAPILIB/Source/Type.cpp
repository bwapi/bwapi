#include <BWAPI/Type.h>

namespace BWAPI
{
  Type::Type(int id) 
    : _id(id)
  {
  }
  Type::operator int() const
  { 
    return _id;
  }
  int Type::getID() const
  {
    return _id;
  }
}
