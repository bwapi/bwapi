#include "Debug.h"
#include "Controller.h"

using namespace AISCRIPT;

Debug debug_impl(AISCRIPT::Enum::DEBUG);
Debug fatal_error_impl(AISCRIPT::Enum::FATAL_ERROR);

bool Debug::execute(aithread &thread) const
{
  // Perform independant behaviours
  if ( this->getOpcode() == AISCRIPT::Enum::FATAL_ERROR )
  {
    // Message
    Broodwar->sendText("Illegal AI script executed.");

    // Debug
    thread.saveDebug(Text::Green, this->getOpcode());
    
    // Kill the script
    thread.killThread();
    thread.noretry();
    return false;
  }
  else if ( this->getOpcode() == AISCRIPT::Enum::DEBUG )
  {
    // Read parameters
    WORD wJump;
    thread.readTuple( std::tie( wJump) );

    // Send the message
    Broodwar->sendText("%s", &pbAIScriptBinary[thread.getScriptOffset()] );

    // Jump to offset
    thread.setScriptOffset(wJump);
    thread.saveDebug(Text::Green, this->getOpcode(), "p_%X %s", wJump, &pbAIScriptBinary[thread.getScriptOffset()]);
  }
  return true;
}
