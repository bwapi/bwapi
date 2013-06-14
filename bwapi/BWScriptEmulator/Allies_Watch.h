#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Allies_Watch : public Opcode
  {
  public:
    // Ctor
    Allies_Watch() : Opcode(Enum::ALLIES_WATCH) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
