#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Rush : public Opcode
  {
  public:
    // Ctor
    Rush() : Opcode(Enum::RUSH) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
