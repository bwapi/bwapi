#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Give_Money : public Opcode
  {
  public:
    // Ctor
    Give_Money() : Opcode(Enum::GIVE_MONEY) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
