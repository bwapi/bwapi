#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Set_RandomSeed : public Opcode
  {
  public:
    // Ctor
    Set_RandomSeed() : Opcode(Enum::SET_RANDOMSEED) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
