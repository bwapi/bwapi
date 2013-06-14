#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Max_Force : public Opcode
  {
  public:
    // Ctor
    Max_Force() : Opcode(Enum::MAX_FORCE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
