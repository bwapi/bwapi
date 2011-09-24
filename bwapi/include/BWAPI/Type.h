#pragma once
#include <string>

namespace BWAPI
{
  class Type
  {
  private:
    int _id;
  public:
    Type(int id);
    operator int() const;
    int getID() const;
  };
}
