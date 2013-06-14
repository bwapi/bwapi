#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Wait : public Opcode
  {
  public:
    // Ctor
    Wait() : Opcode(Enum::WAIT) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
