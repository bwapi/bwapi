#include <BWAPI/Vectorset.h>
#include <BWAPI/Bulletset.h>

#include <utility>

namespace BWAPI
{
  Bulletset::Bulletset(size_t initialSize) : Vectorset(initialSize) { };
  Bulletset::Bulletset(const Bulletset &other) : Vectorset(other) { };
  Bulletset::Bulletset(Bulletset &&other) : Vectorset( std::forward<Bulletset>(other) ) { };

}

