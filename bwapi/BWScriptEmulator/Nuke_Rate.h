#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Nuke_Rate : public Opcode
  {
  public:
    // Ctor
    Nuke_Rate() : Opcode(Enum::NUKE_RATE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
