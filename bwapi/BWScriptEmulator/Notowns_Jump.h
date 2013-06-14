#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Notowns_Jump : public Opcode
  {
  public:
    // Ctor
    Notowns_Jump() : Opcode(Enum::NOTOWNS_JUMP) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
