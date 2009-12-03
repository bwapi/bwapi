#pragma once
#include "StaticGameDataStructure.h"
//#include "UnitStruct.h"

#include <Util\Pipe.h>
#include <Util\RemoteProcess.h>
#include <Util\SharedSet.h>
#include <Util\SharedStructure.h>


namespace Bridge
{
  struct SharedStuff
  {
    // synchronisation means
    Util::Pipe pipe;
    Util::RemoteProcess remoteProcess;

    // shared memories
//    typedef Util::SharedSet<KnownUnit> SharedKnownUnitSet;
//    SharedKnownUnitSet knownUnitSet;
    typedef Util::SharedStructure<StaticGameDataStructure> SharedGameDataStructure;
    SharedGameDataStructure staticData;
  };
}
