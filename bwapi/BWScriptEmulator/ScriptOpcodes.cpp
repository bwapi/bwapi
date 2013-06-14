#include "ScriptThread.h"
#include "ScriptEnum.h"
#include "BWScriptEmulator.h"
#include "Controller.h"
#include "UnitInfo.h"
#include "Opcode.h"

#include <BWAPI.h>

using namespace BWAPI;

void aithread::execute()
{
  // Reset bytes read
  this->dwBytesRead = 0;

  // Check if binary is loaded
  if ( !pbAIScriptBinary )
  {
    Broodwar << Text::Red << "No binary loaded!" << std::endl;
    return this->killThread();
  }

  // Read opcodes until one of them requests a break
  while ( AISCRIPT::Opcode::readOpcode(*this) )
  {
    if ( this->dwScriptOffset == 0 )
    {
      Broodwar << Text::Red << "Script offset is 0!" << std::endl;
      return this->killThread();
    }
  } // read loop

}
