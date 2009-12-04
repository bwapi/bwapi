#pragma once
#include "StaticGameDataStructure.h"
//#include "UnitStruct.h"
#include "SendTextEvent.h"

#include <Util\Pipe.h>
#include <Util\RemoteProcess.h>
#include <Util\SharedSet.h>
#include <Util\SharedStack.h>
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

    typedef Util::SharedStructure<CommandDataStructure> SharedCommandDataStructure;
    SharedCommandDataStructure commandData;

    typedef Util::SharedStack UserInputStack;
    UserInputStack userInput;
  };
}
