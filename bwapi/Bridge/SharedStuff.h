#pragma once
#include "SendTextEvent.h"
#include "KnownUnitEntry.h"

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
    typedef Util::SharedSet<KnownUnitEntry> KnownUnitSet;
    KnownUnitSet knownUnits;

    typedef Util::SharedStack KnownUnitAddEventStack;
    KnownUnitAddEventStack knownUnitAddEvents;

    typedef Util::SharedStack KnownUnitRemoveEventStack;
    KnownUnitRemoveEventStack knownUnitRemoveEvents;

    typedef Util::SharedStructure<BWAPI::StaticGameData> StaticGameDataStructure;
    StaticGameDataStructure staticData;

    typedef Util::SharedStack CommandStack;
    CommandStack commands;

    typedef Util::SharedStack UserInputStack;
    UserInputStack userInput;

    typedef Util::SharedStack SendTextStack;
    SendTextStack sendText;

    typedef Util::SharedStack DrawShapeStack;
    DrawShapeStack drawShapes;
  };
}
