#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Random_Jump : public Opcode
  {
  public:
    // Ctor
    Random_Jump() : Opcode(Enum::RANDOM_JUMP) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
