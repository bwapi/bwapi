#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Player_Ally : public Opcode
  {
  public:
    // Ctor
    Player_Ally(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
