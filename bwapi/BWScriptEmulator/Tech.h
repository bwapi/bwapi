#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Tech : public Opcode
  {
  public:
    // Ctor
    Tech() : Opcode(Enum::TECH) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
