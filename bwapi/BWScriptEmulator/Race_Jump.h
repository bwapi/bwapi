#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Race_Jump : public Opcode
  {
  public:
    // Ctor
    Race_Jump() : Opcode(Enum::RACE_JUMP) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
