#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Multirun : public Opcode
  {
  public:
    // Ctor
    Multirun() : Opcode(Enum::MULTIRUN) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
