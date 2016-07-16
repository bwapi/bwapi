#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Attack_Do : public Opcode
  {
  public:
    // Ctor
    Attack_Do() : Opcode(Enum::ATTACK_DO) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
