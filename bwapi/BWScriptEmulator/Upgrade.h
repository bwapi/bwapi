#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Upgrade : public Opcode
  {
  public:
    // Ctor
    Upgrade() : Opcode(Enum::UPGRADE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
