#pragma once

#include "PlayerId.h"

#include <Util\StaticString.h>
#include <Util\StaticVector.h>

namespace BWAPI
{
  struct Force
  {
    Util::StaticString<32> name;
    Util::StaticVector<PlayerId, 8> players;
  };
}
