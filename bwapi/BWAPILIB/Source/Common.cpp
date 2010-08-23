#include "Common.h"

namespace BWAPI
{
  void fixName(std::string *name)
  {
    for(unsigned int j = 0; j < name->length(); ++j)
    {
      if ( (*name)[j] == ' ')
        (*name)[j] = '_';
      if ( (*name)[j] >= 'a' && (*name)[j] <= 'z')
        (*name)[j] += 'A'-'a';
    }
  }

}