#pragma once
#include "StaticGameData.h"
#include "SendTextEvent.h"
#include "KnownUnitEntry.h"

#include <Util\Pipe.h>
#include <Util\RemoteProcess.h>
#include <Util\SharedSet.h>
#include <Util\SharedStack.h>
#include <Util\SharedStructure.h>

#include <BWAPITypes\UnitState.h>

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

    typedef Util::SharedStructure<StaticGameData> StaticGameDataStructure;
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
