#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Stop : public Opcode
  {
  public:
    // Ctor
    Stop() : Opcode(Enum::STOP) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
