#pragma once

#include <Util\Pipe.h>
#include <Util\RemoteProcess.h>
#include <Util\SharedSet.h>
#include <Util\SharedStack.h>
#include <Util\SharedStructure.h>

#include <BWAPITypes\UnitState.h>
#include <BWAPITypes\StaticGameData.h>

namespace Bridge
{
  struct SharedStuff
  {
    // synchronisation means
    Util::Pipe pipe;
    Util::RemoteProcess remoteProcess;

    // shared memories

    // all limited-size data
    typedef Util::SharedStructure<BWAPI::StaticGameData> StaticGameDataStructure;
    StaticGameDataStructure staticData;

    // every Server->Client message goes through here
    typedef Util::SharedStack EventStack;
    EventStack events;

    // every Client->Server message goes through here
    typedef Util::SharedStack CommandStack;
    CommandStack commands;

    // shapes need special performance attention and are stored separately.
    typedef Util::SharedStack DrawShapeStack;
    DrawShapeStack drawShapes;
  };
}
