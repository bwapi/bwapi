#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Killable : public Opcode
  {
  public:
    // Ctor
    Killable() : Opcode(Enum::KILLABLE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
