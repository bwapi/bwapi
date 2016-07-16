#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Debug : public Opcode
  {
  public:
    // Ctor
    Debug(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
