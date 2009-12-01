#pragma once
#include "KnownUnit.h"
#include "Util\Pipe.h"
#include "Util\RemoteProcess.h"
#include "Util\SharedSet.h"

namespace Bridge
{
  struct Structure
  {
    // synchronisation means
    Util::Pipe pipe;
    Util::RemoteProcess remoteProcess;

    // shared memories
    typedef Util::SharedSet<KnownUnit> SharedKnownUnitSet;
    SharedKnownUnitSet knownUnitSet;
  };
}
