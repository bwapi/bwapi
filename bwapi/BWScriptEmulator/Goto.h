#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Goto : public Opcode
  {
  public:
    // Ctor
    Goto() : Opcode(Enum::GOTO) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
