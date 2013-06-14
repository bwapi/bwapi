#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Groundmap_Jump : public Opcode
  {
  public:
    // Ctor
    Groundmap_Jump() : Opcode(Enum::GROUNDMAP_JUMP) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
