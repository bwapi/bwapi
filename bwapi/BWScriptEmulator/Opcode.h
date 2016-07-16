#pragma once
#include <array>
#include <string>
#include "ScriptEnum.h"
#include "ScriptThread.h"

#define IMPLEMENT(x) x impl ## x

namespace AISCRIPT
{
  class Opcode
  {
  public:
    // Mapping of opcodes
    static std::array<Opcode*,AISCRIPT::Enum::LAST> opcodeList;

    // Constructor
    Opcode(AISCRIPT::Enum::Enum id);

    // Execute an opcode.
    // Return true if script execution should continue, false if execution should break
    virtual bool execute(aithread &thread) const = 0;
    
    // Get name of the opcode
    const char * const getName() const;
    AISCRIPT::Enum::Enum getOpcode() const;

    // Read an opcode from the script file
    // Return true if we should continue reading the script or false if we should return
    // execution to Starcraft.
    static bool readOpcode(aithread &thread);

  private:
    AISCRIPT::Enum::Enum opcode;
  };

}
