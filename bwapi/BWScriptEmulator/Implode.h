#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Implode : public Opcode
  {
  public:
    // Ctor
    Implode() : Opcode(Enum::IMPLODE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
