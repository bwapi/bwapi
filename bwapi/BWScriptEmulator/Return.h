#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Return : public Opcode
  {
  public:
    // Ctor
    Return() : Opcode(Enum::RETURN) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
