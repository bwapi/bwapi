#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Attack_Clear : public Opcode
  {
  public:
    // Ctor
    Attack_Clear() : Opcode(Enum::ATTACK_CLEAR) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
