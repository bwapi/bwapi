#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Time_Jump : public Opcode
  {
  public:
    // Ctor
    Time_Jump() : Opcode(Enum::TIME_JUMP) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
