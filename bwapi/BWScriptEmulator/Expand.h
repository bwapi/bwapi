#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Expand : public Opcode
  {
  public:
    // Ctor
    Expand() : Opcode(Enum::EXPAND) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
