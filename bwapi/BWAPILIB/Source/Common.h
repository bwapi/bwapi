#pragma once
#include <string>

namespace BWAPI
{
  void fixName(std::string *name);
  const char *underscoresToSpaces(const char *pszTypename);
}
