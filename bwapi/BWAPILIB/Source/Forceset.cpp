#include <BWAPI/Vectorset.h>
#include <BWAPI/Forceset.h>
#include <BWAPI/Force.h>
#include <BWAPI/Playerset.h>

#include <utility>

namespace BWAPI
{
  Forceset::Forceset(size_t initialSize) : Vectorset(initialSize) { };
  Forceset::Forceset(const Forceset &other) : Vectorset(other) { };
  Forceset::Forceset(Forceset &&other) : Vectorset( std::forward<Forceset>(other) ) { };

  Playerset Forceset::getPlayers() const
  {
    Playerset rset;
    for ( auto f = this->begin(); f != this->end(); ++f )
      rset |= f->getPlayers();
    return rset;
  }
}

