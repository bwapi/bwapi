#include "Common.h"

#include "../../Debug.h"
#include <string.h>

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

  const char *underscoresToSpaces(const char *pszTypename)
  {
    for ( int i = strlen(pszTypename); i > 0; i-- )
      if ( pszTypename[i] == '_' ) 
        ((char*)pszTypename)[i] = ' ';
    return pszTypename;
  }
}