#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Panic : public Opcode
  {
  public:
    // Ctor
    Panic() : Opcode(Enum::PANIC) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
