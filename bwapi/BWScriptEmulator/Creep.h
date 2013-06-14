#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Creep : public Opcode
  {
  public:
    // Ctor
    Creep() : Opcode(Enum::CREEP) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
